#include "qcepzoomoutcommand.h"
#include <QToolButton>
#include "qcepplotwidget.h"

QcepZoomOutCommand::QcepZoomOutCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotButtonCommand("zoomOutCommand", "Zoom Out", plot, set, ":/images/zoom-out.png", false)
{
  connect(m_ToolButton, &QToolButton::clicked,
          m_PlotWidget, &QcepPlotWidget::zoomOut);
}
