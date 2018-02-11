#include "qxrdprintplotbutton.h"
#include "qxrdplotwidget.h"
#include <QToolButton>

QxrdPrintPlotButton::QxrdPrintPlotButton(QxrdPlotWidget            *plot,
                                         QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand("printPlotButton", "Print Plot...", plot, set, ":/images/print.png", false)
{
  connect(m_ToolButton, &QToolButton::clicked, m_PlotWidget, &QxrdPlotWidget::printGraph);
}
