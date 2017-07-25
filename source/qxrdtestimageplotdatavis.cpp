#include "qxrdtestimageplotdatavis.h"
#include <QGridLayout>
#include <QtDataVisualization>
#include "qxrdtestimageplotdatavishelper.h"

using namespace QtDataVisualization;

QxrdTestImagePlotDataVis::QxrdTestImagePlotDataVis(QWidget *parent)
  : QWidget(parent)
{
  QGridLayout *layout = new QGridLayout(this);

  Q3DSurface  *surface = new Q3DSurface();
  QWidget     *container = QWidget::createWindowContainer(surface);

  layout->addWidget(container, 0, 0, 1, 1);

  m_HelperThread = QSharedPointer<QxrdTestThread<QxrdTestImagePlotDataVisHelper> > (
        new QxrdTestThread<QxrdTestImagePlotDataVisHelper>(QcepObjectWPtr()));

  m_Helper = m_HelperThread->object();

  if (m_Helper) {
    connect (m_Helper.data(), &QxrdTestImagePlotDataVisHelper::newSurfaceSeries,
             this,            &QxrdTestImagePlotDataVis::onNewSurfaceSeries);
  }
}

QxrdTestImagePlotDataVisHelperPtr QxrdTestImagePlotDataVis::helper()
{
  return m_Helper;
}

void QxrdTestImagePlotDataVis::onNewSurfaceSeries(QSurface3DSeriesPtr surface)
{
}
