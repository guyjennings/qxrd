#include "qxrdappviewer.h"
#include "qcepdataobject.h"
#include "qxrdappcommon.h"
#include "qxrdexperimentthread.h"
#include "qxrdexperiment.h"
#include "qxrdviewersettings.h"
#include "qxrdstartupwindow.h"
#include "qxrdstartupwindowsettings.h"

QxrdAppViewer::QxrdAppViewer(int &argc, char **argv)
  : inherited(argc, argv)
{
}

QxrdAppViewer::~QxrdAppViewer()
{
}

void QxrdAppViewer::initializeRoot()
{
  inherited::initializeRoot();

  parseCommandLine(false);

  int nWatches = get_WatcherList().length();
  int nFiles   = get_FileList().length();

  if (nFiles > 0 || nWatches > 0) {
    foreach(QString file, get_FileList()) {
      openFile(file);
    }

    foreach(QString patt, get_WatcherList()) {
      openWatcher(patt);
    }
  } else {
    openWatcher(".");
  }
}

void QxrdAppViewer::openStartupWindow()
{
  if (m_StartupWindowSettings) {
    m_StartupWindow =
        qSharedPointerDynamicCast<QxrdStartupWindow>(
          m_StartupWindowSettings -> newWindow());

    m_StartupWindow -> setApplicationIcon(QIcon(":/images/qxrdviewer-icon-128x128.png"));
    m_StartupWindow -> setApplicationDescription(
          "Data Viewer for 2-D XRay Detector data\n"
          "Guy Jennings\n"
          "Version " STR(QXRD_VERSION) "\n"
          "Build : " __DATE__ " : " __TIME__);

    m_StartupWindow -> setWindowTitle(applicationDescription());
    m_StartupWindow -> setWindowIcon(applicationIcon());
    m_StartupWindow -> show();
    m_StartupWindow -> raise();
  }
}

void QxrdAppViewer::closeStartupWindow()
{
}

void QxrdAppViewer::setDefaultObjectData(QcepDataObject *obj)
{
  if (obj) {
    obj->set_Creator("QXRD Viewer");
    obj->set_Version(STR(QXRD_VERSION));
    obj->set_QtVersion(QT_VERSION_STR);
  }
}

void QxrdAppViewer::criticalMessage(QString msg, QDateTime ts) const
{
  printf("CRITICAL: %s\n", qPrintable(msg));
}

QString QxrdAppViewer::applicationDescription()
{
  return QStringLiteral("QXRDVIEWER Data Viewer for 2-D XRay Detector data");
}

QIcon QxrdAppViewer::applicationIcon()
{
  return QIcon(":/images/qxrdviewer-icon-64x64.png");
}

void QxrdAppViewer::openExperiment(QString path)
{
}

void QxrdAppViewer::editGlobalPreferences()
{
}

void QxrdAppViewer::createNewExperiment()
{
  QxrdExperimentThreadPtr expthr =
      QxrdExperimentThread::newExperimentThread(
        "",
        qSharedPointerDynamicCast<QxrdAppCommon>(sharedFromThis()),
        QxrdExperimentSettingsPtr(),
        QxrdExperiment::ViewerOnly);

  if (expthr) {
    openedExperiment(expthr);

    closeWelcomeWindow();
  }
}

void QxrdAppViewer::chooseExistingExperiment()
{
}

void QxrdAppViewer::readApplicationSettings()
{
  QxrdViewerSettings set(this);

  set.beginGroup("application");
  readSettings(&set);
  set.endGroup();
}

void QxrdAppViewer::writeApplicationSettings()
{
  QxrdViewerSettings set(this);

  set.beginGroup("application");
  writeSettings(&set);
  set.endGroup();
  setChanged(0);
}
