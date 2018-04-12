#include "qceppowderpointscommand.h"
#include <QToolButton>
#include "qceppowderpointpicker.h"
#include "qcepimageplot.h"

QcepPowderPointsCommand::QcepPowderPointsCommand(QcepPlotWidget *plot,
                                                 QcepPlotWidgetSettingsWPtr set)
  : QcepPlotButtonCommand("powderPointsCommand", "Powder Points", plot, set, ":/images/powder_point.png", true)
{  
//  appendMode(":/images/powder_point.png",      "Powder Points");
  appendMode(":/images/powder_point_ring.png", "Powder Point on Ring");
  appendMode(":/images/powder_trace_ring.png", "Trace Points on Ring");

  m_Picker = new QcepPowderPointPicker(m_Plot);
  m_Picker -> setEnabled(false);

  setPlotPicker(m_Picker);

  changeMode(0);

  connect(m_Picker, (void (QwtPlotPicker::*)(const QPointF&)) &QwtPlotPicker::selected,
          this, &QcepPowderPointsCommand::selected);
}

//TODO: implement
void QcepPowderPointsCommand::selected(const QPointF &pt)
{
  printf("QcepPowderPointsCommand::selected(x:%g,y:%g)\n",
         pt.x(), pt.y());
}
