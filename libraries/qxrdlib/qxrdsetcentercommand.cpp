#include "qxrdsetcentercommand.h"
#include <QToolButton>

QxrdSetCenterCommand::QxrdSetCenterCommand(QString name)
  : QxrdPlotCommand(name)
{

}

QToolButton* QxrdSetCenterCommand::toolButton()
{
  QToolButton* res = new QToolButton();

  res->setObjectName(get_Name());
  QIcon icon;

  icon.addFile(QStringLiteral(":/images/ringfinder.png"), QSize(), QIcon::Normal, QIcon::Off);
  res->setIcon(icon);
  res->setIconSize(QSize(24,24));
  res->setCheckable(true);
  res->setAutoExclusive(true);
  res->setToolTip(tr("Set Center"));

  return res;
}
