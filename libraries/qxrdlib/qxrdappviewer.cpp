#include "qxrdappviewer.h"
#include "qcepdataobject.h"
#include "qxrdappviewersettings.h"
#include "qxrdviewersettings.h"
#include "qxrdexperimentthread.h"
#include "qxrdexperiment.h"

QxrdAppViewer::QxrdAppViewer(int &argc, char **argv)
  : inherited(argc, argv)
{
  setSettings(QxrdAppViewerSettingsPtr(
        new QxrdAppViewerSettings(
                  "qxrdViewerSettings",
                  argc,
                  argv)));
}

QxrdAppViewer::~QxrdAppViewer()
{
}

void QxrdAppViewer::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

bool QxrdAppViewer::init(int &argc, char **argv)
{
  inherited::init(argc, argv);

  if (settings()) {
    parseCommandLine(false);

    int nWatches = settings() -> get_WatcherList().length();
    int nFiles   = settings() -> get_FileList().length();

    if (nFiles > 0 || nWatches > 0) {
      foreach(QString file, settings()->get_FileList()) {
        openFile(file);
      }

      foreach(QString patt, settings()->get_WatcherList()) {
        openWatcher(patt);
      }
    } else {
      openWatcher(".");
    }
  }

  return true;
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

QxrdAppViewerSettingsPtr QxrdAppViewer::settings()
{
  return qSharedPointerDynamicCast<QxrdAppViewerSettings>(
        inherited::settings());
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

  if (settings()) {
    set.beginGroup("application");
    settings() -> readSettings(&set);
    set.endGroup();
  }
}

void QxrdAppViewer::writeApplicationSettings()
{
  QxrdViewerSettings set(this);

  if (settings()) {
    set.beginGroup("application");
    settings() -> writeSettings(&set);
    set.endGroup();
    settings() -> setChanged(0);
  }
}
