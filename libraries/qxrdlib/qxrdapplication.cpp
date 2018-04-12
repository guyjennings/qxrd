#define _CRT_SECURE_NO_WARNINGS

#include "qxrddebug.h"
#include "qxrdapplication.h"
#include "qxrdapplication-ptr.h"
#include "qxrdexperimentthread.h"
#include "qxrdwindow.h"
#include "qxrdserverthread.h"
#include "qxrdserver.h"
#include "qxrdsimpleserverthread.h"
#include "qxrdsimpleserver.h"
#include "qxrdprocessor.h"
#include "qxrdintegrator.h"
#include "qxrdacquisition.h"
#include "qcepallocator.h"
#include "qxrdfilesaverthread.h"
#include "qxrdfilesaver.h"
#include "qxrdscriptenginethread.h"
#include "qxrdscriptengine.h"
#include "qxrdglobalpreferencesdialog.h"
#include "qcepproperty.h"
#include "qxrdglobalsettings.h"
#include "qxrdexperiment.h"
#include "qxrdexperimentsettings.h"
#include "qcepmutexlocker.h"
#include "qcepcalibrant.h"
#include "qcepcalibrantlibrary.h"
#include "qxrddetectorcontrolwindowsettings.h"
#include "qcepimagedataformattiff.h"
#include "qxrdplugininfomodel.h"
#include "qxrdstartupwindow.h"
#include "qxrdstartupwindowsettings.h"
#include "qxrddetectorplugin.h"
#include "qxrdsynchronizerplugin.h"
#include <QTime>
#include <QtConcurrentRun>
#include <QFileDialog>
#include <QString>
#include <tiffio.h>
#include <QPluginLoader>
#include <QDesktopServices>
#include <QUrl>
#include <QCoreApplication>
#include <QJsonObject>
#include <QCommandLineParser>
#include <QMessageBox>
#include <QApplication>

int eventCounter;

void QxrdApplication::processEventCounter()
{
  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("%1 events processed").arg(eventCounter));
  }

  eventCounter = 0;
}

QxrdApplication::QxrdApplication(int &argc, char **argv) :
  inherited(argc, argv),
  m_ObjectNamer(this, "application"),
  m_SimulatedDetectorPlugin(NULL),
  m_PerkinElmerDetectorPlugin(NULL),
  m_DexelaPlugin(NULL),
  m_AlliedVisionPlugin(NULL),
  m_PilatusDetectorPlugin(NULL),
  m_AreaDetectorPlugin(NULL),
  m_FileWatcherPlugin(NULL),
  m_SettingsMutex(),
  m_LastLockerCount(0)
{
#ifndef QT_NO_DEBUG
  printf("Constructing application\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdApplication::QxrdApplication(%p)\n", this);
  }

  if (m_Application) {
    m_Application -> setQuitOnLastWindowClosed(false);
  }

  connect(&m_AutoSaveTimer, &QTimer::timeout, this, &QxrdApplication::onAutoSaveTimer);

  m_AutoSaveTimer.start(5000);

  m_PluginInfo =
      QxrdPluginInfoModelPtr(new QxrdPluginInfoModel());
}

void QxrdApplication::initializeRoot()
{
  inherited::initializeRoot();

  connect(&m_LockerTimer, &QTimer::timeout, this, &QxrdApplication::lockerTimerElapsed);

  m_LastLockerTime.start();
  m_LockerTimer.start(5000);

  QDir::setCurrent(QDir::homePath());

  if (m_Application) {
    m_Application -> setOrganizationName("cep");
    m_Application -> setOrganizationDomain("xray.aps.anl.gov");
    m_Application -> setApplicationName("qxrd");
    m_Application -> setApplicationVersion(applicationVersion());
  }

  printMessage("------ Starting QXRD Application ------");

  QString about = applicationName() + " Version " + applicationVersion();

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
  about += " Release";
#else
  about += " Debug";
#endif

  printMessage(about);
  splashMessage(about);
  splashMessage("QWT Version " QWT_VERSION_STR);
  splashMessage(tr("QT Version %1").arg(qVersion()));

  if (get_GuiWanted() == false) {
    foreach(QString cmd, get_CmdList()) {
      printf("Cmd: %s\n", qPrintable(cmd));
    }

    foreach(QString file, get_FileList()) {
      printf("File: %s\n", qPrintable(file));
    }

    foreach(QString patt, get_WatcherList()) {
      printf("Watch: %s\n", qPrintable(patt));
    }
  } else {

  }

  splashMessage(tr("Home Path: %1").arg(QDir::homePath()));
  splashMessage(tr("Current Path: %1").arg(QDir::currentPath()));
  splashMessage(tr("Root Path %1").arg(QDir::rootPath()));

  char *pwd = getenv("PWD");
  splashMessage(tr("pwd: %1").arg(pwd));

  loadPlugins();
//  readApplicationSettings();

  splashMessage(tr("Optimal thread count = %1").arg(QThread::idealThreadCount()));

  //  if (get_FreshStart()) {
  //    editGlobalPreferences();
  //  }

  int nWatches = get_WatcherList().length();
  int nFiles   = get_FileList().length();

  if (nFiles > 0 || nWatches > 0) {
    foreach(QString file, get_FileList()) {
      openFile(file);
    }

    foreach(QString patt, get_WatcherList()) {
      openWatcher(patt);
    }
  } else if (get_OpenNew()) {
    createNewExperiment();
  } else if (get_FreshStart()) {
    openWelcomeWindow();
  } else if (get_CurrentExperiment().length()>0) {
    openExperiment(get_CurrentExperiment());
  } else {
    openWelcomeWindow();
  }
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

  writeApplicationSettings();

  inherited::finish();
}

QString QxrdApplication::applicationName()
{
  return QStringLiteral("QXRD");
}

QString QxrdApplication::applicationVersion()
{
  return QStringLiteral(STR(QXRD_VERSION));
}

QString QxrdApplication::applicationDescription()
{
  return QStringLiteral("QXRD Data Acquisition for 2-D XRay Detectors");
}

QIcon QxrdApplication::applicationIcon()
{
  return QIcon(":/images/qxrd-icon-64x64.png");
}

void QxrdApplication::openStartupWindow()
{
  if (m_StartupWindowSettings) {
    m_StartupWindow =
        qSharedPointerDynamicCast<QxrdStartupWindow>(
          m_StartupWindowSettings -> newWindow());

    m_StartupWindow -> initialize(sharedFromThis());

    m_StartupWindow -> setApplicationIcon(QIcon(":/images/qxrd-icon-256x256.png"));
    m_StartupWindow -> setApplicationDescription(
          "Data Acquisition for 2-D XRay Detectors\n"
          "Guy Jennings\n"
          "Version " + applicationVersion() + "\n"
          "Build : " __DATE__ " : " __TIME__);

    m_StartupWindow -> setWindowTitle(applicationDescription());
    m_StartupWindow -> setWindowIcon(applicationIcon());
    m_StartupWindow -> show();
    m_StartupWindow -> raise();
  }
}

void QxrdApplication::closeStartupWindow()
{
}

void QxrdApplication::onAutoSaveTimer()
{
  if (isChanged()) {
    writeApplicationSettings();
  }
}

QxrdSynchronizerPluginPtr QxrdApplication::synchronizerPlugin()
{
  return m_SynchronizerPlugin;
}

#define xstr(s) str(s)
#define str(s) #s

void QxrdApplication::loadPlugins()
{
  if (get_LoadPlugins()) {
#ifdef QXRD_PLUGIN_PATH
    prop_PluginList()->appendValue(xstr(QXRD_PLUGIN_PATH));
#else
    QDir pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("plugins");
    prop_PluginList()->appendValue(pluginsDir.absolutePath());
#endif

    if (get_PluginList().count() == 0) {
      splashMessage("No plugin directories specified");
    } else {
      foreach (QString dir, get_PluginList()) {
        splashMessage(tr("Plugin directory %1").arg(dir));
      }
    }

    foreach (QDir pluginsDir, get_PluginList()) {
      if (qcepDebug(DEBUG_PLUGINS)) {
        printMessage(tr("Looking for plugins in directory %1").arg(pluginsDir.absolutePath()));
      }

      foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QString fullPath = pluginsDir.absoluteFilePath(fileName);

        if (qcepDebug(DEBUG_PLUGINS)) {
          printMessage("-------------------------------------");
          printMessage(tr("Looking for plugin in file %1").arg(fileName));
        }

        if (QLibrary::isLibrary(fullPath)) {
          QPluginLoader loader(fullPath);

          QJsonObject meta = loader.metaData();

          if (qcepDebug(DEBUG_PLUGINS)) {
            printMessage(tr("Plugin metadata for %1").arg(fullPath));
            foreach(QString key, meta.keys()) {
              printMessage(tr("Key %1 = %2").arg(key).arg(meta.value(key).toString()));
            }
          }

          QString className = meta.value("className").toString();

          QByteArray path = qgetenv("PATH");

          if (className == "QxrdAlliedVisionPlugin") {
            qputenv("PATH", "C:\\Program Files\\Allied Vision\\Vimba_2.1\\VimbaC\\Bin\\Win64;"
                            "C:\\Program Files\\Allied Vision\\Vimba_2.1\\VimbaCPP\\Bin\\Win64");
          }

          QObject *plugin = loader.instance();
          QString errorString = "";
          QString pluginName = "";

          qputenv("PATH", path);

          QxrdDetectorPlugin     *detPlugin   = qobject_cast<QxrdDetectorPlugin*>(plugin);
          QxrdSynchronizerPlugin *syncPlugin  = qobject_cast<QxrdSynchronizerPlugin*>(plugin);

          if (detPlugin) {
            detPlugin -> registerMetaTypes();
          } else if (syncPlugin) {
            syncPlugin -> registerMetaTypes();
          }

          if (className == "QxrdAreaDetectorPlugin") {
            if (m_AreaDetectorPlugin) {
              splashMessage("Area detector plugin already loaded");
            } else {
              m_AreaDetectorPlugin =
                  QxrdDetectorPluginPtr(detPlugin);
              if (m_AreaDetectorPlugin) {
                splashMessage(tr("Area Detector Plugin loaded from %1").arg(fileName));

                m_AreaDetectorPlugin -> initialize(sharedFromThis());
              }
            }
          } else if (className == "QxrdAlliedVisionPlugin") {
            if (m_AlliedVisionPlugin) {
              splashMessage("Allied Vision Detector Plugin already loaded");
            } else {
              m_AlliedVisionPlugin =
                  QxrdDetectorPluginPtr(detPlugin);
              if (m_AlliedVisionPlugin) {
                splashMessage(tr("Allied Vision Plugin loaded from %1").arg(fileName));

                m_AlliedVisionPlugin -> initialize(sharedFromThis());
              }
            }
          } else if (className == "QxrdDexelaPlugin") {
            if (m_DexelaPlugin) {
              splashMessage("Dexela Detector Plugin already loaded");
            } else {
              m_DexelaPlugin =
                  QxrdDetectorPluginPtr(detPlugin);
              if (m_DexelaPlugin) {
                splashMessage(tr("Dexela Plugin loaded from %1").arg(fileName));

                m_DexelaPlugin -> initialize(sharedFromThis());
              }
            }
          } else if (className == "QxrdNIDAQPlugin") {
            if (m_SynchronizerPlugin) {
              splashMessage("NIDAQ Plugin already loaded");
            } else {
              m_SynchronizerPlugin =
                  QxrdSynchronizerPluginPtr(syncPlugin);
              if (m_SynchronizerPlugin) {
                splashMessage(tr("Synchronizer Plugin loaded from %1").arg(fileName));

                m_SynchronizerPlugin -> initialize(sharedFromThis());
              }
            }
          } else if (className == "QxrdPerkinElmerPlugin") {
            if (m_PerkinElmerDetectorPlugin) {
              splashMessage("Perkin Elmer Plugin already loaded");
            } else {
              m_PerkinElmerDetectorPlugin =
                  QxrdDetectorPluginPtr(detPlugin);
              if (m_PerkinElmerDetectorPlugin) {
                splashMessage(tr("Perkin Elmer Plugin loaded from %1").arg(fileName));

                m_PerkinElmerDetectorPlugin -> initialize(sharedFromThis());
              }
            }
          } else if (className == "QxrdPilatusPlugin") {
            if (m_PilatusDetectorPlugin) {
              splashMessage("Pilatus Plugin already loaded");
            } else {
              m_PilatusDetectorPlugin =
                  QxrdDetectorPluginPtr(detPlugin);
              if (m_PilatusDetectorPlugin) {
                splashMessage(tr("Pilatus Plugin loaded from %1").arg(fileName));

                m_PilatusDetectorPlugin -> initialize(sharedFromThis());
              }
            }
          } else if (className == "QxrdSimulatedPlugin") {
            if (m_SimulatedDetectorPlugin) {
              splashMessage("Simulated Detector Plugin already loaded");
            } else {
              m_SimulatedDetectorPlugin =
                  QxrdDetectorPluginPtr(detPlugin);
              if (m_SimulatedDetectorPlugin) {
                splashMessage(tr("Simulated Detector Plugin loaded from %1").arg(fileName));

                m_SimulatedDetectorPlugin -> initialize(sharedFromThis());
              }
            }
          } else if (className == "QxrdFileWatcherPlugin") {
            if (m_FileWatcherPlugin) {
              splashMessage("File Watcher Plugin already loaded");
            } else {
              m_FileWatcherPlugin =
                  QxrdDetectorPluginPtr(detPlugin);
              if (m_FileWatcherPlugin) {
                splashMessage(tr("File Watcher Plugin loaded from %1").arg(fileName));

                m_FileWatcherPlugin -> initialize(sharedFromThis());
              }
            }
          } else {
          }

          if (plugin) {
            QxrdDetectorPlugin* detPlugin = qobject_cast<QxrdDetectorPlugin*>(plugin);

            if (detPlugin) {
              pluginName = detPlugin->name();
            }

            QxrdSynchronizerPlugin* syncPlugin = qobject_cast<QxrdSynchronizerPlugin*>(plugin);

            if (syncPlugin) {
              pluginName = syncPlugin->name();
            }

            if (qcepDebug(DEBUG_PLUGINS)) {
              printMessage(tr("Loaded plugin %1 from %2 : type %3")
                           .arg(pluginName).arg(fullPath).arg(plugin->metaObject()->className()));
            }

            splashMessage(tr("Loaded plugin \"%1\"").arg(pluginName));

            printMessage(tr("Loaded plugin \"%1\" from %2")
                         .arg(pluginName)
                         .arg(pluginsDir.absoluteFilePath(fileName)));
          } else {
            if (qcepDebug(DEBUG_PLUGINS)) {
              printMessage(tr("Failed to load plugin from %1 : %2").arg(fileName).arg(loader.errorString()));
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
            printMessage(tr("File %1 is not a library").arg(fileName));
          }
        }
      }
    }
  }
}

QxrdDetectorPluginPtr QxrdApplication::detectorPlugin(int detType)
{
  QxrdDetectorPluginPtr res;

  switch (detType) {
  case QxrdDetectorSettings::Simulated:
    res = m_SimulatedDetectorPlugin;
    break;

  case QxrdDetectorSettings::PerkinElmer:
    res = m_PerkinElmerDetectorPlugin;
    break;

  case QxrdDetectorSettings::Pilatus:
    res = m_PilatusDetectorPlugin;
    break;

  case QxrdDetectorSettings::AreaDetector:
    res = m_AreaDetectorPlugin;
    break;

  case QxrdDetectorSettings::FileWatcher:
    res = m_FileWatcherPlugin;
    break;

  case QxrdDetectorSettings::Dexela:
    res = m_DexelaPlugin;
    break;

  case QxrdDetectorSettings::AlliedVision:
    res = m_AlliedVisionPlugin;
    break;
  }

  return res;
}


void QxrdApplication::logMessage(QString /*msg*/) const
{
}

void QxrdApplication::warningMessage(QString msg, QDateTime /*ts*/) const
{
  if (experiment(0)) {
    INVOKE_CHECK(QMetaObject::invokeMethod(experiment(0).data(), "warningMessage", Qt::BlockingQueuedConnection, Q_ARG(QString, msg)));
  } else {
    printf("%s\n", qPrintable(msg));
  }
}

void QxrdApplication::statusMessage(QString msg, QDateTime ts) const
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

void QxrdApplication::criticalMessage(QString msg, QDateTime ts) const
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

void QxrdApplication::printLine(QString msg) const
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

void QxrdApplication::readApplicationSettings()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_SettingsMutex);

  QxrdGlobalSettings set(this);

  set.beginGroup("application");
  readSettings(&set);
  set.endGroup();
}

void QxrdApplication::writeApplicationSettings()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_SettingsMutex);

  QxrdGlobalSettings set(this);

  set.beginGroup("application");
  writeSettings(&set);
  set.endGroup();
  setChanged(0);
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

  settings.beginGroup("application");
  readSettings(&settings);
  settings.endGroup();
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

    settings.beginGroup("application");
    writeSettings(&settings);
    settings.endGroup();
  }

  QFile::remove(path+".bak");
  QFile::rename(path, path+".bak");
  QFile::rename(path+".new", path);
}

//void QxrdApplication::doAboutQxrd()
//{
//  QString about = "QXRD Data Acquisition for PE Area Detectors";

//  doAbout(about);
//}

void QxrdApplication::editGlobalPreferences()
{
  QxrdApplicationPtr myself(
        qSharedPointerDynamicCast<QxrdApplication>(sharedFromThis()));

  QxrdGlobalPreferencesDialog prefs(myself, m_PluginInfo);

  prefs.exec();
}

bool QxrdApplication::event(QEvent *ev)
{
  QTime tick;
  tick.start();

  bool res = false;

  if (m_Application) {
    res = inherited::event(ev);
  }

  int elapsed = tick.restart();

  if (elapsed > 1000) {
    printMessage("event processing took more than 1 sec");
  }

  return res;
}

void QxrdApplication::setDefaultObjectData(QcepDataObject *obj)
{
  if (obj) {
    obj->set_Creator(applicationName());
    obj->set_Version(applicationVersion());
    obj->set_QtVersion(QT_VERSION_STR);
  }
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

void QxrdApplication::createNewExperiment()
{
  splashMessage("===== Opening new experiment");

  QxrdExperimentThreadPtr expthr =
      QxrdExperimentThreadPtr(
        new QxrdExperimentThread("experimentThread"));

  if (expthr) {
    expthr -> initialize(sharedFromThis(), "", QxrdExperimentSettingsPtr(), QxrdExperiment::AcquisitionAllowed);
    expthr -> start();

    openedExperiment(expthr);

    closeWelcomeWindow();
  }
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
    QxrdExperimentSettingsPtr settings(
          new QxrdExperimentSettings(
            QxrdExperimentSettings::latestAutoPath(path)));

    QxrdExperimentThreadPtr expthr =
        QxrdExperimentThreadPtr(
          new QxrdExperimentThread("experimentThread"));

    splashMessage(tr("===== Opening Experiment %1").arg(path));

    if (expthr) {
      expthr -> initialize(sharedFromThis(), path, settings, QxrdExperiment::AcquisitionAllowed);
      expthr -> start();

      openedExperiment(expthr);
    }
  }
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
    printMessage(tr("Locker rate %1 locks/sec").arg(rate));
  }
}

QxrdPluginInfoModelWPtr QxrdApplication::pluginInfo()
{
  return m_PluginInfo;
}
