#include "qxrddocument.h"

QxrdDocument::QxrdDocument(QObject *parent) :
  QObject(parent),
  m_DocumentFilePath(this, "documentFilePath", "qxrd.log"),
  m_LogFilePath(this, "logFilePath", "qxrd.log"),
  m_ScanFilePath(this, "scanFilePath", "qxrd.log"),
  m_DetectorType(this,"detectorType", 1),
  m_ProcessorType(this,"processorType", 0),
  m_DefaultLayout(this,"defaultLayout",0),
  m_RunSpecServer(this,"specServer", 1),
  m_SpecServerPort(this,"specServerPort", -1),
  m_RunSimpleServer(this,"simpleServer", 1),
  m_SimpleServerPort(this,"simpleServerPort", 1234),
  m_FreshStart(false),
  m_Splash(NULL),
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
  if (m_FreshStart) {
    QxrdFreshStartDialog *fresh = new QxrdFreshStartDialog();

    if (fresh->exec() == QDialog::Rejected) {
      quit();
      return false;
    }
  }

  m_Splash = splash;

  QcepProperty::registerMetaTypes();

  setupTiffHandlers();

  setObjectName("qxrdapplication");

  QThread::currentThread()->setObjectName("app");
//  printf("application thread %p\n", thread());

  g_Application = this;

  int detectorType = 0;

  int specServer = 0;
  int specServerPort = 0;
  int simpleServer = 0;
  int simpleServerPort = 0;

  QString currentExperiment = get_CurrentExperiment();

  if (currentExperiment.length()>0) {
    QSettings settings(currentExperiment, QSettings::IniFormat);

    detectorType = settings.value("application/detectorType").toInt();
    gCEPDebug = settings.value("application/debug").toInt();
    specServer = settings.value("application/runSpecServer").toInt();
    specServerPort = settings.value("application/specServerPort").toInt();
    simpleServer = settings.value("application/runSimpleServer").toInt();
    simpleServerPort = settings.value("application/simpleServerPort").toInt();
  } else {
    QxrdSettings settings;

    detectorType = settings.value("application/detectorType").toInt();
    gCEPDebug = settings.value("application/debug").toInt();
    specServer = settings.value("application/runSpecServer").toInt();
    specServerPort = settings.value("application/specServerPort").toInt();
    simpleServer = settings.value("application/runSimpleServer").toInt();
    simpleServerPort = settings.value("application/simpleServerPort").toInt();
  }

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nInitializing File Saver");

  m_FileSaverThread = new QxrdFileSaverThread(m_Allocator);
  m_FileSaverThread -> setObjectName("saver");
  m_FileSaverThread -> start();

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nInitializing Data Processing");

  m_DataProcessorThread = new QxrdDataProcessorThread(NULL, m_Allocator, m_FileSaverThread);
  m_DataProcessorThread -> setObjectName("proc");
  m_DataProcessorThread -> start();
  m_DataProcessor = m_DataProcessorThread -> dataProcessor();

  m_FileSaverThread -> setProcessor(m_DataProcessor);

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nInitializing Data Acquisition");

  m_AcquisitionThread = new QxrdAcquisitionThread(m_DataProcessor, m_Allocator, detectorType);
  m_AcquisitionThread -> setObjectName("acqu");
  m_AcquisitionThread -> start();
  m_Acquisition = m_AcquisitionThread -> acquisition();

  m_DataProcessor -> setAcquisition(m_Acquisition);
  m_FileSaverThread -> setAcquisition(m_Acquisition);


//  if (get_GuiWanted()) {
    splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nOpening Main Window");
    m_Window = new QxrdWindow(this, m_Acquisition, m_DataProcessor, m_Allocator);

    m_DataProcessor -> setWindow(m_Window);
    m_Acquisition -> setWindow(m_Window);
//  }

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nLoading plugins");

//  printMessage("about to load plugins");

  loadPlugins();

  m_Acquisition -> setNIDAQPlugin(nidaqPlugin());

  m_AcquisitionThread->initialize();

  if (m_Window) m_Window -> onAcquisitionInit();

  if (specServer) {
    splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nStarting SPEC Server");

    m_ServerThread = new QxrdServerThread("qxrd", specServerPort);
    m_ServerThread -> setObjectName("server");
    m_ServerThread -> start();
    m_Server = m_ServerThread -> server();

    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("Spec Server Thread started: listening on port %1").arg(m_Server->serverPort()));
    }
  }

  if (simpleServer) {
    splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nStarting Simple Socket Server");

    m_SimpleServerThread = new QxrdSimpleServerThread("simpleserver", simpleServerPort);
    m_SimpleServerThread -> setObjectName("smpsrv");
    m_SimpleServerThread -> start();
    m_SimpleServer = m_SimpleServerThread -> server();

    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("Simple Server Thread started: listening on port %1").arg(m_SimpleServer->serverPort()));
    }
  }


  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nStarting Scripting System");

  m_ScriptEngineThread = new QxrdScriptEngineThread(this, m_Window, m_Acquisition, m_DataProcessor);
  m_ScriptEngineThread -> setObjectName("script");
  m_ScriptEngineThread -> start();
  m_ScriptEngine = m_ScriptEngineThread -> scriptEngine();

//  m_ScriptEngineDebugger = new QScriptEngineDebugger(this);
//  m_ScriptEngineDebugger -> attachTo(m_ScriptEngine->scriptEngine());
//  m_ScriptEngineDebugger -> setAutoShowStandardWindow(true);

  if (m_Server) {
    connect(m_Server,         SIGNAL(executeCommand(QString)),           m_ScriptEngine,    SLOT(evaluateSpecCommand(QString)));
    connect(m_ScriptEngine,   SIGNAL(specResultAvailable(QScriptValue)), m_Server,          SLOT(finishedCommand(QScriptValue)));
  }

  if (m_SimpleServer) {
    connect(m_SimpleServer,   SIGNAL(executeCommand(QString)),           m_ScriptEngine,    SLOT(evaluateSimpleServerCommand(QString)));
    connect(m_ScriptEngine,   SIGNAL(simpleServerResultAvailable(QScriptValue)), m_SimpleServer,  SLOT(finishedCommand(QScriptValue)));
  }

  if (m_Window) connect(m_Window,         SIGNAL(executeCommand(QString)),           m_ScriptEngine,    SLOT(evaluateAppCommand(QString)));
  if (m_Window) connect(m_ScriptEngine,   SIGNAL(appResultAvailable(QScriptValue)),  m_Window,          SLOT(finishedCommand(QScriptValue)));

  if (m_Window) m_Window -> setScriptEngine(m_ScriptEngine);

  connect(this, SIGNAL(aboutToQuit()), this, SLOT(shutdownThreads()));

  connect(prop_Debug(), SIGNAL(valueChanged(int,int)), this, SLOT(debugChanged(int)));

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nLoading Preferences");

  readSettings();

  m_SettingsSaverThread = new QxrdSettingsSaverThread(this);
  m_SettingsSaverThread -> setObjectName("settings");
  m_SettingsSaverThread -> start();
  m_SettingsSaver = m_SettingsSaverThread -> settingsSaver();

  g_Application->printMessage(tr("Optimal thread count = %1").arg(QThread::idealThreadCount()));

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nLoading Background Images");

  m_DataProcessor -> loadDefaultImages();

#ifdef Q_OS_WIN32
  QDir::setCurrent(QDir::homePath());
#endif

  g_Application->printMessage(tr("Current directory %1").arg(QDir::currentPath()));

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nOpening Windows");

  if (get_GuiWanted() && m_Window) {
    m_Window -> show();
  }

  m_ResponseTimer = new QxrdResponseTimer(1000, this);

  return true;
}

void QxrdDocument::splashMessage(const char *msg)
{
  printMessage(msg);

  if (m_Splash) {
    m_Splash->showMessage(msg, Qt::AlignBottom|Qt::AlignHCenter);
    processEvents();
  }
}

void QxrdDocumeny::splashMessage(QString msg)
{
  splashMessage(qPrintable(msg));
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

