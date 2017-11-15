#include "qxrdzoomoutcommand.h"
#include <QToolButton>
#include "qxrdimageplot.h"

QxrdZoomOutCommand::QxrdZoomOutCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotButtonCommand(name, plot)
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

  connect(res, &QToolButton::clicked, m_Plot, &QxrdImagePlot::zoomOut);

  return res;
}
