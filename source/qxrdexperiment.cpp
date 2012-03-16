#define _CRT_SECURE_NO_WARNINGS

#include "qcepmacros.h"
#include "qxrdexperiment.h"
#include "qxrdapplication.h"
#include "qxrdsplashscreen.h"
#include "qxrddataprocessorthread.h"
#include "qxrddataprocessor.h"
#include "qxrdwindow.h"
#include "qxrdacquisitionthread.h"
#include "qxrdacquisition.h"
#include "qxrdserverthread.h"
#include "qxrdserver.h"
#include "qxrdsimpleserverthread.h"
#include "qxrdsimpleserver.h"
#include "qxrdscriptenginethread.h"
#include "qxrdscriptengine.h"
#include "qxrdexperimentpreferencesdialog.h"
#include "qxrdexperimentsettings.h"
#include <QFileDialog>
#include "qxrdfilesaverthread.h"
#include "qxrdsettingssaver.h"
#include "qxrdfilesaver.h"

QxrdExperiment::QxrdExperiment(
    QString path,
    QxrdApplication *app,
    QSettings *settings) :
  QObject(NULL),
  m_Application(app),
  m_ExperimentThread(),
  m_SettingsSaver(QxrdSettingsSaverPtr(
                    new QxrdSettingsSaver(this))),
  m_ExperimentKind(m_SettingsSaver, this, "experimentKind", -1, "Kind of Experiment"),
  m_ExperimentDirectory(m_SettingsSaver, this, "experimentDirectory", defaultExperimentDirectory(path), "Experiment Directory"),
  m_ExperimentFileName(m_SettingsSaver, this, "experimentFileName", defaultExperimentFileName(path), "Experiment File"),
  m_ExperimentName(m_SettingsSaver, this, "experimentName", defaultExperimentName(path), "Experiment Name"),
  m_ExperimentDescription(m_SettingsSaver, this, "experimentDescription", "", "Experiment Description"),
  m_LogFileName(m_SettingsSaver, this, "logFileName", defaultLogName(path), "Log File Name"),
  m_ScanFileName(m_SettingsSaver, this, "scanFileName", defaultScanName(path), "Scan File Name"),
  m_DetectorType(m_SettingsSaver, this,"detectorType", 1, "Detector Type"),
  m_ProcessorType(m_SettingsSaver, this,"processorType", 0, "Data Processor Type"),
  m_DefaultLayout(QxrdSettingsSaverPtr(), this,"defaultLayout",0, "Default Layout Used?"),
  m_RunSpecServer(m_SettingsSaver, this,"runSpecServer", 1, "Run SPEC Server?"),
  m_SpecServerPort(m_SettingsSaver, this,"specServerPort", -1, "Port for SPEC Server"),
  m_RunSimpleServer(m_SettingsSaver, this,"runSimpleServer", 1, "Run Simple Socket Server?"),
  m_SimpleServerPort(m_SettingsSaver, this,"simpleServerPort", 1234, "Port for Simple Socket Server"),
  m_WorkCompleted(QxrdSettingsSaverPtr(), this, "workCompleted", 0, "Amount of Work Completed"),
  m_WorkTarget(QxrdSettingsSaverPtr(), this, "workTarget", 0, "Amount of Work Targetted"),
  m_CompletionPercentage(QxrdSettingsSaverPtr(), this, "completionPercentage", 0, "Percentage of Work Completed"),
  m_Window(),
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
  m_FileSaver(NULL),
  m_ScriptEngineThread(NULL),
  m_ScriptEngine(NULL),
  m_ScriptEngineDebugger(NULL),
  m_LogFile(NULL),
  m_ScanFile(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperiment::QxrdExperiment(%p)\n", this);
  }

  readSettings(settings);
}

QxrdExperimentThreadWPtr QxrdExperiment::experimentThread()
{
  return m_ExperimentThread;
}

QxrdSettingsSaverPtr QxrdExperiment::settingsSaver()
{
  return m_SettingsSaver;
}

bool QxrdExperiment::init(QxrdExperimentThreadWPtr expthrd, QxrdExperimentWPtr exp, QSettings *settings)
{
  GUI_THREAD_CHECK;

  m_ExperimentThread = expthrd;

  if (m_Application && m_Application->get_GuiWanted()) {
    m_Splash = new QxrdSplashScreen(NULL);
    m_Splash -> show();
  }

  setObjectName("QxrdExperiment");

  QThread::currentThread()->setObjectName("doc");

  splashMessage("Initializing File Saver");

  if (m_Application) {
    m_FileSaverThread = QxrdFileSaverThreadPtr(
          new QxrdFileSaverThread(m_Application->allocator()));
    m_FileSaverThread -> setObjectName("saver");
    m_FileSaverThread -> start();
    m_FileSaver = m_FileSaverThread -> fileSaver();

    splashMessage("Initializing Data Processing");

    m_DataProcessorThread = QxrdDataProcessorThreadPtr(
          new QxrdDataProcessorThread(m_SettingsSaver,
                                      exp,
                                      QxrdAcquisitionPtr(),
                                      m_Application->allocator(),
                                      m_FileSaver));
    m_DataProcessorThread -> setObjectName("proc");
    m_DataProcessorThread -> start();
    m_DataProcessor = m_DataProcessorThread -> dataProcessor();

    m_FileSaver -> setProcessor(m_DataProcessor);

    splashMessage("Initializing Data Acquisition");

    m_AcquisitionThread = QxrdAcquisitionThreadPtr(
          new QxrdAcquisitionThread(m_SettingsSaver,
                                    exp,
                                    m_DataProcessor,
                                    m_Application->allocator(),
                                    get_DetectorType(),
                                    settings,
                                    "experiment/acquire"));
    m_AcquisitionThread -> setObjectName("acqu");
    m_AcquisitionThread -> start();
    m_Acquisition = m_AcquisitionThread -> acquisition();

    m_DataProcessor -> setAcquisition(m_Acquisition);
    m_FileSaver -> setAcquisition(m_Acquisition);

    m_Acquisition -> setNIDAQPlugin(m_Application->nidaqPlugin());

    if (m_Application && m_Application->get_GuiWanted()) {
      splashMessage("Opening Main Window");
      m_Window =
            new QxrdWindow(m_SettingsSaver,
                           m_Application,
                           exp,
                           m_Acquisition,
                           m_DataProcessor,
                           m_Application->allocator());

      QxrdWindow *win = m_Window;

      if (win) {
        win -> init();

        m_DataProcessor -> setWindow(win);
        m_Acquisition -> setWindow(win);
      }
    }

    QxrdWindow *win = m_Window;

    if (win) {
      win -> onAcquisitionInit();
    }

    if (get_RunSpecServer()) {
      splashMessage("Starting SPEC Server");

#ifdef Q_OS_WIN
      // On windows, there are problems with servers not in the main thread...

      m_Server = QxrdServerPtr(
            new QxrdServer(this, "qxrd", get_SpecServerPort()));

      m_Server->startServer(QHostAddress::Any, get_SpecServerPort());
#else
      m_ServerThread = QxrdServerThreadPtr(
            new QxrdServerThread(this, "qxrd", get_SpecServerPort()));
      m_ServerThread -> setObjectName("server");
      m_ServerThread -> start();
      m_Server = m_ServerThread -> server();
#endif

      if (qcepDebug(DEBUG_SERVER)) {
        printMessage(tr("Spec Server Thread started: listening on port %1").arg(m_Server->serverPort()));
      }
    }

    if (get_RunSimpleServer()) {
      splashMessage("Starting Simple Socket Server");

#ifdef Q_OS_WIN
      m_SimpleServer = QxrdSimpleServerPtr(
            new QxrdSimpleServer(this, "simpleserver", get_SimpleServerPort()));

      m_SimpleServer->startServer(QHostAddress::Any, get_SimpleServerPort());
#else
      m_SimpleServerThread = QxrdSimpleServerThreadPtr(
            new QxrdSimpleServerThread(this, "simpleserver", get_SimpleServerPort()));
      m_SimpleServerThread -> setObjectName("smpsrv");
      m_SimpleServerThread -> start();
      m_SimpleServer = m_SimpleServerThread -> server();
#endif

      if (qcepDebug(DEBUG_SERVER)) {
        printMessage(tr("Simple Server Thread started: listening on port %1").arg(m_SimpleServer->serverPort()));
      }
    }

    m_ScriptEngineThread = QxrdScriptEngineThreadPtr(
          new QxrdScriptEngineThread(m_Application, this));
    m_ScriptEngineThread -> setObjectName("script");
    m_ScriptEngineThread -> start();
    m_ScriptEngine = m_ScriptEngineThread -> scriptEngine();

    //  m_ScriptEngineDebugger = new QScriptEngineDebugger(this);
    //  m_ScriptEngineDebugger -> attachTo(m_ScriptEngine->scriptEngine());
    //  m_ScriptEngineDebugger -> setAutoShowStandardWindow(true);

    if (m_Server) {
      connect(m_Server.data(),         SIGNAL(executeCommand(QString)),
              scriptEngine().data(),   SLOT(evaluateSpecCommand(QString)));

      connect(scriptEngine().data(),   SIGNAL(specResultAvailable(QScriptValue)),
              m_Server.data(),         SLOT(finishedCommand(QScriptValue)));
    }

    if (m_SimpleServer) {
      connect(m_SimpleServer.data(),   SIGNAL(executeCommand(QString)),
              scriptEngine().data(),   SLOT(evaluateSimpleServerCommand(QString)));

      connect(scriptEngine().data(),   SIGNAL(simpleServerResultAvailable(QScriptValue)),
              m_SimpleServer.data(),   SLOT(finishedCommand(QScriptValue)));
    }

    if (win) {
      connect(win,                     SIGNAL(executeCommand(QString)),
              scriptEngine().data(),   SLOT(evaluateAppCommand(QString)));

      connect(scriptEngine().data(),   SIGNAL(appResultAvailable(QScriptValue)),
              win,                     SLOT(finishedCommand(QScriptValue)));
    }

    connect(prop_WorkCompleted(), SIGNAL(valueChanged(int,int)), this, SLOT(updateCompletionPercentage(int,int)));
    connect(prop_WorkTarget(),    SIGNAL(valueChanged(int,int)), this, SLOT(updateCompletionPercentage(int,int)));

    splashMessage("Loading Preferences");

    readSettings(settings);

    splashMessage("Loading Background Images");

    m_DataProcessor -> loadDefaultImages();

#ifdef Q_OS_WIN32
    QDir::setCurrent(QDir::homePath());
#endif

    printMessage(tr("Current directory %1").arg(QDir::currentPath()));

    splashMessage("Opening Windows");

    if (win && m_Application && m_Application->get_GuiWanted()) {
      win -> show();

      m_Splash -> finish(win);
    }
  }

  return true;
}

QxrdExperiment::~QxrdExperiment()
{
  m_SettingsSaver->performSave();

  if (m_Application && qcepDebug(DEBUG_APP)) {
    m_Application->printMessage("QxrdExperiment::~QxrdExperiment");
  }

  closeScanFile();
  closeLogFile();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperiment::~QxrdExperiment(%p)\n", this);
  }
}

void QxrdExperiment::splashMessage(QString msg)
{
  GUI_THREAD_CHECK;

  printMessage(msg);

  if (m_Application && m_Splash) {
    QString msgf = tr("Qxrd Version " STR(QXRD_VERSION) "\n")+msg;

    m_Splash->showMessage(msgf, Qt::AlignBottom|Qt::AlignHCenter);
    m_Application->processEvents();
  }
}

void QxrdExperiment::criticalMessage(QString msg)
{
  QxrdWindow *win = m_Window;

  if (win) {
    win->displayCriticalMessage(msg);
  } else if (m_Application) {
    m_Application->criticalMessage(msg);
  } else {
    printf("%s\n", qPrintable(msg));
  }
}

void QxrdExperiment::statusMessage(QString msg)
{
  QxrdWindow *win = m_Window;

  if (win) {
    win->displayStatusMessage(msg);
  } else if (m_Application) {
    m_Application->statusMessage(msg);
  } else {
    printf("%s\n", qPrintable(msg));
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

    QxrdWindow *win = m_Window;

    if (win) {
      win->displayMessage(message);
//    } else if (m_Application) {
//      m_Application->printMessage(message);
    } else {
      printf("%s\n", qPrintable(message));
    }
  }
}

QxrdWindow *QxrdExperiment::window()
{
  return m_Window;
}

QxrdAcquisitionThreadPtr QxrdExperiment::acquisitionThread()
{
  return m_AcquisitionThread;
}

QxrdAcquisitionPtr QxrdExperiment::acquisition() const
{
  return m_Acquisition;
}

QxrdDataProcessorWPtr QxrdExperiment::dataProcessor() const
{
  return m_DataProcessor;
}

QxrdScriptEnginePtr QxrdExperiment::scriptEngine()
{
  return m_ScriptEngine;
}

void QxrdExperiment::executeCommand(QString cmd)
{
  m_ScriptEngine->evaluateAppCommand(cmd);
}

void QxrdExperiment::newLogFile(QString path)
{
  {
    QMutexLocker lock(&m_LogFileMutex);

    if (m_LogFile) {
      fclose(m_LogFile);
      m_LogFile = NULL;
    }
  }

  set_LogFileName(path);

  openLogFile();
}

void QxrdExperiment::openLogFile()
{
  QMutexLocker lock(&m_LogFileMutex);

  if (m_LogFile == NULL) {
    m_LogFile = fopen(qPrintable(logFilePath()), "a");

    if (m_LogFile) {
      fprintf(m_LogFile, "#F %s\n", qPrintable(get_LogFileName()));
      fprintf(m_LogFile, "#E %d\n", QDateTime::currentDateTime().toTime_t());
      fprintf(m_LogFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")));
      fflush(m_LogFile);
    }
  }
}

FILE* QxrdExperiment::logFile()
{
  QMutexLocker lock(&m_LogFileMutex);

  return m_LogFile;
}

void QxrdExperiment::logMessage(QString msg)
{
  openLogFile();

  QMutexLocker lock(&m_LogFileMutex);

  if (m_LogFile) {
    fprintf(m_LogFile, "#CX %s\n", qPrintable(msg));
    fflush(m_LogFile);
  }
}

void QxrdExperiment::closeLogFile()
{
  QMutexLocker lock(&m_LogFileMutex);

  if (m_LogFile) {
    fprintf(m_LogFile, "#CX %s ------- shutdown --------\n",
            qPrintable(QDateTime::currentDateTime().toString("yyyy.MM.dd : hh:mm:ss.zzz ")));

    fclose(m_LogFile);
    m_LogFile = NULL;
  }
}

void QxrdExperiment::newScanFile(QString path)
{
  {
    QMutexLocker lock(&m_ScanFileMutex);

    if (m_ScanFile) {
      fclose(m_ScanFile);
      m_ScanFile = NULL;
    }
  }

  set_ScanFileName(path);

  openScanFile();
}

void QxrdExperiment::openScanFile()
{
  QMutexLocker lock(&m_ScanFileMutex);

  if (m_ScanFile == NULL) {
    m_ScanFile = fopen(qPrintable(scanFilePath()), "a");
  }
}

FILE* QxrdExperiment::scanFile()
{
  return m_ScanFile;
}

void QxrdExperiment::closeScanFile()
{
  QMutexLocker lock(&m_ScanFileMutex);

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

    QxrdWindow *win = m_Window;

    if (win) {
      win            -> readSettings(settings, section+"/window");
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
    QFile::remove(docPath+".new");

    {
      QSettings settings(docPath+".new", QSettings::IniFormat);

      writeSettings(&settings, "experiment");
    }

    QFile::remove(docPath+".bak");
    QFile::rename(docPath, docPath+".bak");
    QFile::rename(docPath+".new", docPath);
  } else {
    QxrdExperimentSettings settings;

    writeSettings(&settings, "experiment");
  }
}

void QxrdExperiment::writeSettings(QSettings *settings, QString section)
{
  if (settings) {
    QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

    QxrdWindow *win = m_Window;

    if (win) {
      win            -> writeSettings(settings, section+"/window");
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
  set_LogFileName(defaultLogName(path));
  set_ScanFileName(defaultScanName(path));

  QxrdWindow *win = m_Window;

  if (win) {
    win -> updateTitle();
  }

  if (qcepDebug(DEBUG_PREFS)) {
    printMessage(tr("setExperimentFilePath %1").arg(path));
    printMessage(tr("  experimentDirectory: %1").arg(get_ExperimentDirectory()));
    printMessage(tr("  experimentFileName: %1").arg(get_ExperimentFileName()));
    printMessage(tr("  experimentName: %1").arg(get_ExperimentName()));
    printMessage(tr("  logFilePath: %1").arg(get_LogFileName()));
    printMessage(tr("  scanFilePath: %1").arg(get_ScanFileName()));
  }

  newLogFile(get_LogFileName());
  newScanFile(get_ScanFileName());
}

QString QxrdExperiment::logFilePath()
{
  QDir dir(get_ExperimentDirectory());

  return dir.filePath(get_LogFileName());
}

QString QxrdExperiment::scanFilePath()
{
  QDir dir(get_ExperimentDirectory());

  return dir.filePath(get_ScanFileName());
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

void QxrdExperiment::updateCompletionPercentage(int, int)
{
  int completed = get_WorkCompleted();
  int target    = get_WorkTarget();

  if (target > 0) {
    set_CompletionPercentage(100.0*completed/target);
  } else {
    set_CompletionPercentage(0);
  }
}

void QxrdExperiment::completeWork(int amt)
{
  prop_WorkCompleted()->incValue(amt);
}

void QxrdExperiment::commenceWork(int amt)
{
  prop_WorkTarget()->incValue(amt);
}

void QxrdExperiment::finishedWork(int amt)
{
  prop_WorkCompleted()->incValue(-amt);
  prop_WorkTarget()->incValue(-amt);
}
