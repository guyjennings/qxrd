#include "qxrddetectorcontrolwindowsettings.h"
#include "qxrddetectorimageplot.h"
#include "qxrddetectorplotwidgetsettings.h"

QxrdDetectorControlWindowSettings::QxrdDetectorControlWindowSettings(QxrdDetectorSettingsWPtr det)
  : QcepObject("detectorWindowSettings"),
    m_DetectorSettings(det),
    m_DetectorPlotWidgetSettings(new QxrdDetectorPlotWidgetSettings("detectorPlotWidget")),
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

QxrdDetectorPlotWidgetSettingsPtr QxrdDetectorControlWindowSettings::detectorPlotWidgetSettings()
{
  return m_DetectorPlotWidgetSettings;
}

void QxrdDetectorControlWindowSettings::addChildPtr(QcepObjectPtr child)
{
  QcepObject::addChildPtr(child);
}

void QxrdDetectorControlWindowSettings::registerMetaTypes()
{
  qRegisterMetaType<QxrdDetectorControlWindowSettings*>("QxrdDetectorControlWindowSettings*");
}

void QxrdDetectorControlWindowSettings::readSettings(QSettings *set)
{
  QcepObject::readSettings(set);

  set->beginGroup("detectorPlot");
  m_DetectorPlotWidgetSettings->readSettings(set);
  set->endGroup();
}

void QxrdDetectorControlWindowSettings::writeSettings(QSettings *set)
{
  QcepObject::writeSettings(set);

  set->beginGroup("detectorPlot");
  m_DetectorPlotWidgetSettings->writeSettings(set);
  set->endGroup();
}
