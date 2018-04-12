#include "qcepplotcommand.h"
#include "qcepimageplotwidget.h"
#include "qcepplotwidgetsettings.h"

QcepPlotCommand::QcepPlotCommand(QString name,
                                 QString desc,
                                 QcepPlotWidget *plot,
                                 QcepPlotWidgetSettingsWPtr set)
  : QcepObject(name),
    m_Description(desc),
    m_PlotWidget(plot),
    m_Plot((plot?plot->m_Plot:NULL)),
    m_Settings(set)
{
}

void QcepPlotCommand::enable()
{
  m_PlotWidget->disableCommands(this);
}

void QcepPlotCommand::disable()
{
}

void QcepPlotCommand::setPen(const QPen &pen)
{
}

QcepPlotWidgetSettingsWPtr QcepPlotCommand::settings() const
{
  return m_Settings;
}
