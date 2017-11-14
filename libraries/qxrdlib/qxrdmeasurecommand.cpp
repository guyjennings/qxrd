#include "qxrdmeasurecommand.h"
#include <QToolButton>

QxrdMeasureCommand::QxrdMeasureCommand(QString name)
  : QxrdPlotCommand(name)
{

}

QToolButton* QxrdMeasureCommand::toolButton()
{
  QToolButton* res = new QToolButton();

  res->setObjectName(get_Name());
  QIcon icon;

  icon.addFile(QStringLiteral(":/images/measure.png"), QSize(), QIcon::Normal, QIcon::Off);
  res->setIcon(icon);
  res->setIconSize(QSize(24,24));
  res->setCheckable(true);
  res->setAutoExclusive(true);
  res->setToolTip(tr("Measure"));

  return res;
}
