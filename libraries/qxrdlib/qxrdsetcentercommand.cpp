#include "qxrdsetcentercommand.h"
#include <QToolButton>
#include "qxrdcenterfinderpicker.h"
#include "qxrdimageplot.h"

QxrdSetCenterCommand::QxrdSetCenterCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set, ":/images/ringfinder.png", "Set Center", true)
{  
  m_Picker = new QxrdCenterFinderPicker(m_Plot);
  m_Picker -> setEnabled(false);

  setPlotPicker(m_Picker);

  connect(m_Picker, (void (QwtPlotPicker::*)(const QPointF &)) &QwtPlotPicker::selected,
          this, &QxrdSetCenterCommand::selected);
}

//TODO: implement
void QxrdSetCenterCommand::selected(const QPointF &pt)
{
  printMessage(tr("QxrdSetCenterCommand::selected(x:%1,y:%2)")
               .arg(pt.x()).arg(pt.y()));
}
