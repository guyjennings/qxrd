#include "qcepimageplotwidgetsettings.h"

QcepImagePlotWidgetSettings::QcepImagePlotWidgetSettings(QString name)
 : inherited(name),
   m_DisplayMinimumPctle(this, "displayMinimumPctle", 0, "Minimum Displayed Value (as a percentile)"),
   m_DisplayMaximumPctle(this, "displayMaximumPctle", 100, "Maximum Displayed Value (as a percentile)"),
   m_DisplayMinimumPct(this, "displayMinimumPct", 0, "Minimum Displayed Value (as % of data range)"),
   m_DisplayMaximumPct(this, "displayMaximumPct", 100, "Maximum Displayed Value (as % of data range)"),
   m_DisplayMinimumVal(this, "displayMinimumVal", 0,  "Minimum Displayed Value (in absolute units)"),
   m_DisplayMaximumVal(this, "displayMaximumVal", 10000, "Maximum Displayed Value (in absolute units)"),
   m_DisplayScalingMode(this, "displayScalingMode", 0, "Display Scaling Mode (0 = percentage, 1 = percentile, 2 = absolute)"),
   m_DisplayColorMap(this, "displayColorMap", 0, "Display Color Map Index"),
   m_DisplayLog(this, "displayLog", false, "Logarithmic Intensity Display?"),
   m_ImageShown(this, "imageShown", 1, "Image Shown?"),
   m_MaskShown(this, "maskShown", 0, "Mask Shown?"),
   m_OverflowShown(this, "overflowShown", 0, "Overflow Shown?"),
   m_OverflowLevel(this, "overflowLevel", 50000, "Overflow Level"),
   m_InterpolatePixels(this, "interpolatePixels", 1, "Interpolated Pixel Display?"),
   m_MaintainAspectRatio(this, "maintainAspectRatio", 1, "Maintain Equal Scaling in X and Y"),
   m_DisplayROI(this, "displayROI", 1, "Display ROI Outlines"),
   m_TTHMouse(this,"tthMouse",0, "2 Theta at Mouse"),
   m_QMouse(this,"qMouse",0, "Q at Mouse"),
   m_RMouse(this,"rMouse",0, "R at Mouse"),
   m_ValMouse(this,"valMouse",0, "Image Value at Mouse"),
   m_MaskMouse(this,"maskMouse",0, "Mask Value at Mouse")
{

}

void QcepImagePlotWidgetSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}
