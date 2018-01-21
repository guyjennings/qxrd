#include "qxrdpowderpointscommand.h"
#include <QToolButton>
#include "qxrdpowderpointpicker.h"
#include "qxrdimageplot.h"

QxrdPowderPointsCommand::QxrdPowderPointsCommand(QString name,
                                                 QxrdPlotWidget *plot,
                                                 QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set, QString(), QString(), true)
{  
  appendMode(":/images/powder_point.png",      "Powder Points");
  appendMode(":/images/powder_point_ring.png", "Powder Point on Ring");
  appendMode(":/images/powder_trace_ring.png", "Trace Points on Ring");

  m_Picker = new QxrdPowderPointPicker(m_Plot);
  m_Picker -> setEnabled(false);

  setPlotPicker(m_Picker);

  changeMode(0);

  connect(m_Picker, (void (QwtPlotPicker::*)(const QPointF&)) &QwtPlotPicker::selected,
          this, &QxrdPowderPointsCommand::selected);
}

//TODO: implement
void QxrdPowderPointsCommand::selected(const QPointF &pt)
{
  printf("QxrdPowderPointsCommand::selected(x:%g,y:%g)\n",
         pt.x(), pt.y());
}
