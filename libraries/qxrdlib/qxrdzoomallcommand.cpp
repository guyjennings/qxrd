#include "qxrdzoomallcommand.h"
#include <QToolButton>
#include "qxrdimageplot.h"

QxrdZoomAllCommand::QxrdZoomAllCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotButtonCommand(name, plot)
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

  connect(res, &QToolButton::clicked, m_Plot, &QxrdImagePlot::autoScale);

  return res;
}
