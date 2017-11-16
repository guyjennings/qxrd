#include "qxrdmaskpolygonscommand.h"
#include <QToolButton>

QxrdMaskPolygonsCommand::QxrdMaskPolygonsCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotButtonCommand(name, plot)
{

}

QToolButton* QxrdMaskPolygonsCommand::toolButton()
{
  QToolButton* res = new QToolButton();

  res->setObjectName(get_Name());
  QIcon icon;

  icon.addFile(QStringLiteral(":/images/mask_polygon.png"), QSize(), QIcon::Normal, QIcon::Off);
  res->setIcon(icon);
  res->setIconSize(QSize(24,24));
  res->setCheckable(true);
  res->setAutoExclusive(true);
  res->setToolTip(tr("Mask Circles"));

  return res;
}
