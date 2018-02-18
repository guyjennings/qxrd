#include "qxrdappviewer.h"
#include "qcepdataobject.h"
#include "qxrdappcommon.h"
#include "qxrdexperimentthread.h"
#include "qxrdexperiment.h"
#include "qxrdviewersettings.h"
#include "qxrdstartupwindow.h"

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
  m_StartupWindow = QxrdStartupWindowPtr(
        new QxrdStartupWindow());

  m_StartupWindow -> setApplicationIcon(QIcon(":/images/qxrdviewer-icon-128x128.png"));
  m_StartupWindow -> setApplicationDescription(
             "<h3>Data Viewer for 2-D XRay Detector data<h3>\n"
             "<h3>Guy Jennings<h3>\n"
             "<h3>Version " STR(QXRD_VERSION) "</h3>\n"
             "<p>Build : " __DATE__ " : " __TIME__ "</p>\n");

  m_StartupWindow -> setWindowTitle(applicationDescription());
  m_StartupWindow -> show();
  m_StartupWindow -> raise();
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

void QxrdAppViewer::printMessage(QString msg, QDateTime ts) const
{
  printf("%s\n", qPrintable(msg));
}

void QxrdAppViewer::criticalMessage(QString msg, QDateTime ts) const
{
  printf("CRITICAL: %s\n", qPrintable(msg));
}

QString QxrdAppViewer::applicationDescription()
{
  return QStringLiteral("QXRDVIEWER Data Viewer for 2-D XRay Detector data");
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
