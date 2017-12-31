#include "qxrdzoomallcommand.h"
#include <QToolButton>
#include "qxrdplotwidget.h"

QxrdZoomAllCommand::QxrdZoomAllCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set)
{

}

QToolButton* QxrdZoomAllCommand::toolButton()
{
  QToolButton* res = new QToolButton();

  res->setObjectName(get_Name());
  QIcon icon;

  icon.addFile(QStringLiteral(":/images/zoom-all.png"), QSize(), QIcon::Normal, QIcon::Off);
  res->setIcon(icon);
  res->setIconSize(QSize(24,24));
  res->setToolTip(tr("Auto Scale"));

  connect(res, &QToolButton::clicked, m_PlotWidget, &QxrdPlotWidget::zoomAll);

  return res;
}
