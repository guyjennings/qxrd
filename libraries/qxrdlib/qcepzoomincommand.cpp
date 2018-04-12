#include "qcepzoomincommand.h"
#include <QToolButton>
#include "qcepplotwidget.h"

QcepZoomInCommand::QcepZoomInCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotButtonCommand("zoomInCommand", "Zoom In", plot, set, ":/images/zoom-in.png", true)
{
  m_ToolButton->setChecked(true);

  connect(m_ToolButton, &QToolButton::clicked,
          m_PlotWidget, &QcepPlotWidget::zoomIn);
}

void QcepZoomInCommand::enable()
{
  QcepPlotButtonCommand::enable();

  m_PlotWidget->enableZooming();
}

void QcepZoomInCommand::disable()
{
  QcepPlotButtonCommand::disable();

  m_PlotWidget->disableZooming();
}
