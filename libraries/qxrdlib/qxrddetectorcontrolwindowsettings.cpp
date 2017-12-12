#include "qxrddetectorcontrolwindowsettings.h"
#include "qxrddetectorimageplot.h"

QxrdDetectorControlWindowSettings::QxrdDetectorControlWindowSettings(QxrdDetectorSettingsWPtr det)
  : QcepObject("detectorWindowSettings"),
    m_DetectorSettings(det),
    m_DetectorWindowOpen(this, "detectorWindowOpen", 0, "Detector Window Open?"),
    m_DetectorWindowGeometry(this, "detectorWindowGeometry", QByteArray(), "Detector Window Geometry"),
    m_DetectorWindowState(this, "detectorWindowState", QByteArray(), "Detector Window State"),
    m_DetectorWindowNewROIType(this, "detectorWindowNewROIType", QxrdDetectorImagePlot::NewRectROI, "Detector Window New ROI Type"),
    m_DetectorWindowRect(this, "detectorWindowRect", QRectF(), "Window Geometry")
{
}

QxrdDetectorControlWindowSettingsPtr QxrdDetectorControlWindowSettings::newDetectorWindowSettings(
    QxrdDetectorSettingsWPtr det)
{
  QxrdDetectorControlWindowSettingsPtr res(
        new QxrdDetectorControlWindowSettings(det));

  return res;
}

void QxrdDetectorControlWindowSettings::addChildPtr(QcepObjectPtr child)
{
  QcepObject::addChildPtr(child);
}

void QxrdDetectorControlWindowSettings::registerMetaTypes()
{
  qRegisterMetaType<QxrdDetectorControlWindowSettings*>("QxrdDetectorControlWindowSettings*");
}