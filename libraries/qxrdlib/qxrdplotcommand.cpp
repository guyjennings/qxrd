#include "qxrdplotcommand.h"
#include "qxrdimageplotwidget.h"
#include "qxrdplotwidgetsettings.h"

QxrdPlotCommand::QxrdPlotCommand(QString name,
                                 QString desc,
                                 QxrdPlotWidget *plot,
                                 QxrdPlotWidgetSettingsWPtr set)
  : QcepObject(name),
    m_Description(desc),
    m_PlotWidget(plot),
    m_Plot((plot?plot->m_Plot:NULL)),
    m_Settings(set)
{
}

void QxrdPlotCommand::enable()
{
  m_PlotWidget->disableCommands(this);
}

void QxrdPlotCommand::disable()
{
}

void QxrdPlotCommand::setPen(const QPen &pen)
{
}

QxrdPlotWidgetSettingsWPtr QxrdPlotCommand::settings() const
{
  return m_Settings;
}
