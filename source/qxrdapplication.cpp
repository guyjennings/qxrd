#include "qcepdebug.h"
#include "qxrdapplication.h"
#include "qxrdwindow.h"
#include "qxrdserverthread.h"
#include "qxrdserver.h"
#include "qxrdsimpleserverthread.h"
#include "qxrdsimpleserver.h"
#include "qxrddataprocessorthread.h"
#include "qxrddataprocessor.h"
#include "qxrdintegrator.h"
#include "qxrdacquisitionthread.h"
#include "qxrdacquisition.h"
#include "qxrdallocatorthread.h"
#include "qxrdallocator.h"
#include "qxrdfilesaverthread.h"
#include "qxrdfilesaver.h"
#include "qxrdscriptenginethread.h"
#include "qxrdscriptengine.h"
#include "qxrdpreferencesdialog.h"
#include "qcepproperty.h"
#include "qxrdsettingssaverthread.h"
#include "qxrddetectorplugininterface.h"
#include "qxrdprocessorinterface.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdfreshstartdialog.h"

#ifdef HAVE_PERKIN_ELMER
#include "qxrdperkinelmerplugininterface.h"
#endif

#include <QTime>
#include <QtConcurrentRun>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <tiffio.h>
#include <QPluginLoader>
#include <QSplashScreen>

int gCEPDebug = 0;
QxrdApplication *g_Application = 0;

QxrdApplication::QxrdApplication(int &argc, char **argv)
  : QApplication(argc, argv),
    m_DetectorType(this,"detectorType", 1),
    m_ProcessorType(this,"processorType", 0),
    m_Debug(this,"debug", 0),
    m_RunSpecServer(this,"specServer", 1),
    m_SpecServerPort(this,"specServerPort", -1),
    m_RunSimpleServer(this,"simpleServer", 1),
    m_SimpleServerPort(this,"simpleServerPort", 1234),
    m_DefaultLayout(this,"defaultLayout",0),
    m_LogFilePath(this, "logFilePath", "qxrd.log"),
    m_FreshStart(false),
    m_Splash(NULL),
    m_Window(NULL),
    m_ServerThread(NULL),
    m_Server(NULL),
    m_SimpleServerThread(NULL),
    m_SimpleServer(NULL),
    m_AcquisitionThread(NULL),
    m_AllocatorThread(NULL),
    m_FileSaverThread(NULL),
    m_SettingsSaverThread(NULL),
    m_SettingsSaver(NULL),
    m_NIDAQPluginInterface(NULL),
#ifdef HAVE_PERKIN_ELMER
    m_PerkinElmerPluginInterface(NULL),
#endif
    m_LogFileMutex(QMutex::Recursive),
    m_LogFile(NULL)
{
//  printf("Argc = %d\n", argc);

  for (int i=1; i<argc; i++) {
    int dbg=0;
//    printf("Arg %d = %s\n", i, argv[i]);

    if (strcmp(argv[i],"-fresh") == 0) {
      m_FreshStart = true;
    } else if (sscanf(argv[i],"-debug=%d",&dbg)==1) {
      set_Debug(dbg);
    }
  }
}

bool QxrdApplication::init(QSplashScreen *splash)
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

//  printf("application thread %p\n", thread());

  g_Application = this;

  setOrganizationName("bessrc");
  setOrganizationDomain("xor.aps.anl.gov");
  setApplicationName("qxrd");

  int detectorType = 0;

  int specServer = 0;
  int specServerPort = 0;
  int simpleServer = 0;
  int simpleServerPort = 0;

  {
    QxrdSettings settings;

    detectorType = settings.value("application/detectorType").toInt();
    gCEPDebug = settings.value("application/debug").toInt();
    specServer = settings.value("application/runSpecServer").toInt();
    specServerPort = settings.value("application/specServerPort").toInt();
    simpleServer = settings.value("application/runSimpleServer").toInt();
    simpleServerPort = settings.value("application/simpleServerPort").toInt();
  }

  splashMessage("Qxrd Version " QXRD_VERSION "\nInitializing Memory Allocator");

  m_AllocatorThread = new QxrdAllocatorThread();
  m_AllocatorThread -> start();
  m_Allocator = m_AllocatorThread -> allocator();

  splashMessage("Qxrd Version " QXRD_VERSION "\nInitializing File Saver");

  m_FileSaverThread = new QxrdFileSaverThread(m_Allocator);
  m_FileSaverThread -> start();

  splashMessage("Qxrd Version " QXRD_VERSION "\nInitializing Data Processing");

  m_DataProcessorThread = new QxrdDataProcessorThread(NULL, m_Allocator, m_FileSaverThread);

  m_DataProcessorThread -> start();
  m_DataProcessor = m_DataProcessorThread -> dataProcessor();

  m_FileSaverThread -> setProcessor(m_DataProcessor);

  splashMessage("Qxrd Version " QXRD_VERSION "\nInitializing Data Acquisition");

  m_AcquisitionThread = new QxrdAcquisitionThread(m_DataProcessor, m_Allocator, detectorType);
  m_AcquisitionThread -> start();
  m_Acquisition = m_AcquisitionThread -> acquisition();

  m_DataProcessor -> setAcquisition(m_Acquisition);
  m_FileSaverThread -> setAcquisition(m_Acquisition);

  splashMessage("Qxrd Version " QXRD_VERSION "\nOpening Main Window");

  m_Window = new QxrdWindow(this, m_Acquisition, m_DataProcessor, m_Allocator);
//  m_Window -> show();

  m_DataProcessor -> setWindow(m_Window);
  m_Acquisition -> setWindow(m_Window);

//  connect(this, SIGNAL(printMessage(QString,QDateTime)), m_Window, SLOT(printMessage(QString,QDateTime)));
//  connect(this, SIGNAL(criticalMessage(QString,QDateTime)), m_Window, SLOT(criticalMessage(QString,QDateTime)));
//  connect(this, SIGNAL(statusMessage(QString,QDateTime)), m_Window, SLOT(statusMessage(QString,QDateTime)));
//  emit printMessage("window shown");

  splashMessage("Qxrd Version " QXRD_VERSION "\nLoading plugins");

  emit printMessage("about to load plugins");

  loadPlugins();

  m_Acquisition -> setNIDAQPlugin(nidaqPlugin());

//  QObject *nidaq = dynamic_cast<QObject*>(nidaqPlugin());

//  if (nidaq) {
//    connect(nidaq, SIGNAL(statusMessage(QString,QDateTime)), m_Window, SLOT(statusMessage(QString,QDateTime)));
//    connect(nidaq, SIGNAL(printMessage(QString,QDateTime)), m_Window, SLOT(printMessage(QString,QDateTime)));
//    connect(nidaq, SIGNAL(criticalMessage(QString,QDateTime)), m_Window, SLOT(criticalMessage(QString,QDateTime)));
//  }

//  connect(m_Acquisition, SIGNAL(statusMessage(QString,QDateTime)), m_Window, SLOT(statusMessage(QString,QDateTime)));
//  connect(m_Acquisition, SIGNAL(printMessage(QString,QDateTime)), m_Window, SLOT(printMessage(QString,QDateTime)));
//  connect(m_Acquisition, SIGNAL(criticalMessage(QString,QDateTime)), m_Window, SLOT(criticalMessage(QString,QDateTime)));

  m_AcquisitionThread->initialize();

  m_Window -> onAcquisitionInit();

//  connect(m_DataProcessorThread, SIGNAL(printMessage(QString,QDateTime)), m_Window, SLOT(printMessage(QString,QDateTime)));
//  connect(m_DataProcessorThread, SIGNAL(statusMessage(QString,QDateTime)), m_Window, SLOT(statusMessage(QString,QDateTime)));
//  connect(m_DataProcessorThread, SIGNAL(criticalMessage(QString,QDateTime)), m_Window, SLOT(criticalMessage(QString,QDateTime)));

//  connect(m_DataProcessor, SIGNAL(printMessage(QString,QDateTime)), m_Window, SLOT(printMessage(QString,QDateTime)));
//  connect(m_DataProcessor -> integrator(), SIGNAL(printMessage(QString,QDateTime)), m_Window, SLOT(printMessage(QString,QDateTime)));
//  connect(m_DataProcessor, SIGNAL(statusMessage(QString,QDateTime)), m_Window, SLOT(statusMessage(QString,QDateTime)));
//  connect(m_DataProcessor -> integrator(), SIGNAL(statusMessage(QString,QDateTime)), m_Window, SLOT(statusMessage(QString,QDateTime)));
//  connect(m_DataProcessor, SIGNAL(criticalMessage(QString,QDateTime)), m_Window, SLOT(criticalMessage(QString,QDateTime)));
//  connect(m_DataProcessor -> integrator(), SIGNAL(criticalMessage(QString,QDateTime)), m_Window, SLOT(criticalMessage(QString,QDateTime)));

//  connect(m_AllocatorThread, SIGNAL(statusMessage(QString,QDateTime)), m_Window, SLOT(statusMessage(QString,QDateTime)));
//  connect(m_AllocatorThread, SIGNAL(printMessage(QString,QDateTime)), m_Window, SLOT(printMessage(QString,QDateTime)));
//  connect(m_AllocatorThread, SIGNAL(criticalMessage(QString,QDateTime)), m_Window, SLOT(criticalMessage(QString,QDateTime)));

//  connect(m_FileSaverThread, SIGNAL(statusMessage(QString,QDateTime)), m_Window, SLOT(statusMessage(QString,QDateTime)));
//  connect(m_FileSaverThread, SIGNAL(printMessage(QString,QDateTime)), m_Window, SLOT(printMessage(QString,QDateTime)));
//  connect(m_FileSaverThread, SIGNAL(criticalMessage(QString,QDateTime)), m_Window, SLOT(criticalMessage(QString,QDateTime)));

  if (specServer) {
    splashMessage("Qxrd Version " QXRD_VERSION "\nStarting SPEC Server");

    m_ServerThread = new QxrdServerThread("qxrd", specServerPort);

//    connect(m_ServerThread,       SIGNAL(printMessage(QString,QDateTime)), m_Window,            SLOT(printMessage(QString,QDateTime)));
//    connect(m_ServerThread,       SIGNAL(statusMessage(QString,QDateTime)), m_Window,            SLOT(statusMessage(QString,QDateTime)));
//    connect(m_ServerThread,       SIGNAL(criticalMessage(QString,QDateTime)), m_Window,            SLOT(criticalMessage(QString,QDateTime)));

    m_ServerThread -> start();
    m_Server = m_ServerThread -> server();

    emit printMessage(tr("Spec Server Thread started: listening on port %1").arg(m_Server->serverPort()));
  }

  if (simpleServer) {
    splashMessage("Qxrd Version " QXRD_VERSION "\nStarting Simple Socket Server");

    m_SimpleServerThread = new QxrdSimpleServerThread("simpleserver", simpleServerPort);

//    connect(m_SimpleServerThread,       SIGNAL(printMessage(QString,QDateTime)), m_Window,            SLOT(printMessage(QString,QDateTime)));
//    connect(m_SimpleServerThread,       SIGNAL(statusMessage(QString,QDateTime)), m_Window,            SLOT(statusMessage(QString,QDateTime)));
//    connect(m_SimpleServerThread,       SIGNAL(criticalMessage(QString,QDateTime)), m_Window,            SLOT(criticalMessage(QString,QDateTime)));

    m_SimpleServerThread -> start();
    m_SimpleServer = m_SimpleServerThread -> server();

    emit printMessage(tr("Simple Server Thread started: listening on port %1").arg(m_SimpleServer->serverPort()));
  }


  splashMessage("Qxrd Version " QXRD_VERSION "\nStarting Scripting System");

  m_ScriptEngineThread = new QxrdScriptEngineThread(this, m_Window, m_Acquisition, m_DataProcessor);
  m_ScriptEngineThread -> start();
  m_ScriptEngine = m_ScriptEngineThread -> scriptEngine();

//  m_ScriptEngineDebugger = new QScriptEngineDebugger(this);
//  m_ScriptEngineDebugger -> attachTo(m_ScriptEngine->scriptEngine());
//  m_ScriptEngineDebugger -> setAutoShowStandardWindow(true);

//  emit printMessage("script thread created");

//  connect(m_ScriptEngineThread, SIGNAL(printMessage(QString,QDateTime)), m_Window,            SLOT(printMessage(QString,QDateTime)));
//  connect(m_ScriptEngineThread, SIGNAL(statusMessage(QString,QDateTime)), m_Window,            SLOT(statusMessage(QString,QDateTime)));
//  connect(m_ScriptEngineThread, SIGNAL(criticalMessage(QString,QDateTime)), m_Window,            SLOT(criticalMessage(QString,QDateTime)));

//  connect(m_ScriptEngine,       SIGNAL(printMessage(QString,QDateTime)), m_Window,            SLOT(printMessage(QString,QDateTime)));
//  connect(m_ScriptEngine,       SIGNAL(statusMessage(QString,QDateTime)), m_Window,            SLOT(statusMessage(QString,QDateTime)));
//  connect(m_ScriptEngine,       SIGNAL(criticalMessage(QString,QDateTime)), m_Window,            SLOT(criticalMessage(QString,QDateTime)));

  if (m_Server) {
    connect(m_Server,         SIGNAL(executeCommand(QString)),           m_ScriptEngine,    SLOT(evaluateSpecCommand(QString)));
    connect(m_ScriptEngine,   SIGNAL(specResultAvailable(QScriptValue)), m_Server,          SLOT(finishedCommand(QScriptValue)));
  }

  if (m_SimpleServer) {
    connect(m_SimpleServer,   SIGNAL(executeCommand(QString)),           m_ScriptEngine,    SLOT(evaluateSimpleServerCommand(QString)));
    connect(m_ScriptEngine,   SIGNAL(simpleServerResultAvailable(QScriptValue)), m_SimpleServer,  SLOT(finishedCommand(QScriptValue)));
  }

  connect(m_Window,         SIGNAL(executeCommand(QString)),           m_ScriptEngine,    SLOT(evaluateAppCommand(QString)));
  connect(m_ScriptEngine,   SIGNAL(appResultAvailable(QScriptValue)),  m_Window,          SLOT(finishedCommand(QScriptValue)));

  m_Window -> setScriptEngine(m_ScriptEngine);


//  emit printMessage("script thread started");

//  m_AcquisitionThread -> setWindow(m_Window);

  connect(this, SIGNAL(aboutToQuit()), this, SLOT(shutdownThreads()));

  connect(prop_Debug(), SIGNAL(changedValue(int)), this, SLOT(debugChanged(int)));

  splashMessage("Qxrd Version " QXRD_VERSION "\nLoading Preferences");

  readSettings();

  m_SettingsSaverThread = new QxrdSettingsSaverThread(this);

//  connect(m_SettingsSaverThread, SIGNAL(printMessage(QString,QDateTime)),
//          m_Window,              SLOT(printMessage(QString,QDateTime)));
//  connect(m_SettingsSaverThread, SIGNAL(statusMessage(QString,QDateTime)),
//          m_Window,              SLOT(statusMessage(QString,QDateTime)));
//  connect(m_SettingsSaverThread, SIGNAL(criticalMessage(QString,QDateTime)),
//          m_Window,              SLOT(criticalMessage(QString,QDateTime)));

  m_SettingsSaverThread -> start();
  m_SettingsSaver = m_SettingsSaverThread -> settingsSaver();

  g_Application->printMessage(tr("Optimal thread count = %1").arg(QThread::idealThreadCount()));

  splashMessage("Qxrd Version " QXRD_VERSION "\nLoading Background Images");

  m_DataProcessor -> loadDefaultImages();

#ifdef Q_OS_WIN32
  QDir::setCurrent(QDir::homePath());
#endif

  g_Application->printMessage(tr("Current directory %1").arg(QDir::currentPath()));

  m_Window -> show();

  m_ResponseTimer = new QxrdResponseTimer(1000, this);

//  connect(m_ResponseTimer, SIGNAL(printMessage(QString,QDateTime)),
//          m_Window, SLOT(printMessage(QString,QDateTime)));

  return true;
}

QxrdApplication::~QxrdApplication()
{
  if (qcepDebug(DEBUG_APP)) {
    printf("QxrdApplication::~QxrdApplication\n");
  }

//  delete m_ServerThread;
//  delete m_SimpleServerThread;
//  delete m_AcquisitionThread;
//  delete m_DataProcessorThread;
//  delete m_FileSaverThread;
//  delete m_ScriptEngineThread;
//  delete m_AllocatorThread;
//  delete m_Window;

//  m_ServerThread -> deleteLater();
//  m_SimpleServerThread -> deleteLater();
//  m_AcquisitionThread -> deleteLater();
//  m_DataProcessorThread -> deleteLater();
//  m_FileSaverThread -> deleteLater();
//  m_ScriptEngineThread -> deleteLater();
//  m_AllocatorThread -> deleteLater();

//  m_Window -> deleteLater();
//  m_AcquisitionThread -> deleteLater();
//  m_DataProcessorThread -> deleteLater();
//  m_FileSaverThread -> deleteLater();
//  m_AllocatorThread -> deleteLater();
//  m_ScriptEngineThread -> deleteLater();

  if (qcepDebug(DEBUG_APP)) {
    printf("QxrdApplication::~QxrdApplication finished\n");
  }

  closeLogFile();
}

QxrdApplication* QxrdApplication::application()
{
  return g_Application;
}

void QxrdApplication::splashMessage(const char *msg)
{
  if (m_Splash) {
    m_Splash->showMessage(msg, Qt::AlignBottom|Qt::AlignHCenter);
    processEvents();
  }
}

void QxrdApplication::splashMessage(QString msg)
{
  splashMessage(qPrintable(msg));
}

#ifdef HAVE_PERKIN_ELMER

QxrdPerkinElmerPluginInterface* QxrdApplication::perkinElmerPlugin()
{
  return m_PerkinElmerPluginInterface;
}

#endif

QxrdNIDAQPluginInterface* QxrdApplication::nidaqPlugin()
{
  return m_NIDAQPluginInterface;
}

#define xstr(s) str(s)
#define str(s) #s

void QxrdApplication::loadPlugins()
{
  QList<QDir> pluginsDirList;

#ifdef QXRD_PLUGIN_PATH
  pluginsDirList.append(QDir(xstr(QXRD_PLUGIN_PATH)));
#else
  QDir pluginsDir = QDir(qApp->applicationDirPath());
  pluginsDir.cd("plugins");
  pluginsDirList.append(pluginsDir);
#endif

  foreach (QDir pluginsDir, pluginsDirList) {
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
      QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
      QObject *plugin = loader.instance();
      if (plugin) {
        QString pluginName = "";

        QxrdDetectorPluginInterface* detector = qobject_cast<QxrdDetectorPluginInterface*>(plugin);

        if (detector) {
          pluginName = detector -> name();
        }

        QxrdProcessorInterface* processor = qobject_cast<QxrdProcessorInterface*>(plugin);

        if (processor) {
          pluginName = processor -> name();
        }

#ifdef HAVE_PERKIN_ELMER
        QxrdPerkinElmerPluginInterface *perkinElmer = qobject_cast<QxrdPerkinElmerPluginInterface*>(plugin);

        if (perkinElmer) {
          pluginName = perkinElmer -> name();

          m_PerkinElmerPluginInterface = perkinElmer;
        }
#endif

        QxrdNIDAQPluginInterface *nidaq = qobject_cast<QxrdNIDAQPluginInterface*>(plugin);

        if (nidaq) {
          pluginName = nidaq -> name();

          m_NIDAQPluginInterface = nidaq;
        }

        splashMessage(tr("Qxrd Version " QXRD_VERSION "\nLoaded plugin \"%1\"").arg(pluginName));

        g_Application->printMessage(tr("Loaded plugin \"%1\" from %2")
                          .arg(pluginName)
                          .arg(pluginsDir.absoluteFilePath(fileName)));
      } else {

        if (QLibrary::isLibrary(pluginsDir.absoluteFilePath(fileName))) {
          QString msg = tr("Qxrd Version " QXRD_VERSION "\nFailed to load plugin %1 : %2")
              .arg(pluginsDir.absoluteFilePath(fileName))
              .arg(loader.errorString());
          splashMessage(msg);
          g_Application->printMessage(msg);
        }
      }
    }
  }
}

void QxrdApplication::printMessage(QString msg, QDateTime ts)
{
  QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+msg.trimmed();

  logMessage(message);

  if (window()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(window(), "displayMessage", Qt::QueuedConnection, Q_ARG(QString, message)));
  }
}

void QxrdApplication::statusMessage(QString msg, QDateTime ts)
{
  QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+msg.trimmed();

  logMessage(message);

  if (window()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(window(), "displayStatusMessage", Qt::QueuedConnection, Q_ARG(QString, message)));
  }
}

void QxrdApplication::criticalMessage(QString msg, QDateTime ts)
{
  QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+msg.trimmed();

  logMessage(message);

  if (window()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(window(), "displayCriticalMessage", Qt::QueuedConnection, Q_ARG(QString, message)));
  }
}

void QxrdApplication::openLogFile()
{
  if (m_LogFile == NULL) {
    m_LogFile = fopen(qPrintable(get_LogFilePath()), "a");

    if (m_LogFile) {
      writeLogHeader();
    }
  }
}

void QxrdApplication::newLogFile(QString path)
{
  if (m_LogFile) {
    fclose(m_LogFile);
    m_LogFile = NULL;
  }

  set_LogFilePath(path);

  openLogFile();
}

FILE* QxrdApplication::logFile()
{
  return m_LogFile;
}

void QxrdApplication::writeLogHeader()
{
  if (m_LogFile) {
    fprintf(m_LogFile, "#F %s\n", qPrintable(get_LogFilePath()));
    fprintf(m_LogFile, "#E %d\n", QDateTime::currentDateTime().toTime_t());
    fprintf(m_LogFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")));
    fflush(m_LogFile);
  }
}

void QxrdApplication::logMessage(QString msg)
{
  openLogFile();

  if (m_LogFile) {
    fprintf(m_LogFile, "#CX %s\n", qPrintable(msg));
    fflush(m_LogFile);
  }
}

void QxrdApplication::closeLogFile()
{
  if (m_LogFile) {
    logMessage(tr("%1 ------- shutdown --------").
               arg(QDateTime::currentDateTime().toString("yyyy.MM.dd : hh:mm:ss.zzz ")));
    fclose(m_LogFile);
    m_LogFile = NULL;
  }
}

void QxrdApplication::readSettings()
{
  QxrdSettings settings;

  QcepProperty::readSettings(this, &staticMetaObject, "application", settings);

  m_Window       -> readSettings(settings, "window");
  m_Acquisition  -> readSettings(settings, "acquire");
  m_DataProcessor-> readSettings(settings, "processor");

  newLogFile(get_LogFilePath());
}

void QxrdApplication::writeSettings()
{
  QxrdSettings settings;

  m_Window       -> writeSettings(settings, "window");
  m_Acquisition  -> writeSettings(settings, "acquire");
  m_DataProcessor-> writeSettings(settings, "processor");

  QcepProperty::writeSettings(this, &staticMetaObject, "application", settings);
}

void QxrdApplication::doLoadPreferences()
{
  QString loadPrefsFrom = QFileDialog::getOpenFileName(m_Window,
                                                       "Load QXRD Preferences from...");

  if (loadPrefsFrom != "") {
    QxrdSettings settings(loadPrefsFrom, QSettings::IniFormat);

    QcepProperty::readSettings(this, &staticMetaObject, "application", settings);

    m_Window       -> readSettings(settings, "window");
    m_Acquisition  -> readSettings(settings, "acquire");
    m_DataProcessor-> readSettings(settings, "processor");
  }
}

void QxrdApplication::doSavePreferences()
{
  QString savePrefsTo = QFileDialog::getSaveFileName(m_Window,
                                                     "Save QXRD Preferences to...");

  if (savePrefsTo != "") {
    QxrdSettings settings(savePrefsTo, QSettings::IniFormat);

    m_Window       -> writeSettings(settings, "window");
    m_Acquisition  -> writeSettings(settings, "acquire");
    m_DataProcessor-> writeSettings(settings, "processor");

    QcepProperty::writeSettings(this, &staticMetaObject, "application", settings);
  }
}

void QxrdApplication::possiblyQuit()
{
  if (wantToQuit()) {
    quit();
  }
}

bool QxrdApplication::wantToQuit()
{
  return QMessageBox::question(m_Window, tr("Really Quit?"),
                               tr("Do you really want to exit the application?"),
                                  QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
}

void QxrdApplication::shutdownThread(QxrdThread *thread)
{
  if (thread) {
    thread->shutdown();
    delete thread;
  }
}

void QxrdApplication::shutdownThreads()
{
  if (qcepDebug(DEBUG_APP)) {
    printf("QxrdApplication::shutdownThreads\n");
  }

  writeSettings();

  shutdownThread(m_SettingsSaverThread);
  shutdownThread(m_SimpleServerThread);
  shutdownThread(m_ServerThread);
  shutdownThread(m_ScriptEngineThread);
  shutdownThread(m_AcquisitionThread);
  shutdownThread(m_DataProcessorThread);
//  shutdownThread(m_AllocatorThread);
//  shutdownThread(m_FileSaverThread);
}

QxrdWindow *QxrdApplication::window()
{
  return m_Window;
}

QxrdAcquisitionThread *QxrdApplication::acquisitionThread()
{
  return m_AcquisitionThread;
}

QxrdAcquisition *QxrdApplication::acquisition() const
{
  return m_Acquisition;
}

QxrdDataProcessor *QxrdApplication::dataProcessor() const
{
  return m_DataProcessor;
}

QxrdAllocator *QxrdApplication::allocator() const
{
  return m_Allocator;
}

void QxrdApplication::editPreferences()
{
  QxrdPreferencesDialog prefs;

  prefs.exec();
}

void QxrdApplication::debugChanged(int newValue)
{
  g_Application->printMessage(tr("Debug level changed from %1 to %2").arg(gCEPDebug).arg(newValue));

  gCEPDebug = newValue;
}

static void qxrdTIFFWarningHandler(const char* /*module*/, const char* /*fmt*/, va_list /*ap*/)
{
//  char msg[100];
//
//  vsnprintf(msg, sizeof(msg), fmt, ap);
//
//  g_Application -> tiffWarning(module, msg);
}

//static void qxrdTIFFWarningHandlerExt(thandle_t fd, const char* module, const char* fmt, va_list ap)
//{
//  char msg[100];
//
//  vsnprintf(msg, sizeof(msg), fmt, ap);
//
//  g_Application -> tiffWarning(module, msg);
//}

static void qxrdTIFFErrorHandler(const char* module, const char* fmt, va_list ap)
{
  char msg[100];

  vsnprintf(msg, sizeof(msg), fmt, ap);

  g_Application -> tiffError(module, msg);
}

//static void qxrdTIFFErrorHandlerExt(thandle_t fd, const char* module, const char* fmt, va_list ap)
//{
//  char msg[100];
//
//  vsnprintf(msg, sizeof(msg), fmt, ap);
//
//  g_Application -> tiffError(module, msg);
//}

void QxrdApplication::setupTiffHandlers()
{
  TIFFSetErrorHandler      (&qxrdTIFFErrorHandler);
  TIFFSetErrorHandlerExt   (NULL);
  TIFFSetWarningHandler    (&qxrdTIFFWarningHandler);
  TIFFSetWarningHandlerExt (NULL);
}

void QxrdApplication::tiffWarning(const char *module, const char *msg)
{
  g_Application->criticalMessage(tr("TIFF Warning from %1 : %2").arg(module).arg(msg));
}

void QxrdApplication::tiffError(const char *module, const char *msg)
{
  g_Application->criticalMessage(tr("TIFF Error from %1 : %2").arg(module).arg(msg));
}

bool QxrdApplication::event(QEvent *ev)
{
  QTime tick;
  tick.start();

  bool res = QApplication::event(ev);

  int elapsed = tick.restart();

  if (elapsed > 1000) {
    g_Application->printMessage("event processing took more than 1 sec");
  }

  return res;
}
