#define _CRT_SECURE_NO_WARNINGS

#include "qxrddebug.h"
#include "qxrdapplication.h"
#include "qxrdapplication-ptr.h"
#include "qxrdexperimentthread.h"
#include "qxrdwelcomewindow.h"
#include "qxrdwindow.h"
#include "qxrdserverthread.h"
#include "qxrdserver.h"
#include "qxrdsimpleserverthread.h"
#include "qxrdsimpleserver.h"
#include "qxrddataprocessorthread.h"
#include "qxrddataprocessor.h"
#include "qxrdintegrator.h"
#include "qxrdacquisition.h"
#include "qcepallocatorthread.h"
#include "qcepallocator.h"
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
#include "qxrdexperiment.h"
#include "qxrdexperimentsettings.h"
#include "qxrdsplashscreen.h"
#include "qxrdsplashscreen-ptr.h"
#include "qcepmutexlocker.h"
#include "qxrdcalibrant.h"
#include "qxrdcalibrantlibrary.h"
#include "qxrdapplicationsettings.h"
#include "qxrdwindowsettings.h"
#include "qxrddetectorcontrolwindowsettings.h"
#include "qxrdtestgenerator.h"
#include "qcepimagedataformattiff.h"
#include "qxrdplugininfomodel.h"

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
#include <QJsonObject>

static QList<QDir> pluginsDirList;

int eventCounter;

void QxrdApplication::processEventCounter()
{
  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("%1 events processed").arg(eventCounter));
  }

  eventCounter = 0;
}

QxrdApplication::QxrdApplication(int &argc, char **argv) :
  QcepApplication(argc, argv),
  m_ObjectNamer(this, "application"),
//  m_AppSaver(QcepSettingsSaverPtr(
//            new QcepSettingsSaver(this))),
  m_Splash(NULL),
  m_WelcomeWindow(NULL),
  m_NIDAQPlugin(NULL),
  #ifdef HAVE_PERKIN_ELMER
  m_PerkinElmerPluginInterface(NULL),
  #endif
  m_ResponseTimer(NULL),
  m_SettingsMutex(),
  m_LastLockerCount(0)
{
#ifndef QT_NO_DEBUG
  printf("Constructing application\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdApplication::QxrdApplication(%p)\n", this);
  }

  QcepProperty::registerMetaTypes();
  QxrdPowderPoint::registerMetaTypes();
  QxrdPowderPointVector::registerMetaTypes();
  QxrdCalibrantDSpacing::registerMetaTypes();
  QxrdCalibrantDSpacings::registerMetaTypes();
  QxrdExperiment::registerMetaTypes();
  QxrdWindowSettings::registerMetaTypes();
  QxrdDetectorControlWindowSettings::registerMetaTypes();
  QxrdTestGenerator::registerMetaTypes();

  setQuitOnLastWindowClosed(false);

  connect(&m_AutoSaveTimer, &QTimer::timeout, this, &QxrdApplication::onAutoSaveTimer);

  m_AutoSaveTimer.start(5000);

  m_PluginInfo =
      QxrdPluginInfoModelPtr(new QxrdPluginInfoModel());
}

bool QxrdApplication::init(int &argc, char **argv)
{

  connect(this, &QCoreApplication::aboutToQuit, this, &QxrdApplication::finish);

  connect(&m_SplashTimer, &QTimer::timeout, this, &QxrdApplication::hideSplash);

  connect(&m_LockerTimer, &QTimer::timeout, this, &QxrdApplication::lockerTimerElapsed);

  m_LastLockerTime.start();
  m_LockerTimer.start(5000);

  QDir::setCurrent(QDir::homePath());

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

  m_ApplicationSettings = QxrdApplicationSettingsPtr(
        new QxrdApplicationSettings(qSharedPointerDynamicCast<QxrdApplication>(sharedFromThis()), argc, argv));

  if (m_ApplicationSettings) {
    m_ApplicationSettings->init();

    connect(m_ApplicationSettings -> prop_Debug(), &QcepInt64Property::valueChanged,
            this, &QxrdApplication::debugChanged);
    readSettings();

    for (int i=1; i<argc; i++) {
      int dbg=0;

      if (strcmp(argv[i],"-new") == 0) {
        m_ApplicationSettings -> set_OpenNew(true);
      } else if (strcmp(argv[i],"-fresh") == 0) {
        m_ApplicationSettings -> set_FreshStart(true);
      } else if (sscanf(argv[i],"-debug=%d",&dbg)==1) {
        m_ApplicationSettings -> set_Debug(dbg);
      } else if (strcmp(argv[i],"-nogui")==0) {
        m_ApplicationSettings -> set_GuiWanted(false);
      } else if (strcmp(argv[i],"-gui")==0) {
        m_ApplicationSettings -> set_GuiWanted(true);
      } else if (strcmp(argv[i],"-nostart")==0) {
        printf("Don't start detectors\n");
        m_ApplicationSettings -> set_StartDetectors(false);
      } else if (strcmp(argv[i],"-start")==0) {
        m_ApplicationSettings -> set_StartDetectors(true);
      } else if (strcmp(argv[i],"-c")==0) {
        if (i++ < argc) {
          m_ApplicationSettings -> prop_CmdList()->appendValue(argv[i]);
        }
      } else if (strcmp(argv[i],"-s")==0) {
        if (i++ < argc) {
          m_ApplicationSettings -> prop_CmdList()->appendValue(tr("loadScript(\"%1\")").arg(argv[i]));
        }
      } else if (strcmp(argv[i],"-p")==0) {
        QDir appDir(qApp->applicationDirPath());

        appDir.cd("plugins");

        pluginsDirList.append(appDir);
      } else {
        m_ApplicationSettings -> prop_FileList()->appendValue(argv[i]);
      }
    }

    if (m_ApplicationSettings -> get_GuiWanted() == false) {
      foreach(QString cmd, m_ApplicationSettings -> get_CmdList()) {
        printf("Cmd: %s\n", qPrintable(cmd));
      }

      foreach(QString file, m_ApplicationSettings -> get_FileList()) {
        printf("File: %s\n", qPrintable(file));
      }
    }

    printMessage(tr("Home Path: %1").arg(QDir::homePath()));
    printMessage(tr("Current Path: %1").arg(QDir::currentPath()));
    printMessage(tr("Root Path %1").arg(QDir::rootPath()));

    setupTiffHandlers();

    QThread::currentThread()->setObjectName("app");

    loadPlugins();
    readSettings();

    printMessage(tr("Optimal thread count = %1").arg(QThread::idealThreadCount()));

    //  m_ResponseTimer = new QxrdResponseTimer(30000, 5000, this);

    //  if (get_FreshStart()) {
    //    editGlobalPreferences();
    //  }

    if (m_ApplicationSettings -> get_OpenNew()) {
      createNewExperiment();
    } else if (m_ApplicationSettings -> get_FreshStart()) {
      openWelcomeWindow();
    } else if (/*get_OpenDirectly() &&*/ (m_ApplicationSettings -> get_CurrentExperiment().length()>0)) {
      openExperiment(m_ApplicationSettings -> get_CurrentExperiment());
    } else {
      openWelcomeWindow();
    }
  }

//  m_AppSaver->start();

  return true;
}

QxrdApplication::~QxrdApplication()
{
#ifndef QT_NO_DEBUG
  printf("Deleting application\n");
#endif

//  m_AppSaver->performSave();

  finish();
  onAutoSaveTimer();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdApplication::~QxrdApplication(%p)\n", this);
  }

//  int nExp = get_ExperimentCount();

//  while (nExp > 0) {
//    printf("Attempt to stop when %d experiments still existing\n", nExp);
//    QcepThread::sleep(5);
//    nExp = get_ExperimentCount();
//  }

//  int nExp = m_ExperimentThreads.count();

//  if (nExp) {
//    printf("deleting application while %d experiments still active\n", nExp);
//  }
}

void QxrdApplication::finish()
{
#ifndef QT_NO_DEBUG
  printf("QxrdApplication::finish()\n");
#endif

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdApplication::finish()");
  }

  GUI_THREAD_CHECK;

  foreach(QxrdExperimentPtr expt, m_Experiments) {
    if (expt) {
      expt->closeWindows();
    }
  }

  writeSettings();

  while (!m_ExperimentThreads.isEmpty()) {
    QxrdExperimentThreadPtr t = m_ExperimentThreads.takeFirst();

    if (t) {
      t->quit();
      t->wait();
    }
  }
}

void QxrdApplication::onAutoSaveTimer()
{
  if (m_ApplicationSettings && m_ApplicationSettings->isChanged()) {
    writeSettings();
  }
}

void QxrdApplication::openWelcomeWindow()
{
  GUI_THREAD_CHECK;

  if (m_WelcomeWindow == NULL) {
    m_WelcomeWindow = new QxrdWelcomeWindow(qSharedPointerDynamicCast<QxrdApplication>(sharedFromThis()));
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

//QWidget* QxrdApplication::window()
//{
//  return m_WelcomeWindow;
//}

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
  return m_NIDAQPlugin;
}

#define xstr(s) str(s)
#define str(s) #s

void QxrdApplication::loadPlugins()
{

#ifdef QXRD_PLUGIN_PATH
  pluginsDirList.append(QDir(xstr(QXRD_PLUGIN_PATH)));
#else
  QDir pluginsDir = QDir(qApp->applicationDirPath());
  pluginsDir.cd("plugins");
  pluginsDirList.append(pluginsDir);
#endif

  foreach (QDir pluginsDir, pluginsDirList) {
    if (qcepDebug(DEBUG_PLUGINS)) {
      printf("Looking for plugins in directory %s\n", qPrintable(pluginsDir.absolutePath()));
    }

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
      QString fullPath = pluginsDir.absoluteFilePath(fileName);

      if (qcepDebug(DEBUG_PLUGINS)) {
        printf("-------------------------------------\n");
        printf("Looking for plugin in file %s\n", qPrintable(fullPath));
      }

      if (QLibrary::isLibrary(fullPath)) {
        QPluginLoader loader(fullPath);

        QJsonObject meta = loader.metaData();

        if (qcepDebug(DEBUG_PLUGINS)) {
          printf("Plugin metadata for %s\n", qPrintable(fullPath));
          foreach(QString key, meta.keys()) {
            printf("Key %s = %s\n", qPrintable(key), qPrintable(meta.value(key).toString()));
          }
        }

        QObject *plugin = loader.instance();
        QString className = meta.value("className").toString();
        QString errorString = "";
        QString pluginName = "";

        if (className == "QxrdAreaDetectorPlugin") {
          m_AreaDetectorPlugin =
              QxrdDetectorPluginInterfacePtr(qobject_cast<QxrdDetectorPluginInterface*>(plugin));
        } else if (className == "QxrdCudaProcessorPlugin") {
          m_CudaPlugin =
              QxrdProcessorInterfacePtr(qobject_cast<QxrdProcessorInterface*>(plugin));
        } else if (className == "QxrdDexelaPlugin") {
          m_DexelaPlugin =
              QxrdDetectorPluginInterfacePtr(qobject_cast<QxrdDetectorPluginInterface*>(plugin));
        } else if (className == "QxrdNIDAQPlugin") {
          m_NIDAQPlugin =
              QxrdNIDAQPluginInterfacePtr(qobject_cast<QxrdNIDAQPluginInterface*>(plugin));
        } else if (className == "QxrdPerkinElmerPlugin") {
          m_PerkinElmerDetectorPlugin =
              QxrdDetectorPluginInterfacePtr(qobject_cast<QxrdDetectorPluginInterface*>(plugin));
        } else if (className == "QxrdPilatusPlugin") {
          m_PilatusDetectorPlugin =
              QxrdDetectorPluginInterfacePtr(qobject_cast<QxrdDetectorPluginInterface*>(plugin));
        } else if (className == "QxrdSimulatedDetectorPlugin") {
          m_SimulatedDetectorPlugin =
              QxrdDetectorPluginInterfacePtr(qobject_cast<QxrdDetectorPluginInterface*>(plugin));
        } else {
        }

        if (plugin) {
          if (qcepDebug(DEBUG_PLUGINS)) {
            printf("Loaded plugin from %s : type %s\n", qPrintable(fullPath), qPrintable(plugin->metaObject()->className()));
          }


          QxrdDetectorPluginInterface* detector = qobject_cast<QxrdDetectorPluginInterface*>(plugin);

          if (detector) {
            pluginName = detector -> name();

//            m_DetectorPlugins.append(
//                  QxrdDetectorPluginInterfacePtr(detector));
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

          if (m_NIDAQPlugin) {
            pluginName = m_NIDAQPlugin -> name();

            m_NIDAQPlugin -> setErrorOutput(this);
          }

          splashMessage(tr("Loaded plugin \"%1\"").arg(pluginName));

          printMessage(tr("Loaded plugin \"%1\" from %2")
                       .arg(pluginName)
                       .arg(pluginsDir.absoluteFilePath(fileName)));
        } else {
          if (qcepDebug(DEBUG_PLUGINS)) {
            printf("Failed to load plugin from %s : %s\n", qPrintable(fileName), qPrintable(loader.errorString()));
          }

          errorString = loader.errorString();

          if (QLibrary::isLibrary(pluginsDir.absoluteFilePath(fileName))) {
            QString msg = tr("Failed to load plugin %1 : %2")
                .arg(pluginsDir.absoluteFilePath(fileName))
                .arg(loader.errorString());
            splashMessage(msg);
            printMessage(msg);
          }
        }


        if (m_PluginInfo) {
          m_PluginInfo->appendEntry(fullPath,
                                    fileName,
                                    className,
                                    (plugin != NULL),
                                    (quint64) plugin,
                                    errorString);
        }

      } else {
        if (qcepDebug(DEBUG_PLUGINS)) {
          printf("File %s is not a library\n", qPrintable(fileName));
        }
      }
    }
  }
}

//QxrdDetectorPluginInterfacePtr QxrdApplication::detectorPlugin(int n)
//{
//  return m_DetectorPlugins.value(n);
//}

void QxrdApplication::splashMessage(QString msg)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "splashMessage", Q_ARG(QString, msg));
  } else {
    GUI_THREAD_CHECK;

    if (m_ApplicationSettings->get_GuiWanted()) {
      if (m_Splash == NULL) {
        m_Splash = QxrdSplashScreenPtr(new QxrdSplashScreen(NULL));
      }

      m_Splash -> show();

      QString msgf = tr("Qxrd Version " STR(QXRD_VERSION) "\n")+msg;

      m_Splash->showMessage(msgf, Qt::AlignBottom|Qt::AlignHCenter);
      processEvents();

      m_SplashTimer.start(5000);
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
  if (experiment(0)) {
    INVOKE_CHECK(QMetaObject::invokeMethod(experiment(0).data(), "warningMessage", Qt::BlockingQueuedConnection, Q_ARG(QString, msg)));
  } else {
    printf("%s\n", qPrintable(msg));
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
    } else {
      printf("%s\n", qPrintable(msg));
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

    if (experiment(0)) {
      INVOKE_CHECK(QMetaObject::invokeMethod(experiment(0).data(), "statusMessage", Qt::QueuedConnection, Q_ARG(QString, message)));
    } else {
      printf("%s\n", qPrintable(message));
    }
  }
}

void QxrdApplication::criticalMessage(QString msg, QDateTime ts)
{
  QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+msg.trimmed();

  message = message.replace("\n", " : ");

  logMessage(message);

  if (experiment(0)) {
    INVOKE_CHECK(QMetaObject::invokeMethod(experiment(0).data(), "criticalMessage", Qt::QueuedConnection, Q_ARG(QString, message)));
  } else {
    printf("%s\n", qPrintable(message));
  }
}

void QxrdApplication::printLine(QString msg)
{
  logMessage(msg);

  if (experiment(0)) {
    INVOKE_CHECK(QMetaObject::invokeMethod(experiment(0).data(), "printLine", Qt::QueuedConnection, Q_ARG(QString, msg)));
  } else {
    printf("%s\n", qPrintable(msg));
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

QxrdApplicationSettingsWPtr QxrdApplication::settings()
{
  return m_ApplicationSettings;
}

void QxrdApplication::readSettings()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_SettingsMutex);

  QxrdGlobalSettings settings(this);

  if (m_ApplicationSettings) {
    settings.beginGroup("application");
    m_ApplicationSettings -> readSettings(&settings);
    settings.endGroup();
  }
}

void QxrdApplication::writeSettings()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_SettingsMutex);

  QxrdGlobalSettings settings(this);

  if (m_ApplicationSettings) {
    settings.beginGroup("application");
    m_ApplicationSettings -> writeSettings(&settings);
    settings.endGroup();
    m_ApplicationSettings -> setChanged(0);
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

  if (m_ApplicationSettings) {
    settings.beginGroup("application");
    m_ApplicationSettings -> readSettings(&settings);
    settings.endGroup();
  }
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

    if (m_ApplicationSettings) {
      settings.beginGroup("application");
      m_ApplicationSettings -> writeSettings(&settings);
      settings.endGroup();
    }
  }

  QFile::remove(path+".bak");
  QFile::rename(path, path+".bak");
  QFile::rename(path+".new", path);
}

void QxrdApplication::possiblyQuit()
{
  if (wantToQuit()) {
//    finish();
    quit();
  }
}

bool QxrdApplication::wantToQuit()
{
  return QMessageBox::question(NULL, tr("Really Quit?"),
                               tr("Do you really want to exit the application?"),
                               QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
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
#ifdef Q_CC_CLANG
  about += " clang";
#else
  about += " gcc";
#endif
#endif

#ifdef QT_NO_DEBUG
  about += " Release\n";
#else
  about += " Debug\n";
#endif

  about += tr("Qt Version %1\n").arg(qVersion());
  about += tr("Qceplib Version %1\n").arg(STR(QCEPLIB_VERSION));
  about += tr("QWT Version %1\n").arg(STR(QCEPLIB_QWT_VERSION));
  about += tr("Mar345 Version %1\n").arg(STR(QCEPLIB_MAR345_VERSION));
  about += tr("CBF Version %1\n").arg(STR(QCEPLIB_CBF_VERSION));
  about += tr("TIFF Version %1\n").arg(STR(QCEPLIB_TIFF_VERSION));
  about += tr("LevMar Version %1\n").arg(STR(QCEPLIB_LEVMAR_VERSION));
#ifdef QCEPLIB_ZLIB_VERSION
  about += tr("ZLIB Version %1\n").arg(STR(QCEPLIB_ZLIB_VERSION));
#endif

#ifdef QCEPLIB_SZIP_VERSION
  about += tr("SZIP Version %1\n").arg(STR(QCEPLIB_SZIP_VERSION));
#endif

#ifdef QCEPLIB_HDF5_VERSION
  about += tr("HDF5 Version %1\n").arg(STR(QCEPLIB_HDF5_VERSION));
#endif
  about += tr("Spec Server Version %1\n").arg(STR(QCEPLIB_SPECSERVER_VERSION));

  QMessageBox::about(NULL, "QXRD", about);
}

void QxrdApplication::doOpenQXRDWebPage()
{
  QDesktopServices::openUrl(QUrl("http://qxrd.sourceforge.net/"));
}

void QxrdApplication::doOpenURL(QString url)
{
  QDesktopServices::openUrl(QUrl(url));
}

void QxrdApplication::editGlobalPreferences()
{
  QxrdGlobalPreferencesDialog prefs(m_ApplicationSettings, m_PluginInfo);

  prefs.exec();
}

void QxrdApplication::debugChanged(qint64 newValue)
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

  QxrdApplication *app = qobject_cast<QxrdApplication*>(g_Application);

  if (app) {
    app -> tiffError(module, msg);
  }
}

void QxrdApplication::setupTiffHandlers()
{
  qcepTIFFSetErrorHandler      (&qxrdTIFFErrorHandler);
  qcepTIFFSetErrorHandlerExt   (NULL);
  qcepTIFFSetWarningHandler    (&qxrdTIFFWarningHandler);
  qcepTIFFSetWarningHandlerExt (NULL);
}

void QxrdApplication::tiffWarning(const char *module, const char *msg)
{
  QxrdApplication *app = qobject_cast<QxrdApplication*>(g_Application);

  if (app) {
    app->criticalMessage(tr("TIFF Warning from %1 : %2").arg(module).arg(msg));
  }
}

void QxrdApplication::tiffError(const char *module, const char *msg)
{
  QxrdApplication *app = qobject_cast<QxrdApplication*>(g_Application);

  if (app) {
    app->criticalMessage(tr("TIFF Error from %1 : %2").arg(module).arg(msg));
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

void QxrdApplication::setDefaultObjectData(QcepDataObject *obj)
{
  if (obj) {
    obj->set_Creator("QXRD");
    obj->set_Version(STR(QXRD_VERSION));
    obj->set_QtVersion(QT_VERSION_STR);
  }
}

void QxrdApplication::readDefaultSettings()
{
  QSettings settings("cep.xor.aps.anl.gov", "qxrd-defaults");

  m_ApplicationSettings -> set_RecentExperiments(settings.value("recentExperiments").toStringList());
  m_ApplicationSettings -> set_RecentExperimentsSize(settings.value("recentExperimentsSize", 8).toInt());
  m_ApplicationSettings -> set_CurrentExperiment(settings.value("currentExperiment").toString());
}

void QxrdApplication::writeDefaultSettings()
{
  QSettings settings("cep.xor.aps.anl.gov", "qxrd-defaults");

  settings.setValue("recentExperiments", m_ApplicationSettings -> get_RecentExperiments());
  settings.setValue("recentExperimentsSize", m_ApplicationSettings -> get_RecentExperimentsSize());
  settings.setValue("currentExperiment", m_ApplicationSettings -> get_CurrentExperiment());
}

void QxrdApplication::createNewExperiment()
{
  QxrdExperimentThreadPtr expthr =
      QxrdExperimentThread::newExperimentThread(
        "", qSharedPointerDynamicCast<QxrdApplication>(sharedFromThis()), QxrdExperimentSettingsPtr());

  if (expthr) {
    openedExperiment(expthr);

    closeWelcomeWindow();
  }
}

void QxrdApplication::chooseExistingExperiment()
{
  QString res = QFileDialog::getOpenFileName(NULL,
                                             "Open an existing experiment...",
                                             m_ApplicationSettings -> get_CurrentExperiment(),
                                             "QXRD Experiments (*.qxrdp);;Other Files (*)");

  if (res.length() > 0) {
    openExperiment(res);
  }
}

void QxrdApplication::openExperiment(QString path)
{
  if (path.length() > 0) {
    QxrdExperimentSettingsPtr settings(new QxrdExperimentSettings(path));

    QxrdExperimentThreadPtr expthr =
        QxrdExperimentThread::newExperimentThread(
          path,
          qSharedPointerDynamicCast<QxrdApplication>(sharedFromThis()),
          settings);

    printMessage("");
    printMessage(tr("===== Open Experiment %1").arg(path));

    openedExperiment(expthr);
  }
}

void QxrdApplication::openExperiment2(QString path)
{
  if (path.length() > 0) {
    QxrdExperimentSettingsPtr settings(new QxrdExperimentSettings(path));

    if (settings) {
      QcepObjectPtr newObj = QcepObject::readObject(settings.data());

      if (newObj) {
        QxrdExperimentPtr newExpt = qSharedPointerDynamicCast<QxrdExperiment>(newObj);

        if (newExpt) {
          printMessage(tr("Opened new experiment %1").arg(path));

          QxrdExperimentSettingsPtr newSettings(new QxrdExperimentSettings(path+".new2"));

          newExpt->writeSettings(newSettings.data());
        }
      }
    }
  }
}

void QxrdApplication::closeExperiment(QxrdExperimentWPtr expw)
{
  if (qcepDebug(DEBUG_APP)) {
    printf("QxrdApplication::closeExperiment(%p)\n", expw.data());
  }

  QxrdExperimentPtr exp(expw);

  if (exp) {
    closedExperiment(exp->experimentThread());
  }
}

void QxrdApplication::appendRecentExperiment(QString path)
{
  QStringList recent = m_ApplicationSettings -> get_RecentExperiments();

  recent.prepend(path);
  recent.removeDuplicates();

  while(recent.length() > m_ApplicationSettings -> get_RecentExperimentsSize()) {
    recent.removeLast();
  }

  m_ApplicationSettings -> set_RecentExperiments(recent);
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

void QxrdApplication::openRecentExperiment(QString path)
{
  QFileInfo info(path);

  if (info.exists()) {
    openExperiment(path);
  } else {
    printMessage(tr("Experiment %1 does not exist").arg(path));
  }
}

void QxrdApplication::openedExperiment(QxrdExperimentThreadWPtr expwthr)
{
  QxrdExperimentThreadPtr exptthr(expwthr);

  if (exptthr) {
    QxrdExperimentPtr expt(exptthr->experiment());

    if (expt) {
      QString path = expt->experimentFilePath();
      m_ApplicationSettings -> set_CurrentExperiment(path);
      appendRecentExperiment(path);

      m_ExperimentThreads.append(exptthr);
      m_Experiments.append(expt);

      printMessage("");
      printMessage("New experiment loaded");
      printMessage("");

      closeWelcomeWindow();

      expt->openWindows();
      expt->setChanged(0);
    }
  }
}

void QxrdApplication::closedExperiment(QxrdExperimentThreadWPtr expwthr)
{
  QxrdExperimentThreadPtr exptthr(expwthr);

  if (exptthr) {
    QxrdExperimentPtr expt(exptthr->experiment());

    if (expt) {
      m_Experiments.removeAll(expt);
      m_ExperimentThreads.removeAll(exptthr);
    }
  }
}

QList<QxrdExperimentWPtr> &QxrdApplication::experiments()
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

void QxrdApplication::incLockerCount()
{
  m_ApplicationSettings -> prop_LockerCount()->incValue(1);
}

void QxrdApplication::lockerTimerElapsed()
{
  double elapsed = m_LastLockerTime.restart();

  double rate = 1000.0*(m_ApplicationSettings -> get_LockerCount() - m_LastLockerCount)/elapsed;

//  set_LockerRate(rate);

//  m_LastLockerTime = QTime::currentTime();
  m_LastLockerCount= m_ApplicationSettings -> get_LockerCount();

  if (rate>10000) {
    printMessage(tr("Locker rate %1 locks/sec").arg(/*get_LockerRate()*/rate));
  }
}

QxrdPluginInfoModelWPtr QxrdApplication::pluginInfo()
{
  return m_PluginInfo;
}
