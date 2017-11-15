#include "qxrdhistogramcommand.h"
#include <QToolButton>

QxrdHistogramCommand::QxrdHistogramCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotButtonCommand(name, plot)
{

}

QToolButton* QxrdHistogramCommand::toolButton()
{
  QToolButton* res = new QToolButton();

  res->setObjectName(get_Name());
  QIcon icon;

  icon.addFile(QStringLiteral(":/images/histogram.png"), QSize(), QIcon::Normal, QIcon::Off);
  res->setIcon(icon);
  res->setIconSize(QSize(24,24));
  res->setCheckable(true);
  res->setAutoExclusive(true);
  res->setToolTip(tr("Mask Circles"));

  return res;
}
