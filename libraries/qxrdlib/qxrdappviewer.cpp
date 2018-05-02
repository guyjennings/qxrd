#include "qxrdappviewer.h"
#include "qcepdataobject.h"
#include "qxrdappcommon.h"
#include "qxrdexperimentthread.h"
#include "qxrdexperiment.h"
#include "qxrdviewersettings.h"
#include "qcepstartupwindow.h"
#include "qcepstartupwindowsettings.h"

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

void QxrdAppViewer::parseCommandLine()
{
  GUI_THREAD_CHECK;

  inherited::parseCommandLine();
}

void QxrdAppViewer::criticalMessage(QString msg, QDateTime ts) const
{
  printf("CRITICAL: %s\n", qPrintable(msg));
}

QString QxrdAppViewer::applicationName()
{
  return QStringLiteral("QXRDVIEWER");
}

QString QxrdAppViewer::applicationMnemonic()
{
  return QStringLiteral("qxrdviewer");
}

QString QxrdAppViewer::applicationVersion()
{
  return QStringLiteral(STR(QXRD_VERSION));
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
      QxrdExperimentThreadPtr(
        NEWPTR(QxrdExperimentThread("experimentThread")));

  if (expthr) {
    expthr -> initialize(sharedFromThis(),
                         "",
                         QxrdExperimentSettingsPtr(),
                         QxrdExperiment::ViewerOnly);
    expthr -> start();

    openedExperiment(expthr);

    closeWelcomeWindow();
  }
}

void QxrdAppViewer::chooseExistingExperiment()
{
}

//void QxrdAppViewer::readApplicationSettings()
//{
//  QxrdViewerSettings set(this);

//  set.beginGroup("application");
//  readSettings(&set);
//  set.endGroup();
//}

//void QxrdAppViewer::writeApplicationSettings()
//{
//  QxrdViewerSettings set(this);

//  set.beginGroup("application");
//  writeSettings(&set);
//  set.endGroup();
//  setChanged(0);
//}

QSettingsPtr QxrdAppViewer::applicationSettings()
{
  QSettingsPtr res =
      QSettingsPtr(NEWPTR(QxrdViewerSettings(this)));

  return res;
}
