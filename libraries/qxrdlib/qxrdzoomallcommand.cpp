#include "qxrdzoomallcommand.h"
#include <QToolButton>
#include "qxrdplotwidget.h"

QxrdZoomAllCommand::QxrdZoomAllCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand("zoomAllCommand", "Auto Scale", plot, set, ":/images/zoom-all.png", false)
{
  connect(m_ToolButton, &QToolButton::clicked, m_PlotWidget, &QxrdPlotWidget::zoomAll);
}
