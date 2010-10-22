/******************************************************************
*
*  $Id: qxrdapplication.cpp,v 1.10 2010/10/22 21:44:26 jennings Exp $
*
*******************************************************************/

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
    m_SettingsSaver(NULL),
#ifdef HAVE_PERKIN_ELMER
    m_PerkinElmerPluginInterface(NULL),
#endif
    SOURCE_IDENT("$Id: qxrdapplication.cpp,v 1.10 2010/10/22 21:44:26 jennings Exp $")
{
  setupTiffHandlers();

//  QcepProperty::dumpMetaData(&QxrdApplication::staticMetaObject);
//  QcepProperty::dumpMetaData(&QxrdWindow::staticMetaObject);
//  QcepProperty::dumpMetaData(&QxrdAcquisition::staticMetaObject);
//  QcepProperty::dumpMetaData(&QxrdScriptEngine::staticMetaObject);

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

/******************************************************************
*
*  $Log: qxrdapplication.cpp,v $
*  Revision 1.10  2010/10/22 21:44:26  jennings
*  *** empty log message ***
*
*  Revision 1.9  2010/10/21 19:44:03  jennings
*  Adding code to display overflow pixels, removed cuda and simple processors
*
*  Revision 1.8  2010/10/21 16:31:24  jennings
*  Implemented saving of settings soon after they change, rather than at program exit
*
*  Revision 1.7  2010/10/06 20:29:00  jennings
*  Added processor.fileName property, set default detector type to PE
*
*  Revision 1.6  2010/09/24 22:29:37  jennings
*  Work on NSIS installer
*  Fixed startup problem on debug builds when calling QxrdAcquisitionThread->initialize()
*
*  Revision 1.5  2010/09/24 04:31:50  jennings
*  *** empty log message ***
*
*  Revision 1.4  2010/09/23 19:57:32  jennings
*  Modified plugins for perkin elmer - now works in 64 bit mode
*
*  Revision 1.3  2010/09/17 23:12:18  jennings
*  Display port numbers when servers start up
*  Rearrange help files
*
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.6  2010/08/19 22:02:02  jennings
*  Parameterized plugin directory via QXRD_PLUGIN_DIR variable
*
*  Revision 1.1.2.5  2010/08/04 20:22:58  jennings
*  Added simple socket server, and prefs to control which servers are run, and on which ports
*
*  Revision 1.1.2.4  2010/08/02 21:09:29  jennings
*  Disabled script debugger
*
*  Revision 1.1.2.3  2010/07/23 19:20:24  jennings
*  Plugins now working
*
*  Revision 1.1.2.2  2010/07/22 23:20:58  jennings
*  Attempting to load plugins at startup
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.52.4.25  2010/07/21 21:32:18  jennings
*  Improved error message display from file saving routines
*
*  Revision 1.52.4.24  2010/07/20 20:30:25  jennings
*  Added memory usage display to status bar
*  Improved calculation of processing timings
*
*  Revision 1.52.4.23  2010/06/23 21:48:22  jennings
*  Connected printing signals from allocator and file saver
*
*  Revision 1.52.4.22  2010/06/18 16:30:00  jennings
*  Fixed (minor) memory leak by deleting allocator, file saver and script engine threads at app exit
*
*  Revision 1.52.4.21  2010/06/15 20:18:53  jennings
*  First steps to adding CUDA support
*
*  Revision 1.52.4.20  2010/06/09 19:19:13  jennings
*  Removed references to the QxrdFileSaver - all public accesses now through QxrdFileSaverThread
*
*  Revision 1.52.4.19  2010/05/25 22:03:41  jennings
*  Moved all printMessage etc. signal connection into QxrdApplication
*
*  Revision 1.52.4.18  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.52.4.17  2010/05/22 04:28:53  jennings
*  *** empty log message ***
*
*  Revision 1.52.4.16  2010/05/21 18:30:32  jennings
*  Implemented preferences dialog allowing choise of detector type, processor type and debug level
*
*  Revision 1.52.4.15  2010/05/20 20:17:57  jennings
*  Added Allocator and FileSaver threads
*
*  Revision 1.52.4.14  2010/05/19 21:15:34  jennings
*  PE acquisition debugging
*
*  Revision 1.52.4.13  2010/05/17 21:06:32  jennings
*  Various PE related mods
*
*  Revision 1.52.4.12  2010/05/17 15:59:52  jennings
*  Changed debugging output to use QCEP_DEBUG macro
*
*  Revision 1.52.4.11  2010/05/09 13:23:05  jennings
*  Fixed image display bug caused by incomplete implementation of QxrdRasterData::copy
*
*  Revision 1.52.4.10  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.52.4.9  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.52.4.8  2010/04/27 19:37:51  jennings
*  Explicitly delete some data no longer protected by QSharedPointer s
*
*  Revision 1.52.4.7  2010/04/27 01:31:26  jennings
*  Starting to get QSharedPointer working
*
*  Revision 1.52.4.6  2010/04/26 23:46:12  jennings
*  *** empty log message ***
*
*  Revision 1.52.4.5  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.52.4.4  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.52.4.3  2010/04/21 20:50:41  jennings
*  Modified thread initialization so that objects are created from run method to get correct thread affinity.
*
*  Revision 1.52.4.2  2010/04/21 19:51:06  jennings
*  Added application preferences to choose what type of detector and what type of processor to use
*
*  Revision 1.52.4.1  2010/04/21 17:00:22  jennings
*  The QxrdAcquisitionThread::acquisition method now waits until the acquisition object has been created - therefore
*  rearrange the order of statements to call QxrdAcquisitionThread->start before the call to acquisition in order to
*  avoid a deadlock.
*
*  Revision 1.52  2009/10/21 21:32:20  jennings
*  Made default working directory home directory (on windows only)
*
*  Revision 1.51  2009/10/21 21:28:30  jennings
*  Made default working directory home directory
*
*  Revision 1.50  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.49  2009/09/10 21:33:30  jennings
*  Added TIFF error handling
*
*  Revision 1.48  2009/09/07 21:56:55  jennings
*  Load previous dark, gain, bad pixels and mas images at startup
*
*  Revision 1.47  2009/08/27 21:02:17  jennings
*  Partial implementation of lazy plotting
*
*  Revision 1.46  2009/08/04 22:03:31  jennings
*  Moved integration code into QxrdIntegrator, added oversampling option
*  Add each integration result to the az-avg plot panel
*
*  Revision 1.45  2009/08/03 20:58:59  jennings
*  Minor fixups
*
*  Revision 1.44  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.43  2009/07/17 12:41:33  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.42  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.41  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.40  2009/06/30 21:34:41  jennings
*  Debugging meta data static init problems
*
*  Revision 1.39  2009/06/28 11:21:58  jennings
*  Implemented app scripting engine connections
*
*  Revision 1.38  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.37  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

