#include "qxrdprintplotbutton.h"
#include "qxrdplotwidget.h"
#include <QToolButton>

QxrdPrintPlotButton::QxrdPrintPlotButton(QString                    name,
                                         QxrdPlotWidget            *plot,
                                         QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set, ":/images/print.png", "Print Plot...", false)
{
  connect(m_ToolButton, &QToolButton::clicked, m_PlotWidget, &QxrdPlotWidget::printGraph);
}
