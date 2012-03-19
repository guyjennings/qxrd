#include "qxrdimageplotsettings.h"

QxrdImagePlotSettings::QxrdImagePlotSettings(QObject *parent) :
  QxrdPlotSettings(parent),
  m_DisplayMinimumPct(QxrdSettingsSaverPtr(), this, "displayMinimumPct", 0, "Minimum Displayed Value (as % of data range)"),
  m_DisplayMaximumPct(QxrdSettingsSaverPtr(), this, "displayMaximumPct", 100, "Maximum Displayed Value (as % of data range)"),
  m_DisplayMinimumVal(QxrdSettingsSaverPtr(), this, "displayMinimumVal", 0,  "Minimum Displayed Value (in absolute units)"),
  m_DisplayMaximumVal(QxrdSettingsSaverPtr(), this, "displayMaximumVal", 10000, "Maximum Displayed Value (in absoulte units)"),
  m_DisplayMinimumPctle(QxrdSettingsSaverPtr(), this, "displayMinimumPercentile", 0, "Minimum Displayed Value (as a percentile)"),
  m_DisplayMaximumPctle(QxrdSettingsSaverPtr(), this, "displayMaximumPercentile", 100, "Maximum Displayed Value (as a percentile)"),
  m_DisplayScalingMode(QxrdSettingsSaverPtr(), this, "displayScalingMode", 0, "Display Scaling Mode (0 = percentage, 1 = percentile, 2 = absolute)"),
  m_DisplayColorMap(QxrdSettingsSaverPtr(), this, "displayColorMap", 0, "Display Color Map Index"),
  m_DisplayLog(QxrdSettingsSaverPtr(), this, "displayLog", false, "Logarithmic Intensity Display?"),
  m_ImageShown(QxrdSettingsSaverPtr(), this, "imageShown", 1, "Image Shown?"),
  m_MaskShown(QxrdSettingsSaverPtr(), this, "maskShown", 0, "Mask Shown?"),
  m_OverflowShown(QxrdSettingsSaverPtr(), this, "overflowShown", 0, "Overflow Shown?"),
  m_InterpolatePixels(QxrdSettingsSaverPtr(), this, "interpolatePixels", 1, "Interpolated Pixel Display?"),
  m_MaintainAspectRatio(QxrdSettingsSaverPtr(), this, "maintainAspectRatio", 1, "Maintain Equal Scaling in X and Y"),
  m_TTHMouse(QxrdSettingsSaverPtr(), this,"tthMouse",0, "2 Theta at Mouse"),
  m_QMouse(QxrdSettingsSaverPtr(), this,"qMouse",0, "Q at Mouse"),
  m_ValMouse(QxrdSettingsSaverPtr(), this,"valMouse",0, "Image Value at Mouse"),
  m_MaskMouse(QxrdSettingsSaverPtr(), this,"maskMouse",0, "Mask Value at Mouse")
{
}

void QxrdImagePlotSettings::readSettings(QSettings *settings, QString section)
{
  QxrdPlotSettings::readSettings(settings, section);
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdImagePlotSettings::writeSettings(QSettings *settings, QString section)
{
  QxrdPlotSettings::writeSettings(settings, section);
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

