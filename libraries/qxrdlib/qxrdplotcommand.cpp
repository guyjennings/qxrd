#include "qxrdplotcommand.h"
#include "qxrdimageplotwidget.h"

QxrdPlotCommand::QxrdPlotCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QcepObject(name),
    m_PlotWidget(plot),
    m_Plot((plot?plot->m_Plot:NULL)),
    m_Settings(set)
{
}

void QxrdPlotCommand::enable()
{
  m_PlotWidget->disableCommands();
}

void QxrdPlotCommand::disable()
{
}
