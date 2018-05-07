#include "qxrdappcommon.h"
#include "qxrddebug.h"
#include "qcepimagedataformattiff.h"
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

QxrdAppCommon::QxrdAppCommon(int &argc, char **argv)
  : inherited(argc, argv),
    m_WatcherList(this, "watcherList", QStringList(), "File patterns to watch for"),
    m_StartDetectors(this, "startDetectors", 1, "Start Detectors when opening experiments"),
    m_CurrentDirectory(this, "currentDirectory", QDir::homePath(), "Current Directory"),
    //  m_OpenDirectly(m_Saver, this,"openDirectly", false, "Open Last Experiment at Startup"),
    m_FileBrowserLimit(this, "fileBrowserLimit", 1000, "Max Number of Files in Browser Windows (0 = unlimited)"),
//    m_Splash(NULL),
    m_WelcomeWindow(NULL)
{
  QxrdExperiment::registerMetaTypes();
  QxrdWindowSettings::registerMetaTypes();
  QxrdDetectorControlWindowSettings::registerMetaTypes();
  QxrdDetectorSettings::registerMetaTypes();
}

QxrdAppCommon::~QxrdAppCommon()
{
}

void QxrdAppCommon::initializeRoot()
{
  GUI_THREAD_CHECK;

  inherited::initializeRoot();

//  CONNECT_CHECK(connect(m_Application.data(),  &QApplication::aboutToQuit,
//          this,                  &QxrdAppCommon::shutdownDocuments));
//  CONNECT_CHECK(connect(&m_SplashTimer,        &QTimer::timeout,
//          this,                  &QxrdAppCommon::hideSplash));

  setupTiffHandlers();
}

void QxrdAppCommon::shutdownDocuments()
{
  printf("QxrdAppCommon::shutdownDocuments\n");

  GUI_THREAD_CHECK;

  foreach(QxrdExperimentPtr expt, m_Experiments) {
    if (expt) {
#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
      INVOKE_CHECK(
            QMetaObject::invokeMethod(expt.data(),
                                      &QxrdExperiment::shutdownAndSave,
                                      Qt::BlockingQueuedConnection));
#else
      INVOKE_CHECK(
            QMetaObject::invokeMethod(expt.data(),
                                      "shutdownAndSave",
                                      Qt::BlockingQueuedConnection));
#endif

      expt->closeWindows();
    }
  }

//  while (!m_ExperimentThreads.isEmpty()) {
//    QxrdExperimentThreadPtr t = m_ExperimentThreads.takeFirst();

//    if (t) {
//      t->shutdown();
//    }
//  }

  for (int i=0; i<m_ExperimentThreads.count(); i++) {
    QxrdExperimentThreadPtr expt =
        m_ExperimentThreads.value(i);

    if (expt) {
      expt -> haltExperiment();
    }
  }

  inherited::shutdownDocuments();
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

void QxrdAppCommon::appendWatcher(QString patt)
{
  prop_WatcherList()->appendValue(patt);
}

void QxrdAppCommon::parseCommandLine()
{
  THREAD_CHECK;

  QCommandLineOption watchOption({"w", "watch"}, "Watch directory/path for changes (may be repeated)", "pattern");
  addCommandLineOption(&watchOption);

  inherited::parseCommandLine();

  if (m_CommandLineParser -> isSet(watchOption)) {
    QStringList watches(m_CommandLineParser -> values(watchOption));

    foreach(QString w, watches) {
      printMessage(tr("Watch: \"%1\"").arg(w));

      appendWatcher(w);
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
      expt->setupAcquisition();
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
