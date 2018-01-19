#include "qxrdzoomoutcommand.h"
#include <QToolButton>
#include "qxrdplotwidget.h"

QxrdZoomOutCommand::QxrdZoomOutCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set, ":/images/zoom-out.png", "Zoom Out", false)
{
  connect(m_ToolButton, &QToolButton::clicked, m_PlotWidget, &QxrdPlotWidget::zoomOut);
}
