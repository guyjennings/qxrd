#include "qxrdtestimageplotdatavis.h"
#include <QGridLayout>
#include <QtDataVisualization>

using namespace QtDataVisualization;

QxrdTestImagePlotDataVis::QxrdTestImagePlotDataVis(QWidget *parent)
  : QWidget(parent)
{
  QGridLayout *layout = new QGridLayout(this);

  Q3DSurface  *surface = new Q3DSurface();
  QWidget     *container = QWidget::createWindowContainer(surface);

  layout->addWidget(container, 0, 0, 1, 1);
}
