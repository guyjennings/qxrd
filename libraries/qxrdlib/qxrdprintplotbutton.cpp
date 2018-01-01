#include "qxrdprintplotbutton.h"
#include "qxrdplotwidget.h"
#include <QToolButton>

QxrdPrintPlotButton::QxrdPrintPlotButton(QString                    name,
                                         QxrdPlotWidget            *plot,
                                         QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set)
{

}

QToolButton* QxrdPrintPlotButton::toolButton()
{
  QToolButton* res = new QToolButton();

  res->setObjectName(get_Name());
  QIcon icon;

  icon.addFile(QStringLiteral(":/images/print.png"), QSize(), QIcon::Normal, QIcon::Off);
  res->setIcon(icon);
  res->setIconSize(QSize(24,24));
//  res->setCheckable(true);
//  res->setAutoExclusive(true);
  res->setToolTip(tr("Print Plot..."));

  connect(res, &QToolButton::clicked, m_PlotWidget, &QxrdPlotWidget::printGraph);

  return res;
}
