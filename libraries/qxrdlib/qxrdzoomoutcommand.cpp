#include "qxrdzoomoutcommand.h"
#include <QToolButton>

QxrdZoomOutCommand::QxrdZoomOutCommand(QString name)
  : QxrdPlotCommand(name)
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

  return res;
}
