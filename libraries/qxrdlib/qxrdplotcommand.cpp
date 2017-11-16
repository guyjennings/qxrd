#include "qxrdplotcommand.h"
#include "qxrdimageplotwidget.h"

QxrdPlotCommand::QxrdPlotCommand(QString name, QxrdPlotWidget *plot)
  : QcepSerializableObject(name),
    m_PlotWidget(plot),
    m_Plot((plot?plot->m_Plot:NULL))
{
}

void QxrdPlotCommand::enable()
{
  m_PlotWidget->disableCommands();
}

void QxrdPlotCommand::disable()
{
}
