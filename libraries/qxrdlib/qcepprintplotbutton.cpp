#include "qcepprintplotbutton.h"
#include "qcepplotwidget.h"
#include <QToolButton>

QcepPrintPlotButton::QcepPrintPlotButton(QcepPlotWidget            *plot,
                                         QcepPlotWidgetSettingsWPtr set)
  : QcepPlotButtonCommand("printPlotButton", "Print Plot...", plot, set, ":/images/print.png", false)
{
  connect(m_ToolButton, &QToolButton::clicked, m_PlotWidget, &QcepPlotWidget::printGraph);
}
