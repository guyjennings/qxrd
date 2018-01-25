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

  m_AppViewerSettings = QxrdAppViewerSettingsPtr(
        new QxrdAppViewerSettings(qSharedPointerDynamicCast<QxrdAppViewer>(sharedFromThis()), argc, argv));

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

QxrdAppViewerSettingsWPtr QxrdAppViewer::settings()
{
  return m_AppViewerSettings;
}
