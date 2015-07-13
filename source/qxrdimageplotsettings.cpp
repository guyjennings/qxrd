#include "qxrdimageplotsettings.h"
#include "qcepsettingssaver.h"

QxrdImagePlotSettings::QxrdImagePlotSettings(QcepSettingsSaverWPtr saver, QObject *parent) :
  QcepPlotSettings("imageGraph", saver, parent),
  m_DisplayMinimumPctle(saver, this, "displayMinimumPctle", 0, "Minimum Displayed Value (as a percentile)"),
  m_DisplayMaximumPctle(saver, this, "displayMaximumPctle", 100, "Maximum Displayed Value (as a percentile)"),
  m_DisplayMinimumPct(saver, this, "displayMinimumPct", 0, "Minimum Displayed Value (as % of data range)"),
  m_DisplayMaximumPct(saver, this, "displayMaximumPct", 100, "Maximum Displayed Value (as % of data range)"),
  m_DisplayMinimumVal(saver, this, "displayMinimumVal", 0,  "Minimum Displayed Value (in absolute units)"),
  m_DisplayMaximumVal(saver, this, "displayMaximumVal", 10000, "Maximum Displayed Value (in absolute units)"),
  m_DisplayScalingMode(saver, this, "displayScalingMode", 0, "Display Scaling Mode (0 = percentage, 1 = percentile, 2 = absolute)"),
  m_DisplayColorMap(saver, this, "displayColorMap", 0, "Display Color Map Index"),
  m_DisplayLog(saver, this, "displayLog", false, "Logarithmic Intensity Display?"),
  m_ImageShown(saver, this, "imageShown", 1, "Image Shown?"),
  m_MaskShown(saver, this, "maskShown", 0, "Mask Shown?"),
  m_OverflowShown(saver, this, "overflowShown", 0, "Overflow Shown?"),
  m_InterpolatePixels(saver, this, "interpolatePixels", 1, "Interpolated Pixel Display?"),
  m_MaintainAspectRatio(saver, this, "maintainAspectRatio", 1, "Maintain Equal Scaling in X and Y"),
  m_TTHMouse(QcepSettingsSaverWPtr(), this,"tthMouse",0, "2 Theta at Mouse"),
  m_QMouse(QcepSettingsSaverWPtr(), this,"qMouse",0, "Q at Mouse"),
  m_RMouse(QcepSettingsSaverWPtr(), this,"rMouse",0, "R at Mouse"),
  m_ValMouse(QcepSettingsSaverWPtr(), this,"valMouse",0, "Image Value at Mouse"),
  m_MaskMouse(QcepSettingsSaverWPtr(), this,"maskMouse",0, "Mask Value at Mouse")
{
}
