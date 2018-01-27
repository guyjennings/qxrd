#include "qxrdappcommon.h"
#include "qcepimagedataformattiff.h"
#include "qxrdappcommonsettings.h"
#include "qxrdsplashscreen.h"
#include "qxrdpowderpoint.h"
#include "qxrdcalibrantdspacing.h"
#include "qxrdcalibrantdspacings.h"
#include "qxrdexperiment.h"
#include "qxrdtestgenerator.h"
#include "qxrdwindowsettings.h"
#include "qxrddetectorcontrolwindowsettings.h"
#include "qxrdwelcomewindow.h"
#include <QCommandLineParser>
#include <QDir>
#include <QThread>
#include <QMessageBox>

QxrdAppCommon::QxrdAppCommon(int &argc, char **argv)
  : inherited(argc, argv),
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
}

QxrdAppCommon::~QxrdAppCommon()
{
}

bool QxrdAppCommon::init(int &argc, char **argv)
{
  THREAD_CHECK;

  QThread::currentThread()->setObjectName("app");

  connect(this,           &QCoreApplication::aboutToQuit,
          this,           &QxrdAppCommon::finish);
  connect(&m_SplashTimer, &QTimer::timeout,
          this,           &QxrdAppCommon::hideSplash);

  setupTiffHandlers();

  return true;
}

void QxrdAppCommon::finish()
{
  THREAD_CHECK;

  //TODO: move stuff from QxrdApplication::finish
}

QxrdAppCommonSettingsPtr QxrdAppCommon::settings()
{
  return m_ApplicationSettings;
}

void QxrdAppCommon::setSettings(QxrdAppCommonSettingsPtr set)
{
  m_ApplicationSettings = set;
}

void QxrdAppCommon::parseCommandLine(bool wantFullOptions)
{
  THREAD_CHECK;

  if (m_ApplicationSettings) {
    QStringList args(QCoreApplication::arguments());

    QCommandLineParser parser;
    parser.setApplicationDescription(applicationDescription());
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("files", "Files to open, optionally", "[file...]");

    QCommandLineOption newOption({"n", "new"},      translate("main", "Open new experiment"));
    if (wantFullOptions) {
      parser.addOption(newOption);
    }

    QCommandLineOption freshOption({"f", "fresh"},  translate("main", "Fresh start"));
    if (wantFullOptions) {
      parser.addOption(freshOption);
    }

    QCommandLineOption debugOption({"d", "debug"},
                                   translate("main", "Set debug level"),
                                   translate("main", "debugLevel"));
    parser.addOption(debugOption);

    QCommandLineOption noGuiOption("nogui", translate("main", "No GUI"));
    if (wantFullOptions) {
      parser.addOption(noGuiOption);
    }

    QCommandLineOption guiOption("gui", translate("main", "Want GUI"));
    if (wantFullOptions) {
      parser.addOption(guiOption);
    }

    QCommandLineOption noStartOption("nostart", translate("main", "Don't start detectors"));
    if (wantFullOptions) {
      parser.addOption(noStartOption);
    }

    QCommandLineOption startOption("start", translate("main", "Start detectors"));
    if (wantFullOptions) {
      parser.addOption(startOption);
    }

    QCommandLineOption cmdOption({"c", "command"},
                                 translate("main", "Execute command (may be repeated)"),
                                 translate("main", "command"));
    parser.addOption(cmdOption);

    QCommandLineOption scriptOption({"s", "script"},
                                    translate("main", "Read script file (may be repeated)"),
                                    translate("main", "scriptfile"));
    parser.addOption(scriptOption);

    QCommandLineOption watchOption({"w", "watch"},
                                   translate("main", "Watch directory/path for changes (may be repeated)"),
                                   translate("main", "pattern"));
    parser.addOption(watchOption);

    QCommandLineOption pluginOption("p", translate("main", "Special plugin load"));
    if (wantFullOptions) {
      parser.addOption(pluginOption);
    }

    parser.process(args);

    if (wantFullOptions && parser.isSet(newOption)) {
      m_ApplicationSettings -> set_OpenNew(true);
    }

    if (wantFullOptions && parser.isSet(freshOption)) {
      m_ApplicationSettings -> set_FreshStart(true);
    }

    if (parser.isSet(debugOption)) {
      qint64 dbg;
      if (sscanf(qPrintable(parser.value(debugOption)),
                 "%lld", &dbg)==1) {
        m_ApplicationSettings -> set_Debug(dbg);
      }
    }

    if (wantFullOptions && parser.isSet(noGuiOption)) {
      m_ApplicationSettings -> set_GuiWanted(false);
    }

    if (wantFullOptions && parser.isSet(guiOption)) {
      m_ApplicationSettings -> set_GuiWanted(true);
    }

    if (wantFullOptions && parser.isSet(noStartOption)) {
      m_ApplicationSettings -> set_StartDetectors(false);
    }

    if (wantFullOptions && parser.isSet(startOption)) {
      m_ApplicationSettings -> set_StartDetectors(true);
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

            m_ApplicationSettings -> appendCommand(cmd);
          }
        }

        foreach(QString nam, snam) {
          if (opt == nam) {
            QString script = scrp.value(is++);

            printMessage(tr("Script: %1").arg(script));

            m_ApplicationSettings -> appendScript(script);
          }
        }
      }
    }

    if (parser.isSet(watchOption)) {
      QStringList watches(parser.values(watchOption));

      foreach(QString w, watches) {
        printMessage(tr("Watch: \"%1\"").arg(w));

        m_ApplicationSettings->appendWatcher(w);
      }
    }

    if (wantFullOptions && parser.isSet(pluginOption)) {
      QDir appDir(qApp->applicationDirPath());

      appDir.cd("plugins");

      m_ApplicationSettings->appendPlugin(appDir.absolutePath());
    }

    QStringList files(parser.positionalArguments());

    foreach(QString f, files) {
      printMessage(tr("File: %1").arg(f));

      m_ApplicationSettings -> appendFile(f);
    }
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
  printMessage(tr("Open file \"%1\" (to be written)").arg(filePath));
}

void QxrdAppCommon::openWatcher(QString pattern)
{
  printMessage(tr("Open watcher \"%1\" (to be written)").arg(pattern));
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

