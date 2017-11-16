#include "qxrdmaskcirclescommand.h"
#include <QToolButton>

QxrdMaskCirclesCommand::QxrdMaskCirclesCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotButtonCommand(name, plot)
{

}

QToolButton* QxrdMaskCirclesCommand::toolButton()
{
  QToolButton* res = new QToolButton();

  res->setObjectName(get_Name());
  QIcon icon;

  icon.addFile(QStringLiteral(":/images/mask_circles.png"), QSize(), QIcon::Normal, QIcon::Off);
  res->setIcon(icon);
  res->setIconSize(QSize(24,24));
  res->setCheckable(true);
  res->setAutoExclusive(true);
  res->setToolTip(tr("Mask Circles"));

  return res;
}
