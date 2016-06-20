#include "qxrddetectorcontrolwindowsettings.h"

QxrdDetectorControlWindowSettings::QxrdDetectorControlWindowSettings(QxrdDetectorSettingsWPtr det)
  : QcepSerializableObject("detectorWindowSettings"),
    m_DetectorSettings(det),
    m_DetectorWindowOpen(this, "detectorWindowOpen", 0, "Detector Window Open?"),
    m_DetectorWindowGeometry(this, "detectorWindowGeometry", QByteArray(), "Detector Window Geometry"),
    m_DetectorWindowState(this, "detectorWindowState", QByteArray(), "Detector Window State")
{
}

QxrdDetectorControlWindowSettingsPtr QxrdDetectorControlWindowSettings::newDetectorWindowSettings(
    QxrdDetectorSettingsWPtr det)
{
  QxrdDetectorControlWindowSettingsPtr res(
        new QxrdDetectorControlWindowSettings(det));

  return res;
}

void QxrdDetectorControlWindowSettings::addChildPtr(QcepSerializableObjectPtr child)
{
  QcepSerializableObject::addChildPtr(child);
}

void QxrdDetectorControlWindowSettings::registerMetaTypes()
{
  qRegisterMetaType<QxrdDetectorControlWindowSettings*>("QxrdDetectorControlWindowSettings*");
}
