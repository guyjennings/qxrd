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
#include "qxrdnamedplugininterface.h"
#include "qxrddetectorplugininterface.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdglobalsettings.h"
#include "qxrdexperiment.h"
#include "qxrdexperimentsettings.h"
#include "qcepmutexlocker.h"
#include "qxrdcalibrant.h"
#include "qxrdcalibrantlibrary.h"
#include "qxrddetectorcontrolwindowsettings.h"
#include "qcepimagedataformattiff.h"
#include "qxrdplugininfomodel.h"
#include "qxrdstartupwindow.h"

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
  m_NIDAQPlugin(NULL),
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
    m_Application -> setApplicationVersion(STR(QXRD_VERSION));
  }

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

  //  m_ResponseTimer = new QxrdResponseTimer(30000, 5000, this);

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

QString QxrdApplication::applicationDescription()
{
  return QStringLiteral("QXRD Data Acquisition for 2-D XRay Detectors");
}

void QxrdApplication::openStartupWindow()
{
  m_StartupWindow = QxrdStartupWindowPtr(
        new QxrdStartupWindow());

  m_StartupWindow -> setApplicationIcon(QIcon(":/images/qxrd-icon-256x256.png"));
  m_StartupWindow -> setApplicationDescription(tr(
        "<h3>Data Acquisition for 2-D XRay Detectors<h3>\n"
        "<h3>Guy Jennings<h3>\n"
        "<h3>Version %1</h3>\n"
        "<p>Build : " __DATE__ " : " __TIME__ "</p>\n"
        ).arg(STR(QXRD_VERSION)));

  m_StartupWindow -> show();
  m_StartupWindow -> raise();
}

void QxrdApplication::closeStartupWindow()
{
}

//TODO: maybe rewrite...
void QxrdApplication::onAutoSaveTimer()
{
//  if (settings() && settings()->isChanged()) {
//    writeApplicationSettings();
//  }
}

QxrdNIDAQPluginInterfacePtr QxrdApplication::nidaqPlugin()
{
  return m_NIDAQPlugin;
}

#define xstr(s) str(s)
#define str(s) #s

void QxrdApplication::loadPlugins()
{
#ifdef QXRD_PLUGIN_PATH
  prop_PluginList()->appendValue(QDir(xstr(QXRD_PLUGIN_PATH)));
#else
  QDir pluginsDir = QDir(qApp->applicationDirPath());
  pluginsDir.cd("plugins");
  prop_PluginList()->appendValue(pluginsDir.absolutePath());
#endif

  if (get_PluginList().count() == 0) {
    splashMessage("No plugin directories specified\n");
  } else {
    foreach (QString dir, get_PluginList()) {
      splashMessage(tr("Plugin directory %1").arg(dir));
    }
  }

  foreach (QDir pluginsDir, get_PluginList()) {
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
          if (m_AreaDetectorPlugin) {
            splashMessage(tr("Area Detector Plugin loaded from %1").arg(fullPath));

            m_AreaDetectorPlugin -> initialize(sharedFromThis());
          }
        } else if (className == "QxrdDexelaPlugin") {
          m_DexelaPlugin =
              QxrdDetectorPluginInterfacePtr(qobject_cast<QxrdDetectorPluginInterface*>(plugin));
          if (m_DexelaPlugin) {
            splashMessage(tr("Dexela Plugin loaded from %1").arg(fullPath));

            m_DexelaPlugin -> initialize(sharedFromThis());
          }
        } else if (className == "QxrdNIDAQPlugin") {
          m_NIDAQPlugin =
              QxrdNIDAQPluginInterfacePtr(qobject_cast<QxrdNIDAQPluginInterface*>(plugin));
          if (m_NIDAQPlugin) {
            splashMessage(tr("NIDAQ Plugin loaded from %1").arg(fullPath));

            m_NIDAQPlugin -> initialize(sharedFromThis());
          }
        } else if (className == "QxrdPerkinElmerPlugin") {
          m_PerkinElmerDetectorPlugin =
              QxrdDetectorPluginInterfacePtr(qobject_cast<QxrdDetectorPluginInterface*>(plugin));
          if (m_PerkinElmerDetectorPlugin) {
            splashMessage(tr("Perkin Elmer Plugin loaded from %1").arg(fullPath));

            m_PerkinElmerDetectorPlugin -> initialize(sharedFromThis());
          }
        } else if (className == "QxrdPilatusPlugin") {
          m_PilatusDetectorPlugin =
              QxrdDetectorPluginInterfacePtr(qobject_cast<QxrdDetectorPluginInterface*>(plugin));
          if (m_PilatusDetectorPlugin) {
            splashMessage(tr("Pilatus Plugin loaded from %1").arg(fullPath));

            m_PilatusDetectorPlugin -> initialize(sharedFromThis());
          }
        } else if (className == "QxrdSimulatedPlugin") {
          m_SimulatedDetectorPlugin =
              QxrdDetectorPluginInterfacePtr(qobject_cast<QxrdDetectorPluginInterface*>(plugin));
          if (m_SimulatedDetectorPlugin) {
            splashMessage(tr("Simulated Detector Plugin loaded from %1").arg(fullPath));

            m_SimulatedDetectorPlugin -> initialize(sharedFromThis());
          }
        } else if (className == "QxrdFileWatcherPlugin") {
          m_FileWatcherPlugin =
              QxrdDetectorPluginInterfacePtr(qobject_cast<QxrdDetectorPluginInterface*>(plugin));
          if (m_FileWatcherPlugin) {
            splashMessage(tr("File Watcher Plugin loaded from %1").arg(fullPath));

            m_FileWatcherPlugin -> initialize(sharedFromThis());
          }
        } else {
        }

        if (plugin) {
          QxrdNamedPluginInterface* namedPlugin = qobject_cast<QxrdNamedPluginInterface*>(plugin);

          if (namedPlugin) {
            pluginName = namedPlugin->name();
          }

          if (qcepDebug(DEBUG_PLUGINS)) {
            printf("Loaded plugin %s from %s : type %s\n", qPrintable(pluginName), qPrintable(fullPath), qPrintable(plugin->metaObject()->className()));
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

QxrdDetectorPluginInterfaceWPtr QxrdApplication::detectorPlugin(int detType)
{
  QxrdDetectorPluginInterfaceWPtr res;

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

void QxrdApplication::printMessage(QString msg, QDateTime ts) const
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
    obj->set_Creator("QXRD");
    obj->set_Version(STR(QXRD_VERSION));
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
  QxrdExperimentThreadPtr expthr =
      QxrdExperimentThread::newExperimentThread(
        "",
        qSharedPointerDynamicCast<QxrdAppCommon>(sharedFromThis()),
        QxrdExperimentSettingsPtr(),
        QxrdExperiment::AcquisitionAllowed);

  if (expthr) {
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
    QxrdExperimentSettingsPtr settings(new QxrdExperimentSettings(path));

    QxrdExperimentThreadPtr expthr =
        QxrdExperimentThread::newExperimentThread(
          path,
          qSharedPointerDynamicCast<QxrdAppCommon>(sharedFromThis()),
          settings,
          QxrdExperiment::AcquisitionAllowed);

    printMessage("");
    printMessage(tr("===== Open Experiment %1").arg(path));

    openedExperiment(expthr);
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
