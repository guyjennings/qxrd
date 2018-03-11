#include "qxrddetectorcontrolwindowsettings.h"
#include "qxrddetectorimageplot.h"
#include "qxrdimageplotwidgetsettings.h"
#include "qxrddetectorsettings.h"

QxrdDetectorControlWindowSettings::QxrdDetectorControlWindowSettings(QString name)
  : inherited(name),
    m_DetectorWindowOpen(this, "detectorWindowOpen", 0, "Detector Window Open?"),
    m_DetectorWindowGeometry(this, "detectorWindowGeometry", QByteArray(), "Detector Window Geometry"),
    m_DetectorWindowState(this, "detectorWindowState", QByteArray(), "Detector Window State"),
    m_DetectorWindowNewROIType(this, "detectorWindowNewROIType", QxrdDetectorImagePlot::NewRectROI, "Detector Window New ROI Type"),
    m_DetectorWindowRect(this, "detectorWindowRect", QRectF(), "Window Geometry")
{
  m_ImagePlotWidgetSettings =
      QxrdImagePlotWidgetSettingsPtr(
        new QxrdImagePlotWidgetSettings("detectorPlotWidget"));
}

void QxrdDetectorControlWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_DetectorSettings = QxrdDetectorSettings::findDetectorSettings(parent);

#ifndef QT_NO_DEBUG
  if (m_DetectorSettings == NULL) {
    printMessage("QxrdDetectorControlWindowSettings::m_DetectorSettings == NULL");
  }
#endif

  m_ImagePlotWidgetSettings -> initialize(parent);
}

QxrdImagePlotWidgetSettingsPtr QxrdDetectorControlWindowSettings::imagePlotWidgetSettings()
{
  return m_ImagePlotWidgetSettings;
}

void QxrdDetectorControlWindowSettings::registerMetaTypes()
{
  qRegisterMetaType<QxrdDetectorControlWindowSettings*>("QxrdDetectorControlWindowSettings*");
}

void QxrdDetectorControlWindowSettings::readSettings(QSettings *set)
{
  inherited::readSettings(set);

  set->beginGroup("detectorPlot");
  m_ImagePlotWidgetSettings->readSettings(set);
  set->endGroup();
}

void QxrdDetectorControlWindowSettings::writeSettings(QSettings *set)
{
  inherited::writeSettings(set);

  set->beginGroup("detectorPlot");
  m_ImagePlotWidgetSettings->writeSettings(set);
  set->endGroup();
}
