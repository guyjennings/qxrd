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
#include "qxrdglobalpreferencesdialog.h"
#include "qcepproperty.h"
#include "qxrddetectorplugininterface.h"
#include "qxrdprocessorinterface.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdfreshstartdialog.h"
#include "qxrdglobalsettings.h"
#include "qxrdexperimentthread.h"
#include "qxrdexperimentperkinelmeracquisition.h"
#include "qxrdexperimentperkinelmeranalysis.h"
#include "qxrdexperimentsimulatedacquisition.h"
#include "qxrdexperimentgenericanalysis.h"
#include "qxrdexperimentpilatusacquisition.h"
#include "qxrdexperimentpilatusanalysis.h"
#include "qxrdexperimentsettings.h"

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
#include <QDesktopServices>
#include <QUrl>
#include <QCoreApplication>

int gCEPDebug = 0;
QxrdApplication *g_Application = 0;

QCoreApplication::EventFilter oldEventFilter;

int eventCounter;

static bool myEventFilter(void *message, long *result)
{
  eventCounter++;

  if (oldEventFilter) {
    return oldEventFilter(message, result);
  } else {
    return false;
  }
}

QTimer eventCounterTimer;

void QxrdApplication::processEventCounter()
{
  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("%1 events processed").arg(eventCounter));
  }

  eventCounter = 0;
}

QStringList QxrdApplication::makeStringList(int argc, char **argv)
{
  QStringList res;

  for (int i=0; i<argc; i++) {
    res.append(argv[i]);
  }

  return res;
}

QxrdApplication::QxrdApplication(int &argc, char **argv)
  : QApplication(argc, argv),
    m_Saver(NULL, this),
    m_RecentExperiments(&m_Saver, this, "recentExperiments", QStringList()),
    m_RecentExperimentsSize(&m_Saver, this,"recentExperimentsSize", 8),
    m_CurrentExperiment(&m_Saver, this, "currentExperiment", ""),
    m_CurrentDirectory(&m_Saver, this, "currentDirectory", QDir::homePath()),
    m_OpenDirectly(&m_Saver, this,"openDirectly", false),
    m_Debug(&m_Saver, this,"debug", 0),
    m_FreshStart(NULL, this,"freshStart", 0),
    m_FileBrowserLimit(&m_Saver, this, "fileBrowserLimit", 0),
    m_MessageWindowLines(&m_Saver, this, "messageWindowLines", 1000),
    m_UpdateIntervalMsec(&m_Saver, this, "updateIntervalMsec", 1000),
    m_Argc(NULL, this, "argc", argc),
    m_Argv(NULL, this, "argv", makeStringList(argc, argv)),
    m_GuiWanted(NULL, this, "guiWanted", 1),
    m_CmdList(NULL, this, "cmdList", QStringList()),
    m_FileList(NULL, this, "fileList", QStringList()),
    m_WelcomeWindow(NULL),
    m_AllocatorThread(NULL),
    m_Allocator(NULL),
    m_ScriptEngineThread(NULL),
    m_ScriptEngine(NULL),
    m_ScriptEngineDebugger(NULL),
//    m_SettingsSaverThread(NULL),
//    m_SettingsSaver(NULL),
    #ifdef HAVE_PERKIN_ELMER
        m_PerkinElmerPluginInterface(NULL),
    #endif
    m_NIDAQPluginInterface(NULL),
    m_ResponseTimer(NULL)
{
  QDir::setCurrent(QDir::homePath());

  oldEventFilter = setEventFilter(myEventFilter);

  connect(&eventCounterTimer, SIGNAL(timeout()), this, SLOT(processEventCounter()));
  eventCounterTimer.start(10000);

  setOrganizationName("cep");
  setOrganizationDomain("xor.aps.anl.gov");
  setApplicationName("qxrd");



  printMessage("------ Starting QXRD Application ------");

  QString about = "QXRD Version " STR(QXRD_VERSION);

  if (sizeof(void*) == 4) {
    about += " - 32 Bit";
  } else {
    about += " - 64 Bit";
  }

#ifdef Q_CC_MSVC
  about += " MSVC";
#endif

#ifdef Q_CC_GNU
  about += " gcc";
#endif

#ifdef QT_NO_DEBUG
  about += " Release\n";
#else
  about += " Debug\n";
#endif

  printMessage(about);
  printMessage("QWT Version " QWT_VERSION_STR);
  printMessage(tr("QT Version %1").arg(qVersion()));

  readSettings();

  //  printf("Argc = %d\n", argc);

  for (int i=1; i<argc; i++) {
    int dbg=0;
//    printf("Arg %d = %s\n", i, argv[i]);

    if (strcmp(argv[i],"-fresh") == 0) {
      set_FreshStart(true);
    } else if (sscanf(argv[i],"-debug=%d",&dbg)==1) {
      set_Debug(dbg);
    } else if (strcmp(argv[i],"-nogui")==0) {
      set_GuiWanted(false);
    } else if (strcmp(argv[i],"-gui")==0) {
      set_GuiWanted(true);
    } else if (strcmp(argv[i],"-c")==0) {
      if (i++ < argc) {
        prop_CmdList()->appendValue(argv[i]);
      }
    } else if (strcmp(argv[i],"-s")==0) {
      if (i++ < argc) {
        prop_CmdList()->appendValue(tr("loadScript(\"%1\")").arg(argv[i]));
      }
    } else {
      prop_FileList()->appendValue(argv[i]);
    }
  }

  if (get_GuiWanted() == false) {
    foreach(QString cmd, get_CmdList()) {
      printf("Cmd: %s\n", qPrintable(cmd));
    }

    foreach(QString file, get_FileList()) {
      printf("File: %s\n", qPrintable(file));
    }
  }

  m_AllocatorThread = new QxrdAllocatorThread(&m_Saver);
  m_AllocatorThread -> setObjectName("alloc");
  m_AllocatorThread -> start();
  m_Allocator = m_AllocatorThread -> allocator();

  printMessage(tr("Home Path: %1").arg(QDir::homePath()));
  printMessage(tr("Current Path: %1").arg(QDir::currentPath()));
  printMessage(tr("Root Path %1").arg(QDir::rootPath()));

  connect(this, SIGNAL(aboutToQuit()), this, SLOT(shutdownThreads()));
}

bool QxrdApplication::init(QSplashScreen *splash)
{
  QcepProperty::registerMetaTypes();

  setupTiffHandlers();

  setObjectName("qxrdapplication");

  QThread::currentThread()->setObjectName("app");
//  printf("application thread %p\n", thread());

  g_Application = this;

  loadPlugins();
  readSettings();

  splashMessage("Qxrd Version " STR(QXRD_VERSION) "\nStarting Scripting System");

  m_ScriptEngineThread = new QxrdScriptEngineThread(this);
  m_ScriptEngineThread -> setObjectName("script");
  m_ScriptEngineThread -> start();
  m_ScriptEngine = m_ScriptEngineThread -> scriptEngine();

//  m_ScriptEngineDebugger = new QScriptEngineDebugger(this);
//  m_ScriptEngineDebugger -> attachTo(m_ScriptEngine->scriptEngine());
//  m_ScriptEngineDebugger -> setAutoShowStandardWindow(true);

  connect(prop_Debug(), SIGNAL(valueChanged(int,int)), this, SLOT(debugChanged(int)));

//  m_SettingsSaverThread = new QxrdSettingsSaverThread(this);
//  m_SettingsSaverThread -> setObjectName("settings");
//  m_SettingsSaverThread -> start();
//  m_SettingsSaver = m_SettingsSaverThread -> settingsSaver();

  printMessage(tr("Optimal thread count = %1").arg(QThread::idealThreadCount()));

  m_ResponseTimer = new QxrdResponseTimer(1000, this);

  if (get_FreshStart()) {
    QxrdFreshStartDialog *fresh = new QxrdFreshStartDialog();

    fresh->exec();
  }

  if (get_OpenDirectly() && (get_CurrentExperiment().length()>0)) {
    openExperiment(get_CurrentExperiment());
  } else {
    openWelcomeWindow();
  }

  return true;
}

QxrdApplication::~QxrdApplication()
{
  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdApplication::~QxrdApplication");
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

//  writeDefaultSettings();

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdApplication::~QxrdApplication finished");
  }
}

void QxrdApplication::openWelcomeWindow()
{
  GUI_THREAD_CHECK;

  if (m_WelcomeWindow == NULL) {
    m_WelcomeWindow = new QxrdWelcomeWindow(this);
    m_WelcomeWindow->setAttribute(Qt::WA_DeleteOnClose, true);
  }

  m_WelcomeWindow -> show();
}

void QxrdApplication::closeWelcomeWindow()
{
  GUI_THREAD_CHECK;

  if (m_WelcomeWindow) {
    m_WelcomeWindow -> close();

    m_WelcomeWindow = NULL;
  }
}

QxrdScriptEngine* QxrdApplication::scriptEngine()
{
  return m_ScriptEngine;
}

QWidget* QxrdApplication::window()
{
//  if (m_WelcomeWindow == NULL) {
//    openWelcomeWindow();
//  }

  return m_WelcomeWindow;
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

        splashMessage(tr("Qxrd Version " STR(QXRD_VERSION) "\nLoaded plugin \"%1\"").arg(pluginName));

        g_Application->printMessage(tr("Loaded plugin \"%1\" from %2")
                          .arg(pluginName)
                          .arg(pluginsDir.absoluteFilePath(fileName)));
      } else {

        if (QLibrary::isLibrary(pluginsDir.absoluteFilePath(fileName))) {
          QString msg = tr("Qxrd Version " STR(QXRD_VERSION) "\nFailed to load plugin %1 : %2")
              .arg(pluginsDir.absoluteFilePath(fileName))
              .arg(loader.errorString());
          splashMessage(msg);
          g_Application->printMessage(msg);
        }
      }
    }
  }
}

QString QxrdApplication::hexArg(void *p)
{
#if (QT_POINTER_SIZE==4)
    return tr("0x%1").arg((quint32)p, 8, 16, QLatin1Char('0'));
#else
    return tr("0x%1").arg((quint64)p, 16, 16, QLatin1Char('0'));
#endif
}

void QxrdApplication::splashMessage(QString msg)
{
}

void QxrdApplication::logMessage(QString msg)
{
}

void QxrdApplication::warningMessage(QString msg, QDateTime ts)
{
  if (window()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(window(), "warningMessage", Qt::BlockingQueuedConnection, Q_ARG(QString, msg)));
  }
}

void QxrdApplication::printMessage(QString msg, QDateTime ts)
{
  if (qcepDebug(DEBUG_NOMESSAGES)) {
  } else {
    QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+
        QThread::currentThread()->objectName()+": "+
        msg.trimmed();

    message = message.replace("\n", " : ");

    logMessage(message);

    if (window()) {
      INVOKE_CHECK(QMetaObject::invokeMethod(window(), "displayMessage", Qt::QueuedConnection, Q_ARG(QString, message)));
    }
  }
}

void QxrdApplication::statusMessage(QString msg, QDateTime ts)
{
  if (qcepDebug(DEBUG_NOMESSAGES)) {
  } else {
    QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+msg.trimmed();

    message = message.replace("\n", " : ");

    logMessage(message);

    if (window()) {
      INVOKE_CHECK(QMetaObject::invokeMethod(window(), "displayStatusMessage", Qt::QueuedConnection, Q_ARG(QString, message)));
    }
  }
}

void QxrdApplication::criticalMessage(QString msg, QDateTime ts)
{
  QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+msg.trimmed();

  message = message.replace("\n", " : ");

  logMessage(message);

  if (window()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(window(), "displayCriticalMessage", Qt::QueuedConnection, Q_ARG(QString, message)));
  }
}

QString QxrdApplication::homePath()
{
  return QDir::homePath();
}

QString QxrdApplication::currentPath()
{
  return QDir::currentPath();
}

QString QxrdApplication::rootPath()
{
  return QDir::rootPath();
}

void QxrdApplication::readSettings()
{
  QxrdGlobalSettings settings(this);

  readSettings(&settings);
}

void QxrdApplication::readSettings(QSettings *settings)
{
  QcepProperty::readSettings(this, &staticMetaObject, "application", settings);
}

void QxrdApplication::writeSettings()
{
  QxrdGlobalSettings settings(this);

  writeSettings(&settings);
}

void QxrdApplication::writeSettings(QSettings *settings)
{
  QcepProperty::writeSettings(this, &staticMetaObject, "application", settings);
}

void QxrdApplication::doLoadPreferences()
{
  QString loadPrefsFrom = QFileDialog::getOpenFileName(NULL,
                                                       "Load QXRD Preferences from...");

  if (loadPrefsFrom != "") {
    loadPreferences(loadPrefsFrom);
  }
}

void QxrdApplication::loadPreferences(QString path)
{
  QxrdGlobalSettings settings(path, QSettings::IniFormat);

  readSettings(&settings);
}

void QxrdApplication::doSavePreferences()
{
  QString savePrefsTo = QFileDialog::getSaveFileName(NULL,
                                                     "Save QXRD Preferences to...");

  if (savePrefsTo != "") {
    savePreferences(savePrefsTo);
  }
}

void QxrdApplication::savePreferences(QString path)
{
  QxrdGlobalSettings settings(path, QSettings::IniFormat);

  writeSettings(&settings);
}

void QxrdApplication::executeCommand(QString cmd)
{
  m_ScriptEngine->evaluateAppCommand(cmd);
}

void QxrdApplication::possiblyQuit()
{
  if (wantToQuit()) {
    quit();
  }
}

bool QxrdApplication::wantToQuit()
{
  return QMessageBox::question(NULL, tr("Really Quit?"),
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
    g_Application->printMessage("QxrdApplication::shutdownThreads");
  }

  writeSettings();

//  shutdownThread(m_SettingsSaverThread);
//  shutdownThread(m_SimpleServerThread);
//  shutdownThread(m_ServerThread);
  shutdownThread(m_ScriptEngineThread);
//  shutdownThread(m_AcquisitionThread);
//  shutdownThread(m_DataProcessorThread);
//  shutdownThread(m_AllocatorThread);
//  shutdownThread(m_FileSaverThread);

  foreach(QxrdExperimentThreadPtr exp, m_ExperimentThreads) {
    shutdownThread(exp/*.data()*/);
  }
}

QxrdAllocator *QxrdApplication::allocator() const
{
  return m_Allocator;
}

void QxrdApplication::doAboutQxrd()
{
  QString about = "QXRD Data Acquisition for PE Area Detectors\nVersion " STR(QXRD_VERSION);

  if (sizeof(void*) == 4) {
    about += " - 32 Bit";
  } else {
    about += " - 64 Bit";
  }

#ifdef Q_CC_MSVC
  about += " MSVC";
#endif

#ifdef Q_CC_GNU
  about += " gcc";
#endif

#ifdef QT_NO_DEBUG
  about += " Release\n";
#else
  about += " Debug\n";
#endif

  about += tr("Qt Version %1").arg(qVersion());

  QMessageBox::about(NULL, "QXRD", about);
}

void QxrdApplication::doOpenQXRDWebPage()
{
  QDesktopServices::openUrl(QUrl("http://qxrd.sourceforge.net/"));
}

void QxrdApplication::editGlobalPreferences()
{
  QxrdGlobalPreferencesDialog prefs;

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

void QxrdApplication::readDefaultSettings()
{
  QSettings settings("cep.xor.aps.anl.gov", "qxrd-defaults");

  set_RecentExperiments(settings.value("recentExperiments").toStringList());
  set_RecentExperimentsSize(settings.value("recentExperimentsSize", 8).toInt());
  set_CurrentExperiment(settings.value("currentExperiment").toString());
}

void QxrdApplication::writeDefaultSettings()
{
  QSettings settings("cep.xor.aps.anl.gov", "qxrd-defaults");

  settings.setValue("recentExperiments", get_RecentExperiments());
  settings.setValue("recentExperimentsSize", get_RecentExperimentsSize());
  settings.setValue("currentExperiment", get_CurrentExperiment());
}

void QxrdApplication::chooseNewExperiment()
{
  openWelcomeWindow();
//  QxrdNewExperimentDialog *chooser =  new QxrdNewExperimentDialog(this);

//  if (chooser->choose()) {
//    openExperiment(chooser->chosenPath());
//  }
}

void QxrdApplication::chooseExistingExperiment()
{
  QString res = QFileDialog::getOpenFileName(NULL,
                                             "Open an existing experiment...",
                                             get_CurrentExperiment(),
                                             "QXRD Experiments (*.qxrdp);;Other Files (*)");

  if (res.length() > 0) {
    openExperiment(res);
  }
}

void QxrdApplication::openExperiment(QString path)
{
  if (path.length() > 0) {
    QxrdExperimentSettings settings(path);

    QxrdExperimentThreadPtr experiment = QxrdExperimentThread::newExperiment(path, this, &settings);

    printMessage("");
    printMessage(tr("===== Open Experiment %1").arg(path));

    appendRecentExperiment(path);

    openedExperiment(experiment);

    printMessage("");
    printMessage("New experiment loaded");
    printMessage("");

    closeWelcomeWindow();
  }
}

void QxrdApplication::appendRecentExperiment(QString path)
{
  QStringList recent = get_RecentExperiments();

  recent.prepend(path);
  recent.removeDuplicates();

  while(recent.length() > get_RecentExperimentsSize()) {
    recent.removeLast();
  }

  set_RecentExperiments(recent);
}

QString QxrdApplication::normalizeExperimentName(QString filename)
{
  QFileInfo info(filename);
  QString exten = info.suffix();

  if (exten != "qxrdp") {
    return filename+".qxrdp";
  } else {
    return filename;
  }
}

void QxrdApplication::setNewExperimentSettings(QSettings &settings, int type, QString filename)
{
  QFileInfo info(filename);
  QString  path = info.path();
  QString  base = info.completeBaseName();

  settings.setValue("application/detectorType", type);
  settings.setValue("processor/outputDirectory", path);
  settings.setValue("application/logFilePath", base+".log");
  settings.setValue("processor/integratedFilePath", base+".avg");
  settings.setValue("window/inputFileBrowser/rootDirectory", path);
  settings.setValue("window/outputFileBrowser/rootDirectory", path);

  switch(type) {
  case 1: // Perkin Elmer
    break;

  case 2: // Pilatus
    break;

  case 4: // Analysis
    settings.setValue("application/runSpecServer", false);
    settings.setValue("application/runSimpleServer", false);
    break;
  }
}

QString QxrdApplication::newAnalysisExperiment(QString path)
{
  path = normalizeExperimentName(path);

  if (path.length() > 0) {
    savePreferences(path);

    QSettings settings(path, QSettings::IniFormat);

    setNewExperimentSettings(settings, 4, path);

    return path;
  } else {
    return QString();
  }
}

QString QxrdApplication::newPerkinElmerExperiment(QString path)
{
  path = normalizeExperimentName(path);

  if (path.length() > 0) {
    savePreferences(path);

    QSettings settings(path, QSettings::IniFormat);

    setNewExperimentSettings(settings, 1, path);

    return path;
  } else {
    return QString();
  }
}

QString QxrdApplication::newPilatusExperiment(QString path)
{
  path = normalizeExperimentName(path);

  if (path.length() > 0) {
    savePreferences(path);

    QSettings settings(path, QSettings::IniFormat);

    setNewExperimentSettings(settings, 2, path);

    return path;
  } else {
    return QString();
  }
}

void QxrdApplication::openRecentExperiment(QString path)
{
//  printMessage(tr("Attempt to open experiment %1").arg(path));

  QFileInfo info(path);

  if (info.exists()) {
    openExperiment(path);
  } else {
    printMessage(tr("Experiment %1 does not exist").arg(path));
  }
}

void QxrdApplication::openedExperiment(QxrdExperimentThread *expthrd)
{
  if (expthrd) {
    QxrdExperiment *expt = expthrd->experiment();

    QString path = expt->experimentFilePath();
    set_CurrentExperiment(path);
    appendRecentExperiment(path);

    m_ExperimentThreads.append(expthrd);
    m_Experiments.append(expt);

    m_ScriptEngine->experimentOpened(expt);

    if (expt->window()) {
        m_ScriptEngine->windowOpened(expt->window());
    }
//    connect(expthrd, SIGNAL(destroyed(QObject*)), this, SLOT(closedExperiment(QObject*)), Qt::BlockingQueuedConnection);
  }
}

void QxrdApplication::closedExperiment(QObject *obj)
{
  QxrdExperimentThreadPtr expthrd = qobject_cast<QxrdExperimentThreadPtr>(obj);

  if (expthrd) {
    QxrdExperiment *expt = expthrd->experiment();

    m_ExperimentThreads.removeAll(expthrd);
    m_Experiments.removeAll(expt);

    m_ScriptEngine->experimentClosed(expt);

    if (expt->window()) {
        m_ScriptEngine->windowClosed(expt->window());
    }
  }
}

QList<QxrdExperiment*> &QxrdApplication::experiments()
{
  return m_Experiments;
}

QxrdExperiment *QxrdApplication::experiment(int i)
{
    return m_Experiments.value(i);
}

void QxrdApplication::activateExperiment(QString path)
{
  foreach(QxrdExperiment* exp, m_Experiments) {
    if (exp->experimentFilePath() == path) {
      QxrdWindow* win = exp->window();

      if (win) {
        win->activateWindow();
      }
    }
  }
}

void QxrdApplication::doNewPerkinElmerAcquisition()
{
  QString newExperiment = QFileDialog::getSaveFileName(NULL,
                                                       "New Perkin Elmer Experiment",
                                                       get_CurrentExperiment(),
                                                       "QXRD Experiments (*.qxrdp)");

  if (newExperiment.length() >= 1) {
    QxrdExperimentThreadPtr docThread = QxrdExperimentThread::newExperimentPerkinElmerAcquisition(normalizeExperimentName(newExperiment), this);

    openedExperiment(docThread);
  }
}

void QxrdApplication::doNewPilatusAcquisition()
{
  QString newExperiment = QFileDialog::getSaveFileName(NULL,
                                                       "New Pilatus Experiment",
                                                       get_CurrentExperiment(),
                                                       "QXRD Experiments (*.qxrdp)");

  if (newExperiment.length() >= 1) {
    QxrdExperimentThreadPtr docThread = QxrdExperimentThread::newExperimentPilatusAcquisition(normalizeExperimentName(newExperiment), this);

    openedExperiment(docThread);
  }
}

void QxrdApplication::doNewSimulatedAcquisition()
{
  QString newExperiment = QFileDialog::getSaveFileName(NULL,
                                                       "New Simulated Acquisition Experiment",
                                                       get_CurrentExperiment(),
                                                       "QXRD Experiments (*.qxrdp)");

  if (newExperiment.length() >= 1) {
    QxrdExperimentThreadPtr docThread = QxrdExperimentThread::newExperimentSimulatedAcquisition(normalizeExperimentName(newExperiment), this);

    openedExperiment(docThread);
  }
}

void QxrdApplication::doNewPerkinElmerAnalysis()
{
  QString newExperiment = QFileDialog::getSaveFileName(NULL,
                                                       "New Perkin Elmer Analysis Experiment",
                                                       get_CurrentExperiment(),
                                                       "QXRD Experiments (*.qxrdp)");

  if (newExperiment.length() >= 1) {
    QxrdExperimentThreadPtr docThread = QxrdExperimentThread::newExperimentPerkinElmerAnalysis(normalizeExperimentName(newExperiment), this);

    openedExperiment(docThread);
  }
}

void QxrdApplication::doNewPilatusAnalysis()
{
  QString newExperiment = QFileDialog::getSaveFileName(NULL,
                                                       "New Pilatus Analysis Experiment",
                                                       get_CurrentExperiment(),
                                                       "QXRD Experiments (*.qxrdp)");

  if (newExperiment.length() >= 1) {
    QxrdExperimentThreadPtr docThread = QxrdExperimentThread::newExperimentPilatusAnalysis(normalizeExperimentName(newExperiment), this);

    openedExperiment(docThread);
  }
}

void QxrdApplication::doNewGenericAnalysis()
{
  QString newExperiment = QFileDialog::getSaveFileName(NULL,
                                                       "New Generic Analysis Experiment",
                                                       get_CurrentExperiment(),
                                                       "QXRD Experiments (*.qxrdp)");

  if (newExperiment.length() >= 1) {
    QxrdExperimentThreadPtr docThread = QxrdExperimentThread::newExperimentGenericAnalysis(normalizeExperimentName(newExperiment), this);

    openedExperiment(docThread);
  }
}
