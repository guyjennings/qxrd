#include "qcepzoomallcommand.h"
#include <QToolButton>
#include "qcepplotwidget.h"

QcepZoomAllCommand::QcepZoomAllCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotButtonCommand("zoomAllCommand", "Auto Scale", plot, set, ":/images/zoom-all.png", false)
{
  connect(m_ToolButton, &QToolButton::clicked, m_PlotWidget, &QcepPlotWidget::zoomAll);
}
