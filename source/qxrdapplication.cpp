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
#include "qxrdsettings.h"
#include "qxrddetectorplugininterface.h"
#include "qxrdprocessorinterface.h"

#include <QTime>
#include <QtConcurrentRun>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <tiffio.h>
#include <QPluginLoader>

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
    m_Window(NULL),
    m_ServerThread(NULL),
    m_Server(NULL),
    m_SimpleServerThread(NULL),
    m_SimpleServer(NULL),
    m_AcquisitionThread(NULL),
    m_AllocatorThread(NULL),
    m_FileSaverThread(NULL),
    m_SettingsSaverThread(NULL),
    m_SettingsSaver(NULL)
#ifdef HAVE_PERKIN_ELMER
      ,
    m_PerkinElmerPluginInterface(NULL)
#endif
{
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

  m_AllocatorThread = QxrdAllocatorThreadPtr(new QxrdAllocatorThread());
  m_AllocatorThread -> start();
  m_Allocator = m_AllocatorThread -> allocator();

  m_FileSaverThread = QxrdFileSaverThreadPtr(new QxrdFileSaverThread(QxrdAllocatorPtr(m_Allocator)));
  m_FileSaverThread -> start();

  m_DataProcessorThread = QxrdDataProcessorThreadPtr(new QxrdDataProcessorThread(QxrdAcquisitionPtr(NULL),
                                                                                 QxrdAllocatorPtr(m_Allocator),
                                                                                 QxrdFileSaverThreadPtr(m_FileSaverThread)));

  m_DataProcessorThread -> start();
  m_DataProcessor = m_DataProcessorThread -> dataProcessor();

  m_FileSaverThread -> setProcessor(m_DataProcessor);

  m_AcquisitionThread = QxrdAcquisitionThreadPtr(new QxrdAcquisitionThread(m_DataProcessor, m_Allocator, detectorType));
  m_AcquisitionThread -> start();
  m_Acquisition = m_AcquisitionThread -> acquisition();

  m_DataProcessor -> setAcquisition(m_Acquisition);
  m_FileSaverThread -> setAcquisition(m_Acquisition);

  m_Window = QxrdWindowPtr(new QxrdWindow(QxrdApplicationPtr(this), m_Acquisition, m_DataProcessor, m_Allocator));
//  m_Window -> show();

  m_DataProcessor -> setWindow(m_Window);

  connect(this, SIGNAL(printMessage(QString)), m_Window, SLOT(printMessage(QString)));
  connect(this, SIGNAL(criticalMessage(QString)), m_Window, SLOT(criticalMessage(QString)));
  connect(this, SIGNAL(statusMessage(QString)), m_Window, SLOT(statusMessage(QString)));
//  emit printMessage("window shown");

  emit printMessage("about to load plugins");

  loadPlugins();

  m_AcquisitionThread->initialize();

  connect(m_DataProcessorThread, SIGNAL(printMessage(QString)), m_Window, SLOT(printMessage(QString)));
  connect(m_DataProcessorThread, SIGNAL(statusMessage(QString)), m_Window, SLOT(statusMessage(QString)));
  connect(m_DataProcessorThread, SIGNAL(criticalMessage(QString)), m_Window, SLOT(criticalMessage(QString)));

  connect(m_DataProcessor, SIGNAL(printMessage(QString)), m_Window, SLOT(printMessage(QString)));
  connect(m_DataProcessor -> integrator(), SIGNAL(printMessage(QString)), m_Window, SLOT(printMessage(QString)));
  connect(m_DataProcessor, SIGNAL(statusMessage(QString)), m_Window, SLOT(statusMessage(QString)));
  connect(m_DataProcessor -> integrator(), SIGNAL(statusMessage(QString)), m_Window, SLOT(statusMessage(QString)));
  connect(m_DataProcessor, SIGNAL(criticalMessage(QString)), m_Window, SLOT(criticalMessage(QString)));
  connect(m_DataProcessor -> integrator(), SIGNAL(criticalMessage(QString)), m_Window, SLOT(criticalMessage(QString)));

  connect(m_Acquisition, SIGNAL(statusMessage(QString)), m_Window, SLOT(statusMessage(QString)));
  connect(m_Acquisition, SIGNAL(printMessage(QString)), m_Window, SLOT(printMessage(QString)));
  connect(m_Acquisition, SIGNAL(criticalMessage(QString)), m_Window, SLOT(criticalMessage(QString)));

  connect(m_AllocatorThread, SIGNAL(statusMessage(QString)), m_Window, SLOT(statusMessage(QString)));
  connect(m_AllocatorThread, SIGNAL(printMessage(QString)), m_Window, SLOT(printMessage(QString)));
  connect(m_AllocatorThread, SIGNAL(criticalMessage(QString)), m_Window, SLOT(criticalMessage(QString)));

  connect(m_FileSaverThread, SIGNAL(statusMessage(QString)), m_Window, SLOT(statusMessage(QString)));
  connect(m_FileSaverThread, SIGNAL(printMessage(QString)), m_Window, SLOT(printMessage(QString)));
  connect(m_FileSaverThread, SIGNAL(criticalMessage(QString)), m_Window, SLOT(criticalMessage(QString)));

  if (specServer) {
    m_ServerThread = QxrdServerThreadPtr(new QxrdServerThread(m_AcquisitionThread, "qxrd", specServerPort));

    connect(m_ServerThread,       SIGNAL(printMessage(QString)), m_Window,            SLOT(printMessage(QString)));
    connect(m_ServerThread,       SIGNAL(statusMessage(QString)), m_Window,            SLOT(statusMessage(QString)));
    connect(m_ServerThread,       SIGNAL(criticalMessage(QString)), m_Window,            SLOT(criticalMessage(QString)));

    m_ServerThread -> start();
    m_Server = m_ServerThread -> server();

    emit printMessage(tr("Spec Server Thread started: listening on port %1").arg(m_Server->serverPort()));
  }

  if (simpleServer) {
    m_SimpleServerThread = QxrdSimpleServerThreadPtr(new QxrdSimpleServerThread(m_AcquisitionThread, "simpleserver", simpleServerPort));

    connect(m_SimpleServerThread,       SIGNAL(printMessage(QString)), m_Window,            SLOT(printMessage(QString)));
    connect(m_SimpleServerThread,       SIGNAL(statusMessage(QString)), m_Window,            SLOT(statusMessage(QString)));
    connect(m_SimpleServerThread,       SIGNAL(criticalMessage(QString)), m_Window,            SLOT(criticalMessage(QString)));

    m_SimpleServerThread -> start();
    m_SimpleServer = m_SimpleServerThread -> server();

    emit printMessage(tr("Simple Server Thread started: listening on port %1").arg(m_SimpleServer->serverPort()));
  }


  m_ScriptEngineThread = QxrdScriptEngineThreadPtr(new QxrdScriptEngineThread(QxrdApplicationPtr(this), m_Window, m_Acquisition, m_DataProcessor));
  m_ScriptEngineThread -> start();
  m_ScriptEngine = m_ScriptEngineThread -> scriptEngine();

//  m_ScriptEngineDebugger = new QScriptEngineDebugger(this);
//  m_ScriptEngineDebugger -> attachTo(m_ScriptEngine->scriptEngine());
//  m_ScriptEngineDebugger -> setAutoShowStandardWindow(true);

//  emit printMessage("script thread created");

  connect(m_ScriptEngineThread, SIGNAL(printMessage(QString)), m_Window,            SLOT(printMessage(QString)));
  connect(m_ScriptEngineThread, SIGNAL(statusMessage(QString)), m_Window,            SLOT(statusMessage(QString)));
  connect(m_ScriptEngineThread, SIGNAL(criticalMessage(QString)), m_Window,            SLOT(criticalMessage(QString)));

  connect(m_ScriptEngine,       SIGNAL(printMessage(QString)), m_Window,            SLOT(printMessage(QString)));
  connect(m_ScriptEngine,       SIGNAL(statusMessage(QString)), m_Window,            SLOT(statusMessage(QString)));
  connect(m_ScriptEngine,       SIGNAL(criticalMessage(QString)), m_Window,            SLOT(criticalMessage(QString)));

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

  readSettings();

  m_SettingsSaverThread = new QxrdSettingsSaverThread(this);

  connect(m_SettingsSaverThread, SIGNAL(printMessage(QString)), m_Window,            SLOT(printMessage(QString)));
  connect(m_SettingsSaverThread, SIGNAL(statusMessage(QString)), m_Window,            SLOT(statusMessage(QString)));
  connect(m_SettingsSaverThread, SIGNAL(criticalMessage(QString)), m_Window,            SLOT(criticalMessage(QString)));

  m_SettingsSaverThread -> start();
  m_SettingsSaver = m_SettingsSaverThread -> settingsSaver();

  emit printMessage(tr("Optimal thread count = %1").arg(QThread::idealThreadCount()));

  m_DataProcessor -> loadDefaultImages();

#ifdef Q_OS_WIN32
  QDir::setCurrent(QDir::homePath());
#endif

  emit printMessage(tr("Current directory %1").arg(QDir::currentPath()));

  m_Window -> show();
}

QxrdApplication::~QxrdApplication()
{
  QCEP_DEBUG(DEBUG_APP,
             printf("QxrdApplication::~QxrdApplication\n");
  );

  delete m_Window;
  delete m_AcquisitionThread;
  delete m_ServerThread;
  delete m_DataProcessorThread;
  delete m_FileSaverThread;
  delete m_AllocatorThread;
  delete m_ScriptEngineThread;

  QCEP_DEBUG(DEBUG_APP,
             printf("QxrdApplication::~QxrdApplication finished\n");
  );
}

QxrdApplication* QxrdApplication::application()
{
  return g_Application;
}

#ifdef HAVE_PERKIN_ELMER

QxrdPerkinElmerPluginInterface* QxrdApplication::perkinElmerPlugin()
{
  return m_PerkinElmerPluginInterface;
}

#endif

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

        emit printMessage(tr("Loaded plugin \"%1\" from %2")
                          .arg(pluginName)
                          .arg(pluginsDir.absoluteFilePath(fileName)));
      } else {
        emit printMessage(tr("Failed to load plugin %1 : %2")
                          .arg(pluginsDir.absoluteFilePath(fileName))
                          .arg(loader.errorString()));
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

QxrdWindowPtr QxrdApplication::window()
{
  return m_Window;
}

QxrdAcquisitionThreadPtr QxrdApplication::acquisitionThread()
{
  return m_AcquisitionThread;
}

QxrdAcquisitionPtr QxrdApplication::acquisition() const
{
  return m_Acquisition;
}

QxrdDataProcessorPtr QxrdApplication::dataProcessor() const
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
  emit printMessage(tr("Debug level changed from %1 to %2").arg(gCEPDebug).arg(newValue));

  gCEPDebug = newValue;
}

static void qxrdTIFFWarningHandler(const char* module, const char* fmt, va_list ap)
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
  emit criticalMessage(tr("TIFF Warning from %1 : %2").arg(module).arg(msg));
}

void QxrdApplication::tiffError(const char *module, const char *msg)
{
  emit criticalMessage(tr("TIFF Error from %1 : %2").arg(module).arg(msg));
}
