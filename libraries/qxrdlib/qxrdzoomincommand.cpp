#include "qxrdzoomincommand.h"
#include <QToolButton>
#include "qxrdplotwidget.h"

QxrdZoomInCommand::QxrdZoomInCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set, ":/images/zoom-in.png", "Zoom In", true)
{
  m_ToolButton->setChecked(true);

  connect(m_ToolButton, &QToolButton::clicked, m_PlotWidget, &QxrdPlotWidget::zoomIn);
}
