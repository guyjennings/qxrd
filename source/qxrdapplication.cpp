#define _CRT_SECURE_NO_WARNINGS

#include "qxrddebug.h"
#include "qxrdapplication.h"
#include "qxrdapplication-ptr.h"
#include "qxrdwelcomewindow.h"
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
#include "qxrdglobalsettings.h"
#include "qxrdexperimentthread.h"
#include "qxrdexperimentperkinelmeracquisition.h"
#include "qxrdexperimentperkinelmeranalysis.h"
#include "qxrdexperimentsimulatedacquisition.h"
#include "qxrdexperimentgenericanalysis.h"
#include "qxrdexperimentpilatusacquisition.h"
#include "qxrdexperimentpilatusanalysis.h"
#include "qxrdexperimentsettings.h"
#include "qxrdsettingssaver.h"
#include "qxrdsplashscreen.h"
#include "qxrdsplashscreen-ptr.h"
#include "qxrdmutexlocker.h"

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

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include <QJsonObject>
#endif

QxrdApplication *g_Application = NULL;

int eventCounter;

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
QCoreApplication::EventFilter oldEventFilter;

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
#endif

void QxrdApplication::processEventCounter()
{
  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("%1 events processed").arg(eventCounter));
  }

  eventCounter = 0;
}

QStringList QxrdApplication::makeStringListFromArgs(int argc, char **argv)
{
  QStringList res;

  for (int i=0; i<argc; i++) {
    res.append(argv[i]);
  }

  return res;
}

QxrdApplication::QxrdApplication(int &argc, char **argv) :
  QApplication(argc, argv),
  m_ObjectNamer(this, "application"),
  m_Saver(QxrdSettingsSaverPtr(
            new QxrdSettingsSaver(this))),
  m_RecentExperiments(m_Saver, this, "recentExperiments", QStringList(), "Recent Experiments"),
  m_RecentExperimentsSize(m_Saver, this,"recentExperimentsSize", 8, "Number of Recent Experiments to Remember"),
  m_CurrentExperiment(m_Saver, this, "currentExperiment", "", "Current Experiment"),
  m_CurrentDirectory(m_Saver, this, "currentDirectory", QDir::homePath(), "Current Directory"),
//  m_OpenDirectly(m_Saver, this,"openDirectly", false, "Open Last Experiment at Startup"),
  m_Debug(m_Saver, this,"debug", 0, "Debug Level"),
  m_FreshStart(QxrdSettingsSaverPtr(), this,"freshStart", 0, "Do a Fresh Start"),
  m_FileBrowserLimit(m_Saver, this, "fileBrowserLimit", 1000, "Max Number of Files in Browser Windows (0 = unlimited)"),
  m_MessageWindowLines(m_Saver, this, "messageWindowLines", 1000, "Number of Lines in Message Window (0 = unlimited)"),
  m_UpdateIntervalMsec(m_Saver, this, "updateIntervalMsec", 1000, "Time Intervale for Updates (in msec)"),
  m_Argc(QxrdSettingsSaverPtr(), this, "argc", argc, "Number of Command Line Arguments"),
  m_Argv(QxrdSettingsSaverPtr(), this, "argv", makeStringListFromArgs(argc, argv), "Command Line Arguments"),
  m_GuiWanted(QxrdSettingsSaverPtr(), this, "guiWanted", 1, "GUI Wanted?"),
  m_CmdList(QxrdSettingsSaverPtr(), this, "cmdList", QStringList(), "Commands to Execute"),
  m_FileList(QxrdSettingsSaverPtr(), this, "fileList", QStringList(), "Files to Process"),
  m_LockerCount(QxrdSettingsSaverPtr(), this, "lockerCount", 0, "Number of mutex locks taken"),
  m_LockerRate(QxrdSettingsSaverPtr(), this, "lockerRate", 0, "Mutex Locking Rate"),
  m_Splash(NULL),
  m_WelcomeWindow(NULL),
  m_AllocatorThread(NULL),
  m_Allocator(NULL),
  m_NIDAQPluginInterface(NULL),
  #ifdef HAVE_PERKIN_ELMER
  m_PerkinElmerPluginInterface(NULL),
  #endif
  m_ResponseTimer(NULL),
  m_SettingsMutex(),
  m_LastLockerCount(0)
{
#ifndef QT_NO_DEBUG
  printf("QxrdApplication::QxrdApplication(%p)\n", this);
#endif

  g_Application = this;
}

bool QxrdApplication::init(QxrdApplicationWPtr app, int &argc, char **argv)
{
  m_Application = app;

  connect(this, SIGNAL(aboutToQuit()), this, SLOT(finish()));

  connect(&m_SplashTimer, SIGNAL(timeout()), this, SLOT(hideSplash()));

  connect(&m_LockerTimer, SIGNAL(timeout()), this, SLOT(lockerTimerElapsed()));

  m_LastLockerTime.start();
  m_LockerTimer.start(5000);

  QDir::setCurrent(QDir::homePath());

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
  oldEventFilter = setEventFilter(myEventFilter);

  connect(&eventCounterTimer, SIGNAL(timeout()), this, SLOT(processEventCounter()));
  eventCounterTimer.start(10000);
#endif

  setOrganizationName("cep");
  setOrganizationDomain("xray.aps.anl.gov");
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

  connect(prop_Debug(), SIGNAL(valueChanged(int,int)), this, SLOT(debugChanged(int)));
  readSettings();

  for (int i=1; i<argc; i++) {
    int dbg=0;

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

  m_AllocatorThread = QxrdAllocatorThreadPtr(
        new QxrdAllocatorThread(m_Saver));
  m_AllocatorThread -> setObjectName("alloc");
  m_AllocatorThread -> start();
  m_Allocator = m_AllocatorThread -> allocator();

  printMessage(tr("Home Path: %1").arg(QDir::homePath()));
  printMessage(tr("Current Path: %1").arg(QDir::currentPath()));
  printMessage(tr("Root Path %1").arg(QDir::rootPath()));

  QcepProperty::registerMetaTypes();

  setupTiffHandlers();

  QThread::currentThread()->setObjectName("app");

  loadPlugins();
  readSettings();

  printMessage(tr("Optimal thread count = %1").arg(QThread::idealThreadCount()));

  m_ResponseTimer = new QxrdResponseTimer(1000, this);

  if (get_FreshStart()) {
    editGlobalPreferences();
  }

  if (/*get_OpenDirectly() &&*/ (get_CurrentExperiment().length()>0)) {
    openExperiment(get_CurrentExperiment());
  } else {
    openWelcomeWindow();
  }

  m_Saver->start();

  return true;
}

QxrdApplication::~QxrdApplication()
{
  m_Saver->performSave();

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdApplication::~QxrdApplication");
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdApplication::~QxrdApplication(%p)\n", this);
  }
}

void QxrdApplication::finish()
{
  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdApplication::finish()");
  }

  writeSettings();

  m_ExperimentThreads.clear();
  m_Experiments.clear();

  m_AllocatorThread = QxrdAllocatorThreadPtr();
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
    m_WelcomeWindow -> hide();
  }
}

QWidget* QxrdApplication::window()
{
  return m_WelcomeWindow;
}

//QxrdApplicationPtr QxrdApplication::application()
//{
//  return g_Application;
//}

#ifdef HAVE_PERKIN_ELMER

QxrdPerkinElmerPluginInterfacePtr QxrdApplication::perkinElmerPlugin()
{
  return m_PerkinElmerPluginInterface;
}

#endif

QxrdNIDAQPluginInterfacePtr QxrdApplication::nidaqPlugin()
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
#endif
  QDir pluginsDir = QDir(qApp->applicationDirPath());
  pluginsDir.cd("plugins");
  pluginsDirList.append(pluginsDir);

  foreach (QDir pluginsDir, pluginsDirList) {
    if (qcepDebug(DEBUG_PLUGINS)) {
      printf("Looking for plugins in directory %s\n", qPrintable(pluginsDir.absolutePath()));
    }

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
      QString fullPath = pluginsDir.absoluteFilePath(fileName);

      if (qcepDebug(DEBUG_PLUGINS)) {
        printf("Looking for plugin in file %s\n", qPrintable(fullPath));
      }

      if (QLibrary::isLibrary(fullPath)) {
        QPluginLoader loader(fullPath);

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
        QJsonObject meta = loader.metaData();

        if (qcepDebug(DEBUG_PLUGINS)) {
          printf("Plugin metadata for %s\n", qPrintable(fullPath));
          foreach(QString key, meta.keys()) {
            printf("Key %s = %s\n", qPrintable(key), qPrintable(meta.value(key).toString()));
          }
        }
#endif

        QObject *plugin = loader.instance();
        if (plugin) {
          if (qcepDebug(DEBUG_PLUGINS)) {
            printf("Loaded plugin from %s : type %s\n", qPrintable(fullPath), qPrintable(plugin->metaObject()->className()));
          }

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

            m_PerkinElmerPluginInterface = QxrdPerkinElmerPluginInterfacePtr(perkinElmer);
          }
#endif

          QxrdNIDAQPluginInterface *nidaq = qobject_cast<QxrdNIDAQPluginInterface*>(plugin);

          if (nidaq) {
            pluginName = nidaq -> name();

            m_NIDAQPluginInterface = QxrdNIDAQPluginInterfacePtr(nidaq);
            m_NIDAQPluginInterface -> setErrorOutput(this);
          }

          splashMessage(tr("Loaded plugin \"%1\"").arg(pluginName));

          printMessage(tr("Loaded plugin \"%1\" from %2")
                       .arg(pluginName)
                       .arg(pluginsDir.absoluteFilePath(fileName)));
        } else {
          if (qcepDebug(DEBUG_PLUGINS)) {
            printf("Failed to load plugin from %s : %s\n", qPrintable(fileName), qPrintable(loader.errorString()));
          }

          if (QLibrary::isLibrary(pluginsDir.absoluteFilePath(fileName))) {
            QString msg = tr("Failed to load plugin %1 : %2")
                .arg(pluginsDir.absoluteFilePath(fileName))
                .arg(loader.errorString());
            splashMessage(msg);
            printMessage(msg);
          }
        }
      } else {
        if (qcepDebug(DEBUG_PLUGINS)) {
          printf("File %s is not a library\n", qPrintable(fileName));
        }
      }
    }
  }
}

void QxrdApplication::splashMessage(QString msg)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "splashMessage", Q_ARG(QString, msg));
  } else {
    GUI_THREAD_CHECK;

    if (get_GuiWanted()) {
      if (m_Splash == NULL) {
        m_Splash = QxrdSplashScreenPtr(new QxrdSplashScreen(NULL));
      }

      m_Splash -> show();

      QString msgf = tr("Qxrd Version " STR(QXRD_VERSION) "\n")+msg;

      m_Splash->showMessage(msgf, Qt::AlignBottom|Qt::AlignHCenter);
      processEvents();

      m_SplashTimer.start(3000);
    }
  }
}

void QxrdApplication::hideSplash()
{
  GUI_THREAD_CHECK;

  if (m_Splash) {
    m_Splash -> hide();
  }
}

void QxrdApplication::logMessage(QString /*msg*/)
{
}

void QxrdApplication::warningMessage(QString msg, QDateTime /*ts*/)
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

    if (experiment(0)) {
      INVOKE_CHECK(QMetaObject::invokeMethod(experiment(0).data(), "printMessage", Qt::QueuedConnection, Q_ARG(QString, msg)));
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
      INVOKE_CHECK(QMetaObject::invokeMethod(window(), "displayMessage", Qt::QueuedConnection, Q_ARG(QString, message)));
    }
  }
}

void QxrdApplication::criticalMessage(QString msg, QDateTime ts)
{
  QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+msg.trimmed();

  message = message.replace("\n", " : ");

  logMessage(message);

  if (window()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(window(), "displayMessage", Qt::QueuedConnection, Q_ARG(QString, message)));
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
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_SettingsMutex);

  QxrdGlobalSettings settings(this);

  readSettings(&settings);
}

void QxrdApplication::readSettings(QSettings *settings)
{
  QcepProperty::readSettings(this, &staticMetaObject, "application", settings);

  if (m_Allocator) {
    m_Allocator->readSettings(settings, "allocator");
  }
}

void QxrdApplication::writeSettings()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_SettingsMutex);

  QxrdGlobalSettings settings(this);

  writeSettings(&settings);
}

void QxrdApplication::writeSettings(QSettings *settings)
{
  QcepProperty::writeSettings(this, &staticMetaObject, "application", settings);

  if (m_Allocator) {
    m_Allocator->writeSettings(settings, "allocator");
  }
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
  QFile::remove(path+".new");

  {
    QxrdGlobalSettings settings(path+".new", QSettings::IniFormat);

    writeSettings(&settings);
  }

  QFile::remove(path+".bak");
  QFile::rename(path, path+".bak");
  QFile::rename(path+".new", path);
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

QxrdAllocatorWPtr QxrdApplication::allocator() const
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

  about += tr("Qt Version %1\n").arg(qVersion());
  about += tr("QWT Version %1").arg(QWT_VERSION_STR);

  QMessageBox::about(NULL, "QXRD", about);
}

void QxrdApplication::doOpenQXRDWebPage()
{
  QDesktopServices::openUrl(QUrl("http://qxrd.sourceforge.net/"));
}

void QxrdApplication::editGlobalPreferences()
{
  QxrdGlobalPreferencesDialog prefs(this);

  prefs.exec();
}

void QxrdApplication::debugChanged(int newValue)
{
  if (g_DebugLevel) {
    printMessage(tr("Debug level changed from %1 to %2").arg(g_DebugLevel->debugLevel()).arg(newValue));

    g_DebugLevel->setDebugLevel(newValue);
  }
}

static void qxrdTIFFWarningHandler(const char* /*module*/, const char* /*fmt*/, va_list /*ap*/)
{
  //  char msg[100];
  //
  //  vsnprintf(msg, sizeof(msg), fmt, ap);
  //
  //  g_Application -> tiffWarning(module, msg);
}

static void qxrdTIFFErrorHandler(const char* module, const char* fmt, va_list ap)
{
  char msg[100];

  vsnprintf(msg, sizeof(msg), fmt, ap);

  if (g_Application) {
    g_Application -> tiffError(module, msg);
  }
}

void QxrdApplication::setupTiffHandlers()
{
  TIFFSetErrorHandler      (&qxrdTIFFErrorHandler);
  TIFFSetErrorHandlerExt   (NULL);
  TIFFSetWarningHandler    (&qxrdTIFFWarningHandler);
  TIFFSetWarningHandlerExt (NULL);
}

void QxrdApplication::tiffWarning(const char *module, const char *msg)
{
  if (g_Application) {
    g_Application->criticalMessage(tr("TIFF Warning from %1 : %2").arg(module).arg(msg));
  }
}

void QxrdApplication::tiffError(const char *module, const char *msg)
{
  if (g_Application) {
    g_Application->criticalMessage(tr("TIFF Error from %1 : %2").arg(module).arg(msg));
  }
}

bool QxrdApplication::event(QEvent *ev)
{
  QTime tick;
  tick.start();

  bool res = QApplication::event(ev);

  int elapsed = tick.restart();

  if (elapsed > 1000) {
    printMessage("event processing took more than 1 sec");
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

    QxrdExperimentThreadPtr experimentThread = QxrdExperimentThread::newExperiment(path, m_Application, &settings);

    QxrdExperimentPtr exp = experimentThread->experiment();

    printMessage("");
    printMessage(tr("===== Open Experiment %1").arg(path));

    openedExperiment(experimentThread);
  }
}

void QxrdApplication::closeExperiment(QxrdExperimentPtr exp)
{
  if (qcepDebug(DEBUG_APP)) {
    printf("QxrdApplication::closeExperiment(%p)\n", exp.data());
  }

  QxrdExperimentPtr expp(exp);

  if (expp) {
    QxrdExperimentThreadPtr expthrd = expp -> experimentThread();

    if (expthrd) {
      closedExperiment(expthrd);
    }
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

  settings.setValue("application/d", type);
  settings.setValue("processor/outputDirectory", path);
  settings.setValue("application/logFileName", base+".log");
  settings.setValue("processor/integratedFileName", base+".avg");
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
  QFileInfo info(path);

  if (info.exists()) {
    openExperiment(path);
  } else {
    printMessage(tr("Experiment %1 does not exist").arg(path));
  }
}

void QxrdApplication::openedExperiment(QxrdExperimentThreadPtr expthrdw)
{
  QxrdExperimentThreadPtr expthrd(expthrdw);

  if (expthrd) {
    QxrdExperimentPtr expt = expthrd->experiment();

    if (expt) {
      QString path = expt->experimentFilePath();
      set_CurrentExperiment(path);
      appendRecentExperiment(path);

      m_ExperimentThreads.append(expthrd);
      m_Experiments.append(expt);

      printMessage("");
      printMessage("New experiment loaded");
      printMessage("");

      closeWelcomeWindow();

      expt->openWindows();
    }
  }
}

void QxrdApplication::closedExperiment(QxrdExperimentThreadPtr expthrdw)
{
  QxrdExperimentThreadPtr expthrd(expthrdw);

  if (expthrd) {
    QxrdExperimentPtr expt = expthrd->experiment();

    m_ExperimentThreads.removeAll(expthrd);
    m_Experiments.removeAll(expt);
  }
}

QList<QxrdExperimentPtr> &QxrdApplication::experiments()
{
  return m_Experiments;
}

QxrdExperimentPtr QxrdApplication::experiment(int i)
{
  return m_Experiments.value(i);
}

void QxrdApplication::activateExperiment(QString path)
{
  foreach(QxrdExperimentPtr exp, m_Experiments) {
    if (exp->experimentFilePath() == path) {
      QxrdWindowPtr win = exp->window();

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
    QxrdExperimentThreadPtr docThread = QxrdExperimentThread::newExperimentPerkinElmerAcquisition(normalizeExperimentName(newExperiment), m_Application);

    docThread->init(docThread);

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
    QxrdExperimentThreadPtr docThread = QxrdExperimentThread::newExperimentPilatusAcquisition(normalizeExperimentName(newExperiment), m_Application);

    docThread->init(docThread);

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
    QxrdExperimentThreadPtr docThread = QxrdExperimentThread::newExperimentSimulatedAcquisition(normalizeExperimentName(newExperiment), m_Application);

    docThread->init(docThread);

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
    QxrdExperimentThreadPtr docThread = QxrdExperimentThread::newExperimentPerkinElmerAnalysis(normalizeExperimentName(newExperiment), m_Application);

    docThread->init(docThread);

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
    QxrdExperimentThreadPtr docThread = QxrdExperimentThread::newExperimentPilatusAnalysis(normalizeExperimentName(newExperiment), m_Application);

    docThread->init(docThread);

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
    QxrdExperimentThreadPtr docThread = QxrdExperimentThread::newExperimentGenericAnalysis(normalizeExperimentName(newExperiment), m_Application);

    docThread->init(docThread);

    openedExperiment(docThread);
  }
}

void QxrdApplication::incLockerCount()
{
  prop_LockerCount()->incValue(1);
}

void QxrdApplication::lockerTimerElapsed()
{
  double elapsed = m_LastLockerTime.restart();

  double rate = 1000.0*(get_LockerCount() - m_LastLockerCount)/elapsed;

//  set_LockerRate(rate);

//  m_LastLockerTime = QTime::currentTime();
  m_LastLockerCount= get_LockerCount();

  if (rate>10000) {
    printMessage(tr("Locker rate %1 locks/sec").arg(/*get_LockerRate()*/rate));
  }
}
