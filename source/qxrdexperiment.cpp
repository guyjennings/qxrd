#include "qcepmacros.h"
#include "qxrdexperiment.h"
#include "qxrdfreshstartdialog.h"
#include "qxrdapplication.h"
#include "qxrddataprocessor.h"
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdserver.h"
#include "qxrdsimpleserver.h"
#include "qxrdscriptengine.h"
#include "qxrdexperimentpreferencesdialog.h"
#include "qxrdexperimentsettings.h"
#include <QFileDialog>

QxrdExperiment::QxrdExperiment(QString path,
                               QxrdApplication *app,
                               QSettings *settings,
                               QObject *parent) :
  QObject(parent),
  m_Saver(NULL, this),
  m_ExperimentKind(&m_Saver, this, "experimentKind", -1),
  m_ExperimentDirectory(&m_Saver, this, "experimentDirectory", defaultExperimentDirectory(path)),
  m_ExperimentFileName(&m_Saver, this, "experimentFileName", defaultExperimentFileName(path)),
  m_ExperimentName(&m_Saver, this, "experimentName", defaultExperimentName(path)),
  m_ExperimentDescription(&m_Saver, this, "experimentDescription", ""),
  m_LogFilePath(&m_Saver, this, "logFilePath", defaultLogName(path)),
  m_ScanFilePath(&m_Saver, this, "scanFilePath", defaultScanName(path)),
  m_DetectorType(&m_Saver, this,"detectorType", 1),
  m_ProcessorType(&m_Saver, this,"processorType", 0),
  m_DefaultLayout(NULL, this,"defaultLayout",0),
  m_RunSpecServer(&m_Saver, this,"runSpecServer", 1),
  m_SpecServerPort(&m_Saver, this,"specServerPort", -1),
  m_RunSimpleServer(&m_Saver, this,"runSimpleServer", 1),
  m_SimpleServerPort(&m_Saver, this,"simpleServerPort", 1234),
  m_Application(app),
  m_Window(NULL),
  m_Splash(NULL),
  m_ServerThread(NULL),
  m_Server(NULL),
  m_SimpleServerThread(NULL),
  m_SimpleServer(NULL),
  m_DataProcessorThread(NULL),
  m_DataProcessor(NULL),
  m_AcquisitionThread(NULL),
  m_Acquisition(NULL),
  m_FileSaverThread(NULL),
  m_ScriptEngineThread(NULL),
  m_ScriptEngine(NULL),
  m_ScriptEngineDebugger(NULL),
  m_LogFile(NULL),
  m_ScanFile(NULL)
{
  readSettings(settings);
}

QxrdSettingsSaver *QxrdExperiment::saver()
{
  return &m_Saver;
}

bool QxrdExperiment::init(QSettings *settings)
{
  GUI_THREAD_CHECK;

  if (m_Application->get_GuiWanted()) {
    m_Splash = new QxrdSplashScreen(NULL);
    m_Splash -> show();
  }

  setObjectName("QxrdExperiment");

  QThread::currentThread()->setObjectName("doc");

  splashMessage("Initializing File Saver");

  m_FileSaverThread = new QxrdFileSaverThread(m_Application->allocator());
  m_FileSaverThread -> setObjectName("saver");
  m_FileSaverThread -> start();

  splashMessage("Initializing Data Processing");

  m_DataProcessorThread = new QxrdDataProcessorThread(saver(),
                                                      this,
                                                      NULL,
                                                      m_Application->allocator(),
                                                      m_FileSaverThread,
                                                      settings,
                                                      "experiment/processor");
  m_DataProcessorThread -> setObjectName("proc");
  m_DataProcessorThread -> start();
  m_DataProcessor = m_DataProcessorThread -> dataProcessor();

  m_FileSaverThread -> setProcessor(m_DataProcessor);

  splashMessage("Initializing Data Acquisition");

  m_AcquisitionThread = new QxrdAcquisitionThread(saver(),
                                                  this,
                                                  m_DataProcessor,
                                                  m_Application->allocator(),
                                                  get_DetectorType(),
                                                  settings,
                                                  "experiment/acquire");
  m_AcquisitionThread -> setObjectName("acqu");
  m_AcquisitionThread -> start();
  m_Acquisition = m_AcquisitionThread -> acquisition();

  m_DataProcessor -> setAcquisition(m_Acquisition);
  m_FileSaverThread -> setAcquisition(m_Acquisition);


  if (m_Application->get_GuiWanted()) {
    splashMessage("Opening Main Window");
    m_Window = new QxrdWindow(saver(),
                              m_Application,
                              this,
                              m_Acquisition,
                              m_DataProcessor,
                              m_Application->allocator(),
                              settings,
                              "experiment/window");

    m_DataProcessor -> setWindow(m_Window);
    m_Acquisition -> setWindow(m_Window);
  }

  splashMessage("Loading plugins");

  //  printMessage("about to load plugins");

  m_Acquisition -> setNIDAQPlugin(m_Application->nidaqPlugin());

  m_AcquisitionThread->initialize();

  if (m_Window) m_Window -> onAcquisitionInit();

  if (get_RunSpecServer()) {
    splashMessage("Starting SPEC Server");

    m_ServerThread = new QxrdServerThread(this, "qxrd", get_SpecServerPort());
    m_ServerThread -> setObjectName("server");
    m_ServerThread -> start();
    m_Server = m_ServerThread -> server();

    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("Spec Server Thread started: listening on port %1").arg(m_Server->serverPort()));
    }
  }

  if (get_RunSimpleServer()) {
    splashMessage("Starting Simple Socket Server");

    m_SimpleServerThread = new QxrdSimpleServerThread(this, "simpleserver", get_SimpleServerPort());
    m_SimpleServerThread -> setObjectName("smpsrv");
    m_SimpleServerThread -> start();
    m_SimpleServer = m_SimpleServerThread -> server();

    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("Simple Server Thread started: listening on port %1").arg(m_SimpleServer->serverPort()));
    }
  }

  m_ScriptEngineThread = new QxrdScriptEngineThread(m_Application, this);
  m_ScriptEngineThread -> setObjectName("script");
  m_ScriptEngineThread -> start();
  m_ScriptEngine = m_ScriptEngineThread -> scriptEngine();

//  m_ScriptEngineDebugger = new QScriptEngineDebugger(this);
//  m_ScriptEngineDebugger -> attachTo(m_ScriptEngine->scriptEngine());
//  m_ScriptEngineDebugger -> setAutoShowStandardWindow(true);

  if (m_Server) {
    connect(m_Server,         SIGNAL(executeCommand(QString)),           scriptEngine(),    SLOT(evaluateSpecCommand(QString)));
    connect(scriptEngine(),   SIGNAL(specResultAvailable(QScriptValue)), m_Server,          SLOT(finishedCommand(QScriptValue)));
  }

  if (m_SimpleServer) {
    connect(m_SimpleServer,   SIGNAL(executeCommand(QString)),           scriptEngine(),    SLOT(evaluateSimpleServerCommand(QString)));
    connect(scriptEngine(),   SIGNAL(simpleServerResultAvailable(QScriptValue)), m_SimpleServer,  SLOT(finishedCommand(QScriptValue)));
  }

  if (m_Window) connect(m_Window,         SIGNAL(executeCommand(QString)),           scriptEngine(),    SLOT(evaluateAppCommand(QString)));
  if (m_Window) connect(scriptEngine(),   SIGNAL(appResultAvailable(QScriptValue)),  m_Window,          SLOT(finishedCommand(QScriptValue)));

  connect(m_Application, SIGNAL(aboutToQuit()), this, SLOT(shutdownThreads()));

  splashMessage("Loading Preferences");

  readSettings(settings);

  splashMessage("Loading Background Images");

  m_DataProcessor -> loadDefaultImages();

#ifdef Q_OS_WIN32
  QDir::setCurrent(QDir::homePath());
#endif

  printMessage(tr("Current directory %1").arg(QDir::currentPath()));

  splashMessage("Opening Windows");

  if (m_Application->get_GuiWanted() && m_Window) {
    m_Window -> show();

    m_Splash -> finish(m_Window);
  }

  return true;
}

QxrdExperiment::~QxrdExperiment()
{
  printf("QxrdExperiment::~QxrdExperiment\n");

  if (qcepDebug(DEBUG_APP)) {
    m_Application->printMessage("QxrdExperiment::~QxrdExperiment");
  }

  closeScanFile();
  closeLogFile();
}

void QxrdExperiment::closeExperiment()
{
}

void QxrdExperiment::shutdownThread(QxrdThread *thread)
{
  if (thread) {
    thread->shutdown();
    delete thread;
  }
}

void QxrdExperiment::shutdown()
{
  if (qcepDebug(DEBUG_APP)) {
    m_Application->printMessage("QxrdExperiment::shutdown()");
  }

  shutdownThread(m_ScriptEngineThread);
}

void QxrdExperiment::splashMessage(const char *msg)
{
  GUI_THREAD_CHECK;

  printMessage(msg);

  if (m_Splash) {
    char msgf[256];
    snprintf(msgf, 200, "Qxrd Version " STR(QXRD_VERSION) "\n%s", msg);
    m_Splash->showMessage(msgf, Qt::AlignBottom|Qt::AlignHCenter);
    m_Application->processEvents();
  }
}

void QxrdExperiment::splashMessage(QString msg)
{
  splashMessage(qPrintable(msg));
}

void QxrdExperiment::criticalMessage(QString msg)
{
  if (m_Window) {
    INVOKE_CHECK(QMetaObject::invokeMethod(m_Window, "displayCriticalMessage", Q_ARG(QString, msg)));
  } else {
    m_Application->criticalMessage(msg);
  }
}

void QxrdExperiment::statusMessage(QString msg)
{
  if (m_Window) {
    INVOKE_CHECK(QMetaObject::invokeMethod(m_Window, "displayStatusMessage", Q_ARG(QString, msg)));
  } else {
    m_Application->statusMessage(msg);
  }
}

void QxrdExperiment::printMessage(QString msg, QDateTime ts)
{
  if (qcepDebug(DEBUG_NOMESSAGES)) {
  } else {
    QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+
        QThread::currentThread()->objectName()+": "+
        msg.trimmed();

    message = message.replace("\n", " : ");

    logMessage(message);

    if (m_Window) {
      INVOKE_CHECK(QMetaObject::invokeMethod(window(), "displayMessage", Qt::QueuedConnection, Q_ARG(QString, message)));
    } else {
      m_Application->printMessage(qPrintable(message));
    }
  }
}

void QxrdExperiment::openScanFile()
{
  if (m_ScanFile) {
    fclose(m_ScanFile);
  }

  m_ScanFile = fopen(qPrintable(scanFilePath()), "a");

  if (m_ScanFile) {
    writeScanHeader();
  }
}

void QxrdExperiment::writeScanHeader()
{
}

QxrdWindow *QxrdExperiment::window()
{
  return m_Window;
}

QxrdAcquisitionThread *QxrdExperiment::acquisitionThread()
{
  return m_AcquisitionThread;
}

QxrdAcquisition *QxrdExperiment::acquisition() const
{
  return m_Acquisition;
}

QxrdDataProcessor *QxrdExperiment::dataProcessor() const
{
  return m_DataProcessor;
}

QxrdScriptEngine* QxrdExperiment::scriptEngine()
{
  return m_ScriptEngine;
}

void QxrdExperiment::executeCommand(QString cmd)
{
  m_ScriptEngine->evaluateAppCommand(cmd);
}

void QxrdExperiment::openLogFile()
{
  if (m_LogFile) {
    fclose(m_LogFile);
  }

  m_LogFile = fopen(qPrintable(logFilePath()), "a");

  if (m_LogFile) {
    writeLogHeader();
  }
}

void QxrdExperiment::newLogFile(QString path)
{
  if (m_LogFile) {
    fclose(m_LogFile);
    m_LogFile = NULL;
  }

  set_LogFilePath(path);

  openLogFile();
}

FILE* QxrdExperiment::logFile()
{
  return m_LogFile;
}

void QxrdExperiment::writeLogHeader()
{
  if (m_LogFile) {
    fprintf(m_LogFile, "#F %s\n", qPrintable(get_LogFilePath()));
    fprintf(m_LogFile, "#E %d\n", QDateTime::currentDateTime().toTime_t());
    fprintf(m_LogFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")));
    fflush(m_LogFile);
  }
}

void QxrdExperiment::logMessage(QString msg)
{
  openLogFile();

  if (m_LogFile) {
    fprintf(m_LogFile, "#CX %s\n", qPrintable(msg));
    fflush(m_LogFile);
  }
}

void QxrdExperiment::closeLogFile()
{
  if (m_LogFile) {
    logMessage(tr("%1 ------- shutdown --------").
               arg(QDateTime::currentDateTime().toString("yyyy.MM.dd : hh:mm:ss.zzz ")));
    fclose(m_LogFile);
    m_LogFile = NULL;
  }
}

FILE* QxrdExperiment::scanFile()
{
  return m_ScanFile;
}

void QxrdExperiment::closeScanFile()
{
  if (m_ScanFile) {
    fclose(m_ScanFile);
    m_ScanFile = NULL;
  }
}

void QxrdExperiment::readSettings()
{
  QString docPath = experimentFilePath();

  if (docPath.length()>0) {
    QSettings settings(docPath, QSettings::IniFormat);

    readSettings(&settings, "experiment");
  } else {
    QxrdExperimentSettings settings;

    readSettings(&settings, "experiment");
  }
}

void QxrdExperiment::readSettings(QSettings *settings, QString section)
{
  if (settings) {
    QcepProperty::readSettings(this, &staticMetaObject, section, settings);

    if (m_Window) {
      m_Window       -> readSettings(settings, section+"/window");
    }

    if (m_Acquisition) {
      m_Acquisition  -> readSettings(settings, section+"/acquire");
    }

    if (m_DataProcessor) {
      m_DataProcessor-> readSettings(settings, section+"/processor");
    }
  }
}

void QxrdExperiment::writeSettings()
{
  QString docPath = experimentFilePath();

  if (docPath.length()>0) {
    QSettings settings(docPath, QSettings::IniFormat);

    writeSettings(&settings, "experiment");
  } else {
    QxrdExperimentSettings settings;

    writeSettings(&settings, "experiment");
  }
}

void QxrdExperiment::writeSettings(QSettings *settings, QString section)
{
  if (settings) {
    QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

    if (m_Window) {
      m_Window       -> writeSettings(settings, section+"/window");
    }

    if (m_Acquisition) {
      m_Acquisition  -> writeSettings(settings, section+"/acquire");
    }

    if (m_DataProcessor) {
      m_DataProcessor-> writeSettings(settings, section+"/processor");
    }
  }
}

QString QxrdExperiment::defaultExperimentDirectory(QString path)
{
  QFileInfo info(path);

  QString directory = info.dir().canonicalPath();

  return directory;
}

QString QxrdExperiment::defaultExperimentFileName(QString path)
{
  QFileInfo info(path);

  if (info.suffix() == "qxrdp") {
    return info.fileName();
  } else {
    return info.fileName()+".qxrdp";
  }
}

QString QxrdExperiment::defaultExperimentName(QString path)
{
  QFileInfo info(path);

  if (info.suffix() == "qxrdp") {
    return info.completeBaseName();
  } else {
    return info.fileName();
  }
}

QString QxrdExperiment::defaultLogName(QString path)
{
  return defaultExperimentName(path)+".log";
}

QString QxrdExperiment::defaultScanName(QString path)
{
  return defaultExperimentName(path)+".scans";
}

QString QxrdExperiment::experimentFilePath()
{
  QDir dir(get_ExperimentDirectory());

  return dir.filePath(get_ExperimentFileName());
}

void QxrdExperiment::setExperimentFilePath(QString path)
{
  set_ExperimentDirectory(defaultExperimentDirectory(path));
  set_ExperimentFileName(defaultExperimentFileName(path));
  set_ExperimentName(defaultExperimentName(path));
  set_LogFilePath(defaultLogName(path));
  set_ScanFilePath(defaultScanName(path));

  if (m_Window) {
    m_Window -> updateTitle();
  }

  if (qcepDebug(DEBUG_PREFS)) {
    printMessage(tr("setExperimentFilePath %1").arg(path));
    printMessage(tr("  experimentDirectory: %1").arg(get_ExperimentDirectory()));
    printMessage(tr("  experimentFileName: %1").arg(get_ExperimentFileName()));
    printMessage(tr("  experimentName: %1").arg(get_ExperimentName()));
    printMessage(tr("  logFilePath: %1").arg(get_LogFilePath()));
    printMessage(tr("  scanFilePath: %1").arg(get_ScanFilePath()));
  }

  openLogFile();
  openScanFile();
}

QString QxrdExperiment::logFilePath()
{
  QDir dir(get_ExperimentDirectory());

  return dir.filePath(get_LogFilePath());
}

QString QxrdExperiment::scanFilePath()
{
  QDir dir(get_ExperimentDirectory());

  return dir.filePath(get_ScanFilePath());
}

void QxrdExperiment::saveExperiment()
{
  writeSettings();
}

void QxrdExperiment::saveExperimentAs(QString path)
{
  QxrdExperimentSettings settings(path);

  setExperimentFilePath(path);

  writeSettings(&settings);
}

void QxrdExperiment::saveExperimentCopyAs(QString path)
{
  printf("Save experiment copy as %s\n", qPrintable(path));

  QxrdExperimentSettings settings(path);

  writeSettings(&settings);

  QxrdExperiment *exp = new QxrdExperiment(path, m_Application, &settings);

  exp -> setExperimentFilePath(path);

  exp -> writeSettings(&settings);

  delete exp;
}
