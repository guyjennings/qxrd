#include "qxrdplotwidgetsettings.h"
#include <qwt_plot.h>

QxrdPlotWidgetSettings::QxrdPlotWidgetSettings(QString name)
  : QcepObject(name),
    m_LegendPosition(this, "legendPosition", QwtPlot::RightLegend, "Plot Legend Position"),
    m_XAxisLog      (this,"xAxisLog",0, "Logarithmic X Axis?"),
    m_YAxisLog      (this,"yAxisLog",0, "Logarithmic Y Axis?"),
    m_X2AxisLog     (this,"x2AxisLog",0, "Logarithmic 2nd X Axis?"),
    m_Y2AxisLog     (this,"y2AxisLog",0, "Logarithmic 2nd Y Axis?")
{
}
