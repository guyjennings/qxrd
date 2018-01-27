#include "qxrdappviewer.h"
#include "qcepdataobject.h"
#include "qxrdappviewersettings.h"

QxrdAppViewer::QxrdAppViewer(int &argc, char **argv)
  : inherited(argc, argv)
{

}

QxrdAppViewer::~QxrdAppViewer()
{
}

bool QxrdAppViewer::init(int &argc, char **argv)
{
  inherited::init(argc, argv);

  setSettings(QxrdAppViewerSettingsPtr(
        new QxrdAppViewerSettings(qSharedPointerDynamicCast<QxrdAppViewer>(sharedFromThis()), argc, argv)));

  if (settings()) {
    settings()->init();

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

void QxrdAppViewer::printMessage(QString msg, QDateTime ts)
{
}

void QxrdAppViewer::criticalMessage(QString msg, QDateTime ts)
{
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
}

void QxrdAppViewer::chooseExistingExperiment()
{
}
