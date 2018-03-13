#include "qxrdappcommon.h"
#include "qxrddebug.h"
#include "qcepimagedataformattiff.h"
#include "qxrdpowderpoint.h"
#include "qxrdcalibrantdspacing.h"
#include "qxrdcalibrantdspacings.h"
#include "qxrdexperiment.h"
#include "qxrdwindowsettings.h"
#include "qxrddetectorcontrolwindowsettings.h"
#include "qxrdwelcomewindow.h"
#include "qxrdexperiment.h"
#include "qxrdexperimentthread.h"
#include "qxrdwindow.h"
#include "qxrdmainwindowsettings.h"
#include <QCommandLineParser>
#include <QDir>
#include <QThread>
#include <QMessageBox>
#include <QDesktopServices>
#include <QApplication>
#include <QMetaObject>
#include <QMetaEnum>
#include "qcepallocator.h"
#include "qxrdstartupwindow.h"
#include "qxrdstartupwindowsettings.h"

QxrdAppCommon::QxrdAppCommon(int &argc, char **argv)
  : inherited(argc, argv),
    m_CmdList(this, "cmdList", QStringList(), "Commands to Execute"),
    m_FileList(this, "fileList", QStringList(), "Files to Process"),
    m_WatcherList(this, "watcherList", QStringList(), "File patterns to watch for"),
    m_PluginList(this, "pluginList", QStringList(), "Plugin directories"),
    m_Debug(this,"debug", 0, "Debug Level"),
    m_OpenNew(this,"openNew", 0, "Open a new experiment"),
    m_FreshStart(this,"freshStart", 0, "Do a Fresh Start"),
    m_LoadPlugins(this, "loadPlugins", 1, "Load plugins"),
    m_StartDetectors(this, "startDetectors", 1, "Start Detectors when opening experiments"),
    m_CurrentDirectory(this, "currentDirectory", QDir::homePath(), "Current Directory"),
    //  m_OpenDirectly(m_Saver, this,"openDirectly", false, "Open Last Experiment at Startup"),
    m_FileBrowserLimit(this, "fileBrowserLimit", 1000, "Max Number of Files in Browser Windows (0 = unlimited)"),
    m_MessageWindowLines(this, "messageWindowLines", 1000, "Number of Lines in Message Window (0 = unlimited)"),
    m_LockerCount(this, "lockerCount", 0, "Number of mutex locks taken"),
    m_LockerRate(this, "lockerRate", 0, "Mutex Locking Rate"),
//    m_Splash(NULL),
    m_WelcomeWindow(NULL)
{
  QcepProperty::registerMetaTypes();
  QxrdPowderPoint::registerMetaTypes();
  QxrdPowderPointVector::registerMetaTypes();
  QxrdCalibrantDSpacing::registerMetaTypes();
  QxrdCalibrantDSpacings::registerMetaTypes();
  QxrdExperiment::registerMetaTypes();
  QxrdWindowSettings::registerMetaTypes();
  QxrdDetectorControlWindowSettings::registerMetaTypes();
  QxrdDetectorSettings::registerMetaTypes();

  m_Allocator = QcepAllocatorPtr(
        new QcepAllocator("allocator"));
}

QxrdAppCommon::~QxrdAppCommon()
{
}

void QxrdAppCommon::initializeRoot()
{
  THREAD_CHECK;

  inherited::initializeRoot();

  if (m_Allocator) {
    m_Allocator->initialize(sharedFromThis());
  }

  m_StartupWindowSettings =
      qSharedPointerDynamicCast<QxrdStartupWindowSettings>(
        appendWindowSettings(
          QxrdStartupWindowSettingsPtr(
            new QxrdStartupWindowSettings("startupWindowSettings"))));

  if (m_StartupWindowSettings) {
    m_StartupWindowSettings->initialize(sharedFromThis());
  }

  connect(prop_Debug(), &QcepInt64Property::valueChanged,
          this,         &QxrdAppCommon::debugChanged);
  readApplicationSettings();

  parseCommandLine(true);

  if (get_GuiWanted()) {
    openStartupWindow();
  }

  processEvents();

  QThread::currentThread()->setObjectName("applicationThread");

  connect(m_Application.data(),  &QApplication::aboutToQuit,
          this,                  &QxrdAppCommon::finish);
  connect(&m_SplashTimer,        &QTimer::timeout,
          this,                  &QxrdAppCommon::hideSplash);

  setupTiffHandlers();
}

void QxrdAppCommon::finish()
{
  THREAD_CHECK;

  foreach(QxrdExperimentPtr expt, m_Experiments) {
    if (expt) {
      if (expt->isChanged() || expt->wasAutoSaved()) {
        expt->saveExperiment();
      }

      expt->closeWindows();
    }
  }

  while (!m_ExperimentThreads.isEmpty()) {
    QxrdExperimentThreadPtr t = m_ExperimentThreads.takeFirst();

    if (t) {
      t->quit();
      t->wait();
    }
  }
}

QxrdAppCommonWPtr QxrdAppCommon::findApplication(QcepObjectWPtr p)
{
  QxrdAppCommonWPtr res =
      qSharedPointerDynamicCast<QxrdAppCommon>(p);

  if (res == NULL) {
    QcepObjectPtr objp(p);

    if (objp) {
      res = findApplication(objp->parentPtr());
    }
  }

  return res;
}

QcepAllocatorWPtr QxrdAppCommon::allocator() const
{
  return m_Allocator;
}

void QxrdAppCommon::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

  if (m_Allocator) {
    settings->beginGroup("allocator");
    m_Allocator->readSettings(settings);
    settings->endGroup();
  }

  if (m_StartupWindowSettings) {
    settings->beginGroup("startupWindowSettings");
    m_StartupWindowSettings->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdAppCommon::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

  if (m_Allocator) {
    settings->beginGroup("allocator");
    m_Allocator->writeSettings(settings);
    settings->endGroup();
  }

  if (m_StartupWindowSettings) {
    settings->beginGroup("startupWindowSettings");
    m_StartupWindowSettings->writeSettings(settings);
    settings->endGroup();
  }
}

void QxrdAppCommon::appendCommand(QString cmd)
{
  prop_CmdList()->appendValue(cmd);
}

void QxrdAppCommon::appendScript(QString script)
{
  prop_CmdList()->appendValue(tr("loadScript(\"%1\")").arg(script));
}

void QxrdAppCommon::appendFile(QString file)
{
  prop_FileList()->appendValue(file);
}

void QxrdAppCommon::appendWatcher(QString patt)
{
  prop_WatcherList()->appendValue(patt);
}

void QxrdAppCommon::appendPlugin(QString dir)
{
  prop_PluginList()->appendValue(dir);
}

void QxrdAppCommon::parseCommandLine(bool wantFullOptions)
{
  THREAD_CHECK;

  QStringList args(QCoreApplication::arguments());

  QCommandLineParser parser;
  parser.setApplicationDescription(applicationDescription());
  parser.addHelpOption();
  parser.addVersionOption();

  parser.addPositionalArgument("files", "Files to open, optionally", "[file...]");

  QCommandLineOption newOption({"n", "new"},      "Open new experiment");
  if (wantFullOptions) {
    parser.addOption(newOption);
  }

  QCommandLineOption freshOption({"f", "fresh"},  "Fresh start");
  if (wantFullOptions) {
    parser.addOption(freshOption);
  }

  QCommandLineOption debugOption({"d", "debug"},  "Set debug level", "debugLevel");
  parser.addOption(debugOption);

  QCommandLineOption debugListOption("Dlist", "List available debug flags");
  parser.addOption(debugListOption);

  QCommandLineOption debugNoneOption("Dnone", "Unset all debug flags");
  parser.addOption(debugNoneOption);

  QList<QCommandLineOption> debugFlagOptions;

  for (int i=0; i<debugFlagCount(); i++) {
    QString opt = debugFlagOption(i);
    QString dsc = debugFlagDescription(i);

    debugFlagOptions.append(
          QCommandLineOption(opt.mid(1), dsc));
  }

  parser.addOptions(debugFlagOptions);

  QCommandLineOption noGuiOption("nogui", "No GUI");
  if (wantFullOptions) {
    parser.addOption(noGuiOption);
  }

  QCommandLineOption guiOption("gui", "Want GUI");
  if (wantFullOptions) {
    parser.addOption(guiOption);
  }

  QCommandLineOption noStartOption("nostart", "Don't start detectors");
  if (wantFullOptions) {
    parser.addOption(noStartOption);
  }

  QCommandLineOption startOption("start", "Start detectors");
  if (wantFullOptions) {
    parser.addOption(startOption);
  }

  QCommandLineOption cmdOption({"c", "command"},  "Execute command (may be repeated)" , "command");
  parser.addOption(cmdOption);

  QCommandLineOption scriptOption({"s", "script"}, "Read script file (may be repeated)", "scriptfile");
  parser.addOption(scriptOption);

  QCommandLineOption watchOption({"w", "watch"}, "Watch directory/path for changes (may be repeated)", "pattern");
  parser.addOption(watchOption);

  QCommandLineOption pluginOption("p", "Special plugin load");
  if (wantFullOptions) {
    parser.addOption(pluginOption);
  }

  QCommandLineOption pluginDirOption({"P", "plugin"}, "Extra Plugin Search Directory (may be repeated)", "dir");
  if (wantFullOptions) {
    parser.addOption(pluginDirOption);
  }

  QCommandLineOption noPluginOption("noplugins", "Don't load plugins");
  if (wantFullOptions) {
    parser.addOption(noPluginOption);
  }

  parser.setSingleDashWordOptionMode(
        QCommandLineParser::ParseAsLongOptions);

  parser.process(args);

  if (wantFullOptions && parser.isSet(newOption)) {
    set_OpenNew(true);
  }

  if (wantFullOptions && parser.isSet(freshOption)) {
    set_FreshStart(true);
  }

  if (parser.isSet(debugOption)) {
    qint64 dbg;
    if (sscanf(qPrintable(parser.value(debugOption)),
               "%lld", &dbg)==1) {
      set_Debug(dbg);
    }
  }

  qint64 dbg = get_Debug();
  bool dbgChanged = false;

  if (parser.isSet(debugNoneOption)) {
    dbg = 0;
    dbgChanged = true;
  }

  for (int i=0; i<debugFlagCount(); i++) {
    if (parser.isSet(debugFlagOptions[i])) {
      dbg |= qint64(1) << i;
      dbgChanged = true;
    }
  }

  if (dbgChanged) {
    set_Debug(dbg);

#ifndef QT_NO_DEBUG
    printf(" new debug level 0x%llx\n", get_Debug());
#endif
  }

  if (parser.isSet(debugListOption)) {
    listDebugFlags();
    ::exit(0);
  }

  if (wantFullOptions && parser.isSet(noGuiOption)) {
    set_GuiWanted(false);
  }

  if (wantFullOptions && parser.isSet(guiOption)) {
    set_GuiWanted(true);
  }

  if (wantFullOptions && parser.isSet(noStartOption)) {
    set_StartDetectors(false);
  }

  if (wantFullOptions && parser.isSet(startOption)) {
    set_StartDetectors(true);
  }

  if (parser.isSet(cmdOption) || parser.isSet(scriptOption)) {
    // Want to preserve order of -c and -s options
    QStringList cnam = cmdOption.names();
    QStringList snam = scriptOption.names();
    QStringList opts = parser.optionNames();
    QStringList cmds = parser.values(cmdOption);
    QStringList scrp = parser.values(scriptOption);

    for (int i=0, ic=0, is=0; i<opts.size(); i++) {
      QString opt = opts.value(i);

      foreach( QString nam, cnam) {
        if (opt == nam) {
          QString cmd = cmds.value(ic++);

          printMessage(tr("cmd: %1").arg(cmd));

          appendCommand(cmd);
        }
      }

      foreach(QString nam, snam) {
        if (opt == nam) {
          QString script = scrp.value(is++);

          printMessage(tr("Script: %1").arg(script));

          appendScript(script);
        }
      }
    }
  }

  if (parser.isSet(watchOption)) {
    QStringList watches(parser.values(watchOption));

    foreach(QString w, watches) {
      printMessage(tr("Watch: \"%1\"").arg(w));

      appendWatcher(w);
    }
  }

  if (wantFullOptions && parser.isSet(pluginOption)) {
    QDir appDir(qApp->applicationDirPath());

    appDir.cd("plugins");

    appendPlugin(appDir.absolutePath());
  }

  if (wantFullOptions && parser.isSet(pluginDirOption)) {
    QStringList plugins(parser.values(pluginDirOption));

    foreach(QString ds, plugins) {
      QDir d(ds);

      appendPlugin(d.absolutePath());
    }
  }

  if (wantFullOptions && parser.isSet(noPluginOption)) {
    set_LoadPlugins(false);
  }

  QStringList files(parser.positionalArguments());

  foreach(QString f, files) {
    printMessage(tr("File: %1").arg(f));

    appendFile(f);
  }
}

void QxrdAppCommon::openWelcomeWindow()
{
  GUI_THREAD_CHECK;

  if (m_WelcomeWindow == NULL) {
    m_WelcomeWindow = new QxrdWelcomeWindow(qSharedPointerDynamicCast<QxrdAppCommon>(sharedFromThis()));
    m_WelcomeWindow->setAttribute(Qt::WA_DeleteOnClose, true);
  }

  m_WelcomeWindow -> show();
}

void QxrdAppCommon::closeWelcomeWindow()
{
  GUI_THREAD_CHECK;

  if (m_WelcomeWindow) {
    m_WelcomeWindow -> hide();
  }
}

void QxrdAppCommon::splashMessage(QString msg, QDateTime dt)
{
//  GUI_THREAD_CHECK;

  if (get_GuiWanted()) {
    if (m_StartupWindow) {
      INVOKE_CHECK(
            QMetaObject::invokeMethod(m_StartupWindow.data(),
                                      "appendSplashMessage",
                                      Qt::QueuedConnection,
                                      Q_ARG(QString, msg),
                                      Q_ARG(QDateTime, dt)));
    }

    processEvents();
  }
}

void QxrdAppCommon::printMessage(QString msg, QDateTime dt) const
{
  if (get_GuiWanted()) {
    if (m_StartupWindow) {
      INVOKE_CHECK(
            QMetaObject::invokeMethod(m_StartupWindow.data(),
                                      "appendMessage",
                                      Qt::QueuedConnection,
                                      Q_ARG(QString, msg),
                                      Q_ARG(QDateTime, dt)));

      processEvents();
    }
  }
}

void QxrdAppCommon::hideSplash()
{
  GUI_THREAD_CHECK;

  if (m_StartupWindow) {
    m_StartupWindow -> hide();
  }
}

void QxrdAppCommon::tiffWarning(const char *module, const char *msg)
{
  criticalMessage(tr("TIFF Warning from %1 : %2").arg(module).arg(msg));
}

void QxrdAppCommon::tiffError(const char *module, const char *msg)
{
  criticalMessage(tr("TIFF Error from %1 : %2").arg(module).arg(msg));
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

  QxrdAppCommon *app = qobject_cast<QxrdAppCommon*>(g_Application);

  if (app) {
    app -> tiffError(module, msg);
  }
}

void QxrdAppCommon::setupTiffHandlers()
{
  THREAD_CHECK;

  qcepTIFFSetErrorHandler      (&qxrdTIFFErrorHandler);
  qcepTIFFSetErrorHandlerExt   (NULL);
  qcepTIFFSetWarningHandler    (&qxrdTIFFWarningHandler);
  qcepTIFFSetWarningHandlerExt (NULL);
}

void QxrdAppCommon::openFile(QString filePath)
{
  QDir dir;

  QStringList files = dir.entryList(QStringList(filePath));

  if (files.length() == 0) {
    printMessage(tr("No files match \"%1\"").arg(filePath));
  } else {
    foreach (QString f, files) {
      printMessage(tr("Open file \"%1\" (to be written)").arg(f));
    }
  }
}

void QxrdAppCommon::openWatcher(QString pattern)
{
  QxrdExperimentPtr expt(qSharedPointerDynamicCast<QxrdExperiment>(getFirstExperiment()));

  if (expt) {
    INVOKE_CHECK(
          QMetaObject::invokeMethod(
            expt.data(), "openWatcher", Q_ARG(QString, pattern)));
  }
}

void QxrdAppCommon::openWindow(QxrdMainWindowSettingsWPtr set)
{
  GUI_THREAD_CHECK;

  QxrdMainWindowSettingsPtr s(set);

  if (s) {
    s->openWindow();
  }
}

void QxrdAppCommon::doAboutQxrd()
{
  QString about = applicationDescription();

  about += "\nVersion " + applicationVersion();

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

  QMessageBox::about(NULL, applicationName(), about);
}

void QxrdAppCommon::doOpenQXRDWebPage()
{
  QDesktopServices::openUrl(QUrl("http://qxrd.sourceforge.net/"));
}

void QxrdAppCommon::doOpenURL(QString url)
{
  QDesktopServices::openUrl(QUrl(url));
}

QcepExperimentPtr QxrdAppCommon::getFirstExperiment()
{
  if (experiments().count() == 0) {
    createNewExperiment();
  }

  return experiment(0);
}

void QxrdAppCommon::openRecentExperiment(QString path)
{
  QFileInfo info(path);

  if (info.exists()) {
    openExperiment(path);
  } else {
    printMessage(tr("Experiment %1 does not exist").arg(path));
  }
}

void QxrdAppCommon::closeExperiment(QxrdExperimentWPtr expw)
{
  if (qcepDebug(DEBUG_APP)) {
    printf("QxrdAppCommon::closeExperiment(%p)\n", expw.data());
  }

  QxrdExperimentPtr exp(expw);

  if (exp) {
    closedExperiment(exp->experimentThread());
  }
}

void QxrdAppCommon::appendRecentExperiment(QString path)
{
  QStringList recent = get_RecentExperiments();

  recent.prepend(path);
  recent.removeDuplicates();

  while(recent.length() > get_RecentExperimentsSize()) {
    recent.removeLast();
  }

  set_RecentExperiments(recent);
}

void QxrdAppCommon::openedExperiment(QxrdExperimentThreadWPtr expwthr)
{
  QxrdExperimentThreadPtr exptthr(expwthr);

  if (exptthr) {
    QxrdExperimentPtr expt(exptthr->experiment());

    if (expt) {
      QString path = expt->experimentFilePath();
      set_CurrentExperiment(path);
      appendRecentExperiment(path);

      m_ExperimentThreads.append(exptthr);
      m_Experiments.append(expt);

      printMessage("New experiment loaded");

      closeWelcomeWindow();

      expt->openWindows();
      expt->setChanged(0);
    }
  }
}

void QxrdAppCommon::closedExperiment(QxrdExperimentThreadWPtr expwthr)
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

QList<QxrdExperimentWPtr> &QxrdAppCommon::experiments()
{
  return m_Experiments;
}

QcepExperimentPtr QxrdAppCommon::experiment(int i) const
{
  return m_Experiments.value(i);
}

void QxrdAppCommon::possiblyQuit()
{
  if (wantToQuit()) {
//    finish();
    quit();
  }
}

bool QxrdAppCommon::wantToQuit()
{
  return QMessageBox::question(NULL, tr("Really Quit?"),
                               tr("Do you really want to exit the application?"),
                               QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
}

void QxrdAppCommon::debugChanged(qint64 newValue)
{
  if (g_DebugLevel) {
    printMessage(tr("Debug level changed from %1 to %2").arg(g_DebugLevel->debugLevel()).arg(newValue));

    g_DebugLevel->setDebugLevel(newValue);
  }
}

int QxrdAppCommon::debugFlag(QString f)
{
  int n1 = QxrdDebugDictionary::debugFlagCheck(f);

  return n1;
}

QString QxrdAppCommon::debugFlagName(int i)
{
  QString res = QxrdDebugDictionary::debugFlagName(i);

  return res;
}

int QxrdAppCommon::debugFlagCount()
{
  return QxrdDebugDictionary::debugFlagCount();
}

QString QxrdAppCommon::debugFlagOption(int i)
{
  QString res;
  QString name = debugFlagName(i);

  if (name.startsWith("DEBUG_")) {
    res = tr("-D%1").arg(name.mid(6));
  }

  return res;
}

QString QxrdAppCommon::debugFlagDescription(int i)
{
  QString res;

  if (g_DebugLevel) {
    res = g_DebugLevel -> message(i);
  }

  return res;
}

void QxrdAppCommon::listDebugFlags()
{
  for (int i=0; i<debugFlagCount(); i++) {
    printf("%-20s : %s\n",
           qPrintable(debugFlagOption(i)),
           qPrintable(debugFlagDescription(i)));
  }
}
