#include "qxrdzoomoutcommand.h"
#include <QToolButton>
#include "qxrdplotwidget.h"

QxrdZoomOutCommand::QxrdZoomOutCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand("zoomOutCommand", "Zoom Out", plot, set, ":/images/zoom-out.png", false)
{
  connect(m_ToolButton, &QToolButton::clicked,
          m_PlotWidget, &QxrdPlotWidget::zoomOut);
}
