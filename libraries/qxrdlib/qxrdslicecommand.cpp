#include "qxrdslicecommand.h"
#include <QToolButton>

QxrdSliceCommand::QxrdSliceCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotButtonCommand(name, plot)
{

}

QToolButton* QxrdSliceCommand::toolButton()
{
  QToolButton* res = new QToolButton();

  res->setObjectName(get_Name());
  QIcon icon;

  icon.addFile(QStringLiteral(":/images/slice.png"), QSize(), QIcon::Normal, QIcon::Off);
  res->setIcon(icon);
  res->setIconSize(QSize(24,24));
  res->setCheckable(true);
  res->setAutoExclusive(true);
  res->setToolTip(tr("Slice"));

  return res;
}
