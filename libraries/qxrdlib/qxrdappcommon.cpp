#include "qxrdappcommon.h"
#include "qxrddebug.h"
#include "qcepimagedataformattiff.h"
#include "qxrdsplashscreen.h"
#include "qxrdpowderpoint.h"
#include "qxrdcalibrantdspacing.h"
#include "qxrdcalibrantdspacings.h"
#include "qxrdexperiment.h"
#include "qxrdtestgenerator.h"
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
#include "qcepallocator.h"

QxrdAppCommon::QxrdAppCommon(int &argc, char **argv)
  : inherited(argc, argv),
    m_Argc(this, "argc", argc, "Number of Command Line Arguments"),
    m_Argv(this, "argv", makeStringListFromArgs(argc, argv), "Command Line Arguments"),
    m_GuiWanted(this, "guiWanted", 1, "GUI Wanted?"),
    m_CmdList(this, "cmdList", QStringList(), "Commands to Execute"),
    m_FileList(this, "fileList", QStringList(), "Files to Process"),
    m_WatcherList(this, "watcherList", QStringList(), "File patterns to watch for"),
    m_PluginList(this, "pluginList", QStringList(), "Plugin directories"),
    m_Debug(this,"debug", 0, "Debug Level"),
    m_OpenNew(this,"openNew", 0, "Open a new experiment"),
    m_FreshStart(this,"freshStart", 0, "Do a Fresh Start"),
    m_StartDetectors(this, "startDetectors", 1, "Start Detectors when opening experiments"),
    m_CurrentExperiment(this, "currentExperiment", "", "Current Experiment"),
    m_RecentExperiments(this, "recentExperiments", QStringList(), "Recent Experiments"),
    m_RecentExperimentsSize(this,"recentExperimentsSize", 8, "Number of Recent Experiments to Remember"),
    m_ExperimentCount(this, "experimentCount", 0, "Number of open experiments"),
    m_CurrentDirectory(this, "currentDirectory", QDir::homePath(), "Current Directory"),
    //  m_OpenDirectly(m_Saver, this,"openDirectly", false, "Open Last Experiment at Startup"),
    m_FileBrowserLimit(this, "fileBrowserLimit", 1000, "Max Number of Files in Browser Windows (0 = unlimited)"),
    m_MessageWindowLines(this, "messageWindowLines", 1000, "Number of Lines in Message Window (0 = unlimited)"),
    m_UpdateIntervalMsec(this, "updateIntervalMsec", 1000, "Time Intervale for Updates (in msec)"),
    m_LockerCount(this, "lockerCount", 0, "Number of mutex locks taken"),
    m_LockerRate(this, "lockerRate", 0, "Mutex Locking Rate"),
    m_Splash(NULL),
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
  QxrdTestGenerator::registerMetaTypes();
  QxrdDetectorSettings::registerMetaTypes();

  m_Allocator = QcepAllocatorPtr(
        new QcepAllocator("allocator"));
}

QxrdAppCommon::~QxrdAppCommon()
{
}

void QxrdAppCommon::initialize(QcepObjectWPtr parent)
{
  THREAD_CHECK;

  QThread::currentThread()->setObjectName("applicationThread");

  inherited::initialize(parent);

  if (m_Allocator) {
    m_Allocator->initialize(parent);
  }

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
}

void QxrdAppCommon::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

  if (m_Allocator) {
    settings->beginGroup("allocator");
    m_Allocator->writeSettings(settings);
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

  QCommandLineOption newOption({"n", "new"},      QCoreApplication::translate("main", "Open new experiment"));
  if (wantFullOptions) {
    parser.addOption(newOption);
  }

  QCommandLineOption freshOption({"f", "fresh"},  QCoreApplication::translate("main", "Fresh start"));
  if (wantFullOptions) {
    parser.addOption(freshOption);
  }

  QCommandLineOption debugOption({"d", "debug"},
                                 QCoreApplication::translate("main", "Set debug level"),
                                 QCoreApplication::translate("main", "debugLevel"));
  parser.addOption(debugOption);

  QCommandLineOption noGuiOption("nogui", QCoreApplication::translate("main", "No GUI"));
  if (wantFullOptions) {
    parser.addOption(noGuiOption);
  }

  QCommandLineOption guiOption("gui", QCoreApplication::translate("main", "Want GUI"));
  if (wantFullOptions) {
    parser.addOption(guiOption);
  }

  QCommandLineOption noStartOption("nostart", QCoreApplication::translate("main", "Don't start detectors"));
  if (wantFullOptions) {
    parser.addOption(noStartOption);
  }

  QCommandLineOption startOption("start", QCoreApplication::translate("main", "Start detectors"));
  if (wantFullOptions) {
    parser.addOption(startOption);
  }

  QCommandLineOption cmdOption({"c", "command"},
                               QCoreApplication::translate("main", "Execute command (may be repeated)"),
                               QCoreApplication::translate("main", "command"));
  parser.addOption(cmdOption);

  QCommandLineOption scriptOption({"s", "script"},
                                  QCoreApplication::translate("main", "Read script file (may be repeated)"),
                                  QCoreApplication::translate("main", "scriptfile"));
  parser.addOption(scriptOption);

  QCommandLineOption watchOption({"w", "watch"},
                                 QCoreApplication::translate("main", "Watch directory/path for changes (may be repeated)"),
                                 QCoreApplication::translate("main", "pattern"));
  parser.addOption(watchOption);

  QCommandLineOption pluginOption("p", QCoreApplication::translate("main", "Special plugin load"));
  if (wantFullOptions) {
    parser.addOption(pluginOption);
  }

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

void QxrdAppCommon::splashMessage(QString msg)
{
  GUI_THREAD_CHECK;

  if (get_GuiWanted()) {
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

void QxrdAppCommon::hideSplash()
{
  GUI_THREAD_CHECK;

  if (m_Splash) {
    m_Splash -> hide();
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
  QxrdExperimentPtr expt(getFirstExperiment());

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

  about += "\nVersion " STR(QXRD_VERSION);

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

void QxrdAppCommon::doOpenQXRDWebPage()
{
  QDesktopServices::openUrl(QUrl("http://qxrd.sourceforge.net/"));
}

void QxrdAppCommon::doOpenURL(QString url)
{
  QDesktopServices::openUrl(QUrl(url));
}

QxrdExperimentPtr QxrdAppCommon::getFirstExperiment()
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

      printMessage("");
      printMessage("New experiment loaded");
      printMessage("");

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

QxrdExperimentPtr QxrdAppCommon::experiment(int i) const
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

QStringList QxrdAppCommon::makeStringListFromArgs(int argc, char **argv)
{
  QStringList res;

  for (int i=0; i<argc; i++) {
    res.append(argv[i]);
  }

  return res;
}


