#include "qcepplotsettings.h"

#include <qwt_plot.h>

QcepPlotSettings::QcepPlotSettings(QString name, QcepSettingsSaverWPtr saver, QcepObject *parent) :
  QcepObject(name, parent),
  m_XMouse(QcepSettingsSaverWPtr(), this,"xMouse",0, "X Position of Mouse"),
  m_YMouse(QcepSettingsSaverWPtr(), this,"yMouse",0, "Y Position of Mouse"),
  m_LegendPosition(saver, this, "legendPosition", QwtPlot::RightLegend, "Plot Legend Position"),
  m_XAxisLog(saver, this,"xAxisLog",0, "Logarithmic X Axis?"),
  m_YAxisLog(saver, this,"yAxisLog",0, "Logarithmic Y Axis?"),
  m_X2AxisLog(saver, this,"x2AxisLog",0, "Logarithmic 2nd X Axis?"),
  m_Y2AxisLog(saver, this,"y2AxisLog",0, "Logarithmic 2nd Y Axis?")
{
}

