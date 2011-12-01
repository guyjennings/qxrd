#include "qxrddocument.h"
#include "qxrdfreshstartdialog.h"
#include "qxrdapplication.h"
#include "qxrddataprocessor.h"
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdserver.h"
#include "qxrdsimpleserver.h"
#include "qxrdscriptengine.h"

QxrdDocument::QxrdDocument(QString path,
                           QxrdApplication *app,
                           QObject *parent) :
  QObject(parent),
  m_DocumentFilePath(this, "documentFilePath", path),
  m_LogFilePath(this, "logFilePath", "qxrd.log"),
  m_ScanFilePath(this, "scanFilePath", "qxrd.log"),
  m_DetectorType(this,"detectorType", 1),
  m_ProcessorType(this,"processorType", 0),
  m_DefaultLayout(this,"defaultLayout",0),
  m_RunSpecServer(this,"specServer", 1),
  m_SpecServerPort(this,"specServerPort", -1),
  m_RunSimpleServer(this,"simpleServer", 1),
  m_SimpleServerPort(this,"simpleServerPort", 1234),
  m_Splash(NULL),
  m_Application(app),
  m_Window(NULL),
  m_ServerThread(NULL),
  m_Server(NULL),
  m_SimpleServerThread(NULL),
  m_SimpleServer(NULL),
  m_AcquisitionThread(NULL),
  m_FileSaverThread(NULL),
  m_LogFile(NULL),
  m_ScanFile(NULL)
{
}

bool QxrdDocument::init(QSplashScreen *splash)
{
  m_Splash = splash;

  setObjectName("qxrddocument");

  QThread::currentThread()->setObjectName("doc");
//  printf("application thread %p\n", thread());

//  int detectorType = 0;

//  int specServer = 0;
//  int specServerPort = 0;
//  int simpleServer = 0;
//  int simpleServerPort = 0;

//  QString currentExperiment = get_CurrentExperiment();

//  if (currentExperiment.length()>0) {
//    QSettings settings(currentExperiment, QSettings::IniFormat);

//    detectorType = settings.value("application/detectorType").toInt();
//    gCEPDebug = settings.value("application/debug").toInt();
//    specServer = settings.value("application/runSpecServer").toInt();
//    specServerPort = settings.value("application/specServerPort").toInt();
//    simpleServer = settings.value("application/runSimpleServer").toInt();
//    simpleServerPort = settings.value("application/simpleServerPort").toInt();
//  } else {
//    QxrdSettings settings;

//    detectorType = settings.value("application/detectorType").toInt();
//    gCEPDebug = settings.value("application/debug").toInt();
//    specServer = settings.value("application/runSpecServer").toInt();
//    specServerPort = settings.value("application/specServerPort").toInt();
//    simpleServer = settings.value("application/runSimpleServer").toInt();
//    simpleServerPort = settings.value("application/simpleServerPort").toInt();
//  }

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nInitializing File Saver");

  m_FileSaverThread = new QxrdFileSaverThread(g_Application->allocator());
  m_FileSaverThread -> setObjectName("saver");
  m_FileSaverThread -> start();

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nInitializing Data Processing");

  m_DataProcessorThread = new QxrdDataProcessorThread(this, NULL, g_Application->allocator(), m_FileSaverThread);
  m_DataProcessorThread -> setObjectName("proc");
  m_DataProcessorThread -> start();
  m_DataProcessor = m_DataProcessorThread -> dataProcessor();

  m_FileSaverThread -> setProcessor(m_DataProcessor);

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nInitializing Data Acquisition");

  m_AcquisitionThread = new QxrdAcquisitionThread(this, m_DataProcessor, g_Application->allocator(), get_DetectorType());
  m_AcquisitionThread -> setObjectName("acqu");
  m_AcquisitionThread -> start();
  m_Acquisition = m_AcquisitionThread -> acquisition();

  m_DataProcessor -> setAcquisition(m_Acquisition);
  m_FileSaverThread -> setAcquisition(m_Acquisition);


//  if (g_Application->get_GuiWanted()) {
    splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nOpening Main Window");
    m_Window = new QxrdWindow(g_Application, this, m_Acquisition, m_DataProcessor, g_Application->allocator());

    m_DataProcessor -> setWindow(m_Window);
    m_Acquisition -> setWindow(m_Window);
//  }

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nLoading plugins");

//  printMessage("about to load plugins");

  m_Acquisition -> setNIDAQPlugin(g_Application->nidaqPlugin());

  m_AcquisitionThread->initialize();

  if (m_Window) m_Window -> onAcquisitionInit();

  if (get_RunSpecServer()) {
    splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nStarting SPEC Server");

    m_ServerThread = new QxrdServerThread("qxrd", get_SpecServerPort());
    m_ServerThread -> setObjectName("server");
    m_ServerThread -> start();
    m_Server = m_ServerThread -> server();

    if (qcepDebug(DEBUG_SERVER)) {
      g_Application->printMessage(tr("Spec Server Thread started: listening on port %1").arg(m_Server->serverPort()));
    }
  }

  if (get_RunSimpleServer()) {
    splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nStarting Simple Socket Server");

    m_SimpleServerThread = new QxrdSimpleServerThread("simpleserver", get_SimpleServerPort());
    m_SimpleServerThread -> setObjectName("smpsrv");
    m_SimpleServerThread -> start();
    m_SimpleServer = m_SimpleServerThread -> server();

    if (qcepDebug(DEBUG_SERVER)) {
      g_Application->printMessage(tr("Simple Server Thread started: listening on port %1").arg(m_SimpleServer->serverPort()));
    }
  }

  if (m_Server) {
    connect(m_Server,         SIGNAL(executeCommand(QString)),           g_Application->scriptEngine(),    SLOT(evaluateSpecCommand(QString)));
    connect(g_Application->scriptEngine(),   SIGNAL(specResultAvailable(QScriptValue)), m_Server,          SLOT(finishedCommand(QScriptValue)));
  }

  if (m_SimpleServer) {
    connect(m_SimpleServer,   SIGNAL(executeCommand(QString)),           g_Application->scriptEngine(),    SLOT(evaluateSimpleServerCommand(QString)));
    connect(g_Application->scriptEngine(),   SIGNAL(simpleServerResultAvailable(QScriptValue)), m_SimpleServer,  SLOT(finishedCommand(QScriptValue)));
  }

  if (m_Window) connect(m_Window,         SIGNAL(executeCommand(QString)),           g_Application->scriptEngine(),    SLOT(evaluateAppCommand(QString)));
  if (m_Window) connect(g_Application->scriptEngine(),   SIGNAL(appResultAvailable(QScriptValue)),  m_Window,          SLOT(finishedCommand(QScriptValue)));

  if (m_Window) m_Window -> setScriptEngine(g_Application->scriptEngine());

  connect(this, SIGNAL(aboutToQuit()), this, SLOT(shutdownThreads()));

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nLoading Preferences");

  readSettings();

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nLoading Background Images");

  m_DataProcessor -> loadDefaultImages();

#ifdef Q_OS_WIN32
  QDir::setCurrent(QDir::homePath());
#endif

  g_Application->printMessage(tr("Current directory %1").arg(QDir::currentPath()));

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nOpening Windows");

  if (g_Application->get_GuiWanted() && m_Window) {
    m_Window -> show();
  }

  return true;
}

QxrdDocument::~QxrdDocument()
{
  closeScanFile();
  closeLogFile();
}

void QxrdDocument::splashMessage(const char *msg)
{
  g_Application->printMessage(msg);

  if (m_Splash) {
    m_Splash->showMessage(msg, Qt::AlignBottom|Qt::AlignHCenter);
    g_Application->processEvents();
  }
}

void QxrdDocument::splashMessage(QString msg)
{
  splashMessage(qPrintable(msg));
}

void QxrdDocument::openLogFile()
{
  if (m_LogFile == NULL) {
    m_LogFile = fopen(qPrintable(get_LogFilePath()), "a");

    if (m_LogFile) {
      writeLogHeader();
    }
  }
}

QxrdWindow *QxrdDocument::window()
{
  return m_Window;
}

QxrdAcquisitionThread *QxrdDocument::acquisitionThread()
{
  return m_AcquisitionThread;
}

QxrdAcquisition *QxrdDocument::acquisition() const
{
  return m_Acquisition;
}

QxrdDataProcessor *QxrdDocument::dataProcessor() const
{
  return m_DataProcessor;
}

void QxrdDocument::newLogFile(QString path)
{
  if (m_LogFile) {
    fclose(m_LogFile);
    m_LogFile = NULL;
  }

  set_LogFilePath(path);

  openLogFile();
}

FILE* QxrdDocument::logFile()
{
  return m_LogFile;
}

void QxrdDocument::writeLogHeader()
{
  if (m_LogFile) {
    fprintf(m_LogFile, "#F %s\n", qPrintable(get_LogFilePath()));
    fprintf(m_LogFile, "#E %d\n", QDateTime::currentDateTime().toTime_t());
    fprintf(m_LogFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")));
    fflush(m_LogFile);
  }
}

void QxrdDocument::logMessage(QString msg)
{
  openLogFile();

  if (m_LogFile) {
    fprintf(m_LogFile, "#CX %s\n", qPrintable(msg));
    fflush(m_LogFile);
  }
}

void QxrdDocument::closeLogFile()
{
  if (m_LogFile) {
    logMessage(tr("%1 ------- shutdown --------").
               arg(QDateTime::currentDateTime().toString("yyyy.MM.dd : hh:mm:ss.zzz ")));
    fclose(m_LogFile);
    m_LogFile = NULL;
  }
}

FILE* QxrdDocument::scanFile()
{
  return m_ScanFile;
}

void QxrdDocument::closeScanFile()
{
  if (m_ScanFile) {
    fclose(m_ScanFile);
    m_ScanFile = NULL;
  }
}

void QxrdDocument::readSettings()
{
  QString docPath = get_DocumentFilePath();

  if (docPath.length()>0) {
    QSettings settings(docPath, QSettings::IniFormat);

    readSettings(settings);
  } else {
    QxrdSettings settings;

    readSettings(settings);
  }
}

void QxrdDocument::readSettings(QSettings &settings)
{
  QcepProperty::readSettings(this, &staticMetaObject, "document", settings);

  m_Window       -> readSettings(settings, "window");
  m_Acquisition  -> readSettings(settings, "acquire");
  m_DataProcessor-> readSettings(settings, "processor");
}

void QxrdDocument::writeSettings()
{
  QString docPath = get_DocumentFilePath();

  if (docPath.length()>0) {
    QSettings settings(docPath, QSettings::IniFormat);

    writeSettings(settings);
  } else {
    QxrdSettings settings;

    writeSettings(settings);
  }
}

void QxrdDocument::writeSettings(QSettings &settings)
{
  m_Window       -> writeSettings(settings, "window");
  m_Acquisition  -> writeSettings(settings, "acquire");
  m_DataProcessor-> writeSettings(settings, "processor");

  QcepProperty::writeSettings(this, &staticMetaObject, "document", settings);
}

