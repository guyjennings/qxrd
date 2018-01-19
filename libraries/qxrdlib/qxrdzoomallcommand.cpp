#include "qxrdzoomallcommand.h"
#include <QToolButton>
#include "qxrdplotwidget.h"

QxrdZoomAllCommand::QxrdZoomAllCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set, ":/images/zoom-all.png", "Auto Scale", false)
{
  connect(m_ToolButton, &QToolButton::clicked, m_PlotWidget, &QxrdPlotWidget::zoomAll);
}
