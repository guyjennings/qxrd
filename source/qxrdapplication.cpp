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
    m_NIDAQPluginInterface(NULL)
#ifdef HAVE_PERKIN_ELMER
      ,
    m_PerkinElmerPluginInterface(NULL)
#endif
{
}

void QxrdApplication::init(QSplashScreen *splash)
{
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

  splashMessage("Initializing Memory Allocator");

  m_AllocatorThread = new QxrdAllocatorThread();
  m_AllocatorThread -> start();
  m_Allocator = m_AllocatorThread -> allocator();

  splashMessage("Initializing File Saver");

  m_FileSaverThread = new QxrdFileSaverThread(m_Allocator);
  m_FileSaverThread -> start();

  splashMessage("Initializing Data Processing");

  m_DataProcessorThread = new QxrdDataProcessorThread(NULL, m_Allocator, m_FileSaverThread);

  m_DataProcessorThread -> start();
  m_DataProcessor = m_DataProcessorThread -> dataProcessor();

  m_FileSaverThread -> setProcessor(m_DataProcessor);

  splashMessage("Initializing Data Acquisition");

  m_AcquisitionThread = new QxrdAcquisitionThread(m_DataProcessor, m_Allocator, detectorType);
  m_AcquisitionThread -> start();
  m_Acquisition = m_AcquisitionThread -> acquisition();

  m_DataProcessor -> setAcquisition(m_Acquisition);
  m_FileSaverThread -> setAcquisition(m_Acquisition);

  splashMessage("Opening Main Window");

  m_Window = new QxrdWindow(this, m_Acquisition, m_DataProcessor, m_Allocator);
//  m_Window -> show();

  m_DataProcessor -> setWindow(m_Window);
  m_Acquisition -> setWindow(m_Window);

  connect(this, SIGNAL(printMessage(QDateTime,QString)), m_Window, SLOT(printMessage(QDateTime,QString)));
  connect(this, SIGNAL(criticalMessage(QDateTime,QString)), m_Window, SLOT(criticalMessage(QDateTime,QString)));
  connect(this, SIGNAL(statusMessage(QDateTime,QString)), m_Window, SLOT(statusMessage(QDateTime,QString)));
//  emit printMessage("window shown");

  emit printMessage(QDateTime::currentDateTime(), "about to load plugins");

  loadPlugins();

  m_Acquisition -> setNIDAQPlugin(nidaqPlugin());

  connect(m_Acquisition, SIGNAL(statusMessage(QDateTime,QString)), m_Window, SLOT(statusMessage(QDateTime,QString)));
  connect(m_Acquisition, SIGNAL(printMessage(QDateTime,QString)), m_Window, SLOT(printMessage(QDateTime,QString)));
  connect(m_Acquisition, SIGNAL(criticalMessage(QDateTime,QString)), m_Window, SLOT(criticalMessage(QDateTime,QString)));

  m_AcquisitionThread->initialize();

  m_Window -> onAcquisitionInit();

  connect(m_DataProcessorThread, SIGNAL(printMessage(QDateTime,QString)), m_Window, SLOT(printMessage(QDateTime,QString)));
  connect(m_DataProcessorThread, SIGNAL(statusMessage(QDateTime,QString)), m_Window, SLOT(statusMessage(QDateTime,QString)));
  connect(m_DataProcessorThread, SIGNAL(criticalMessage(QDateTime,QString)), m_Window, SLOT(criticalMessage(QDateTime,QString)));

  connect(m_DataProcessor, SIGNAL(printMessage(QDateTime,QString)), m_Window, SLOT(printMessage(QDateTime,QString)));
  connect(m_DataProcessor -> integrator(), SIGNAL(printMessage(QDateTime,QString)), m_Window, SLOT(printMessage(QDateTime,QString)));
  connect(m_DataProcessor, SIGNAL(statusMessage(QDateTime,QString)), m_Window, SLOT(statusMessage(QDateTime,QString)));
  connect(m_DataProcessor -> integrator(), SIGNAL(statusMessage(QDateTime,QString)), m_Window, SLOT(statusMessage(QDateTime,QString)));
  connect(m_DataProcessor, SIGNAL(criticalMessage(QDateTime,QString)), m_Window, SLOT(criticalMessage(QDateTime,QString)));
  connect(m_DataProcessor -> integrator(), SIGNAL(criticalMessage(QDateTime,QString)), m_Window, SLOT(criticalMessage(QDateTime,QString)));

  connect(m_AllocatorThread, SIGNAL(statusMessage(QDateTime,QString)), m_Window, SLOT(statusMessage(QDateTime,QString)));
  connect(m_AllocatorThread, SIGNAL(printMessage(QDateTime,QString)), m_Window, SLOT(printMessage(QDateTime,QString)));
  connect(m_AllocatorThread, SIGNAL(criticalMessage(QDateTime,QString)), m_Window, SLOT(criticalMessage(QDateTime,QString)));

  connect(m_FileSaverThread, SIGNAL(statusMessage(QDateTime,QString)), m_Window, SLOT(statusMessage(QDateTime,QString)));
  connect(m_FileSaverThread, SIGNAL(printMessage(QDateTime,QString)), m_Window, SLOT(printMessage(QDateTime,QString)));
  connect(m_FileSaverThread, SIGNAL(criticalMessage(QDateTime,QString)), m_Window, SLOT(criticalMessage(QDateTime,QString)));

  if (specServer) {
    splashMessage("Starting SPEC Server");

    m_ServerThread = new QxrdServerThread("qxrd", specServerPort);

    connect(m_ServerThread,       SIGNAL(printMessage(QDateTime,QString)), m_Window,            SLOT(printMessage(QDateTime,QString)));
    connect(m_ServerThread,       SIGNAL(statusMessage(QDateTime,QString)), m_Window,            SLOT(statusMessage(QDateTime,QString)));
    connect(m_ServerThread,       SIGNAL(criticalMessage(QDateTime,QString)), m_Window,            SLOT(criticalMessage(QDateTime,QString)));

    m_ServerThread -> start();
    m_Server = m_ServerThread -> server();

    emit printMessage(QDateTime::currentDateTime(),
                      tr("Spec Server Thread started: listening on port %1").arg(m_Server->serverPort()));
  }

  if (simpleServer) {
    splashMessage("Starting Simple Socket Server");

    m_SimpleServerThread = new QxrdSimpleServerThread("simpleserver", simpleServerPort);

    connect(m_SimpleServerThread,       SIGNAL(printMessage(QDateTime,QString)), m_Window,            SLOT(printMessage(QDateTime,QString)));
    connect(m_SimpleServerThread,       SIGNAL(statusMessage(QDateTime,QString)), m_Window,            SLOT(statusMessage(QDateTime,QString)));
    connect(m_SimpleServerThread,       SIGNAL(criticalMessage(QDateTime,QString)), m_Window,            SLOT(criticalMessage(QDateTime,QString)));

    m_SimpleServerThread -> start();
    m_SimpleServer = m_SimpleServerThread -> server();

    emit printMessage(QDateTime::currentDateTime(),
                      tr("Simple Server Thread started: listening on port %1").arg(m_SimpleServer->serverPort()));
  }


  splashMessage("Starting Scripting System");

  m_ScriptEngineThread = new QxrdScriptEngineThread(this, m_Window, m_Acquisition, m_DataProcessor);
  m_ScriptEngineThread -> start();
  m_ScriptEngine = m_ScriptEngineThread -> scriptEngine();

//  m_ScriptEngineDebugger = new QScriptEngineDebugger(this);
//  m_ScriptEngineDebugger -> attachTo(m_ScriptEngine->scriptEngine());
//  m_ScriptEngineDebugger -> setAutoShowStandardWindow(true);

//  emit printMessage("script thread created");

  connect(m_ScriptEngineThread, SIGNAL(printMessage(QDateTime,QString)), m_Window,            SLOT(printMessage(QDateTime,QString)));
  connect(m_ScriptEngineThread, SIGNAL(statusMessage(QDateTime,QString)), m_Window,            SLOT(statusMessage(QDateTime,QString)));
  connect(m_ScriptEngineThread, SIGNAL(criticalMessage(QDateTime,QString)), m_Window,            SLOT(criticalMessage(QDateTime,QString)));

  connect(m_ScriptEngine,       SIGNAL(printMessage(QDateTime,QString)), m_Window,            SLOT(printMessage(QDateTime,QString)));
  connect(m_ScriptEngine,       SIGNAL(statusMessage(QDateTime,QString)), m_Window,            SLOT(statusMessage(QDateTime,QString)));
  connect(m_ScriptEngine,       SIGNAL(criticalMessage(QDateTime,QString)), m_Window,            SLOT(criticalMessage(QDateTime,QString)));

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


//  emit printMessage(QDateTime::currentDateTime(), "script thread started");

//  m_AcquisitionThread -> setWindow(m_Window);

  connect(this, SIGNAL(aboutToQuit()), this, SLOT(shutdownThreads()));

  connect(prop_Debug(), SIGNAL(changedValue(int)), this, SLOT(debugChanged(int)));

  splashMessage("Loading Preferences");

  readSettings();

  m_SettingsSaverThread = new QxrdSettingsSaverThread(this);

  connect(m_SettingsSaverThread, SIGNAL(printMessage(QDateTime,QString)),
          m_Window,              SLOT(printMessage(QDateTime,QString)));
  connect(m_SettingsSaverThread, SIGNAL(statusMessage(QDateTime,QString)),
          m_Window,              SLOT(statusMessage(QDateTime,QString)));
  connect(m_SettingsSaverThread, SIGNAL(criticalMessage(QDateTime,QString)),
          m_Window,              SLOT(criticalMessage(QDateTime,QString)));

  m_SettingsSaverThread -> start();
  m_SettingsSaver = m_SettingsSaverThread -> settingsSaver();

  emit printMessage(QDateTime::currentDateTime(),
                    tr("Optimal thread count = %1").arg(QThread::idealThreadCount()));

  splashMessage("Loading Background Images");

  m_DataProcessor -> loadDefaultImages();

#ifdef Q_OS_WIN32
  QDir::setCurrent(QDir::homePath());
#endif

  emit printMessage(QDateTime::currentDateTime(),
                    tr("Current directory %1").arg(QDir::currentPath()));

  m_Window -> show();
}

QxrdApplication::~QxrdApplication()
{
  QCEP_DEBUG(DEBUG_APP,
             printf("QxrdApplication::~QxrdApplication\n");
  );

  m_Window -> deleteLater();
  m_AcquisitionThread -> deleteLater();
  m_ServerThread -> deleteLater();
  m_DataProcessorThread -> deleteLater();
  m_FileSaverThread -> deleteLater();
  m_AllocatorThread -> deleteLater();
  m_ScriptEngineThread -> deleteLater();

  QCEP_DEBUG(DEBUG_APP,
             printf("QxrdApplication::~QxrdApplication finished\n");
  );
}

QxrdApplication* QxrdApplication::application()
{
  return g_Application;
}

void QxrdApplication::splashMessage(const char *msg)
{
  if (m_Splash) {
    m_Splash->showMessage(msg, Qt::AlignBottom|Qt::AlignHCenter);
  }
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

        emit printMessage(QDateTime::currentDateTime(),
                          tr("Loaded plugin \"%1\" from %2")
                          .arg(pluginName)
                          .arg(pluginsDir.absoluteFilePath(fileName)));
      } else {
        if (QLibrary::isLibrary(pluginsDir.absoluteFilePath(fileName))) {
          emit printMessage(QDateTime::currentDateTime(),
                            tr("Failed to load plugin %1 : %2")
                            .arg(pluginsDir.absoluteFilePath(fileName))
                            .arg(loader.errorString()));
        }
      }
    }
  }
}

void QxrdApplication::readSettings()
{
  QxrdSettings settings;

  m_Window       -> readSettings(settings, "window");
  m_Acquisition  -> readSettings(settings, "acquire");
  m_DataProcessor-> readSettings(settings, "processor");

  QcepProperty::readSettings(this, &staticMetaObject, "application", settings);
}

void QxrdApplication::writeSettings()
{
  QxrdSettings settings;

  m_Window       -> writeSettings(settings, "window");
  m_Acquisition  -> writeSettings(settings, "acquire");
  m_DataProcessor-> writeSettings(settings, "processor");

  QcepProperty::writeSettings(this, &staticMetaObject, "application", settings);
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

void QxrdApplication::shutdownThreads()
{
  QCEP_DEBUG(DEBUG_APP,
             printf("QxrdApplication::shutdownThreads\n");
  );

  writeSettings();

  m_DataProcessorThread -> shutdown();
  m_AcquisitionThread -> shutdown();
  m_ServerThread -> shutdown();
  m_AllocatorThread -> shutdown();
  m_FileSaverThread -> shutdown();
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

void QxrdApplication::editPreferences()
{
  QxrdPreferencesDialog prefs;

  prefs.exec();
}

void QxrdApplication::debugChanged(int newValue)
{
  emit printMessage(QDateTime::currentDateTime(),
                    tr("Debug level changed from %1 to %2").arg(gCEPDebug).arg(newValue));

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
  emit criticalMessage(QDateTime::currentDateTime(),
                       tr("TIFF Warning from %1 : %2").arg(module).arg(msg));
}

void QxrdApplication::tiffError(const char *module, const char *msg)
{
  emit criticalMessage(QDateTime::currentDateTime(),
                       tr("TIFF Error from %1 : %2").arg(module).arg(msg));
}
