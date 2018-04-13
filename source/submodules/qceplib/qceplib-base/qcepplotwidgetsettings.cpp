#include "qcepplotwidgetsettings.h"
#include <qwt_plot.h>

QcepPlotWidgetSettings::QcepPlotWidgetSettings(QString name)
  : QcepObject(name),
    m_LegendPosition(this, "legendPosition", QwtPlot::RightLegend, "Plot Legend Position"),
    m_XAxisVis      (this,"xAxisVis",1, "X Axis Visible?"),
    m_YAxisVis      (this,"yAxisVis",1, "Y Axis Visible?"),
    m_X2AxisVis     (this,"x2AxisLog",0, "2nd X Axis Visible?"),
    m_Y2AxisVis     (this,"y2AxisLog",0, "2nd Y Axis Visible?"),
    m_XAxisLog      (this,"xAxisLog",0, "Logarithmic X Axis?"),
    m_YAxisLog      (this,"yAxisLog",0, "Logarithmic Y Axis?"),
    m_X2AxisLog     (this,"x2AxisLog",0, "Logarithmic 2nd X Axis?"),
    m_Y2AxisLog     (this,"y2AxisLog",0, "Logarithmic 2nd Y Axis?"),
    m_XMouse        (this,"xMouse",0, "X Position of Mouse"),
    m_YMouse        (this,"yMouse",0, "Y Position of Mouse")
{
}
