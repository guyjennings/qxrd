#include "qxrdzoomincommand.h"
#include <QToolButton>
#include "qxrdplotwidget.h"

QxrdZoomInCommand::QxrdZoomInCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand("zoomInCommand", "Zoom In", plot, set, ":/images/zoom-in.png", true)
{
  m_ToolButton->setChecked(true);

  connect(m_ToolButton, &QToolButton::clicked,
          m_PlotWidget, &QxrdPlotWidget::zoomIn);
}

void QxrdZoomInCommand::enable()
{
  QxrdPlotButtonCommand::enable();

  m_PlotWidget->enableZooming();
}

void QxrdZoomInCommand::disable()
{
  QxrdPlotButtonCommand::disable();

  m_PlotWidget->disableZooming();
}
