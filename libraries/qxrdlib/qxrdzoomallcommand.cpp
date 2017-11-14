#include "qxrdzoomallcommand.h"
#include <QToolButton>

QxrdZoomAllCommand::QxrdZoomAllCommand(QString name)
  : QxrdPlotCommand(name)
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

  return res;
}
