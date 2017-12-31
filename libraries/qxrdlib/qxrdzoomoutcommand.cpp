#include "qxrdzoomoutcommand.h"
#include <QToolButton>
#include "qxrdplotwidget.h"

QxrdZoomOutCommand::QxrdZoomOutCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set)
{

}

QToolButton* QxrdZoomOutCommand::toolButton()
{
  QToolButton* res = new QToolButton();

  res->setObjectName(get_Name());
  QIcon icon;

  icon.addFile(QStringLiteral(":/images/zoom-out.png"), QSize(), QIcon::Normal, QIcon::Off);
  res->setIcon(icon);
  res->setIconSize(QSize(24,24));
  res->setToolTip(tr("Zoom Out"));

  connect(res, &QToolButton::clicked, m_PlotWidget, &QxrdPlotWidget::zoomOut);

  return res;
}
