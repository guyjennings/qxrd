#ifdef HAVE_DATAVIS

#include "qxrdtestimageplotdatavis.h"
#include <QGridLayout>
#include <QtDataVisualization>
#include "qxrdtestimageplotdatavishelper.h"

using namespace QtDataVisualization;

QxrdTestImagePlotDataVis::QxrdTestImagePlotDataVis(QWidget *parent)
  : QWidget(parent)
{
  QGridLayout *layout = new QGridLayout(this);

  m_SurfaceGraph = new Q3DSurface();

  QWidget     *container = QWidget::createWindowContainer(m_SurfaceGraph);

  layout->addWidget(container, 0, 0, 1, 1);

  m_HelperThread = QSharedPointer<QxrdTestThread<QxrdTestImagePlotDataVisHelper> > (
        new QxrdTestThread<QxrdTestImagePlotDataVisHelper>(QcepObjectWPtr()));

  m_HelperThread -> start();

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
  if (m_Surface) {
    m_SurfaceGraph->removeSeries(m_Surface.data());
  }

  m_Surface = surface;

  m_SurfaceGraph->axisX()->setRange(0, 2048);
  m_SurfaceGraph->axisY()->setRange(0, 255);
  m_SurfaceGraph->axisZ()->setRange(0, 2048);

  m_SurfaceGraph->addSeries(m_Surface.data());
}

#endif // HAVE_DATAVIS
