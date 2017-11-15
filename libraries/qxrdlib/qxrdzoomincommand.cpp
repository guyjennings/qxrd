#include "qxrdzoomincommand.h"
#include <QToolButton>
#include "qxrdimageplot.h"

QxrdZoomInCommand::QxrdZoomInCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotButtonCommand(name, plot)
{

}

QToolButton* QxrdZoomInCommand::toolButton()
{
  QToolButton* res = new QToolButton();

  res->setObjectName(get_Name());
  QIcon icon;

  icon.addFile(QStringLiteral(":/images/zoom-in.png"), QSize(), QIcon::Normal, QIcon::Off);
  res->setIcon(icon);
  res->setIconSize(QSize(24,24));
  res->setCheckable(true);
  res->setChecked(true);
  res->setAutoExclusive(true);
  res->setToolTip(tr("Zoom In"));

  connect(res, &QToolButton::clicked, m_Plot, &QxrdImagePlot::zoomIn);

  return res;
}
