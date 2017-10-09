#ifdef HAVE_DATAVIS

#include "qxrdtestscanplotdatavis.h"
#include "qxrdtestscanplotdatavishelper.h"
#include <QGridLayout>

QxrdTestScanPlotDataVis::QxrdTestScanPlotDataVis(QWidget *parent) : QtCharts::QChartView(parent)
{
  m_HelperThread = QSharedPointer<QxrdTestThread<QxrdTestScanPlotDataVisHelper> > (
        new QxrdTestThread<QxrdTestScanPlotDataVisHelper>(QcepObjectWPtr()));

  m_HelperThread -> start();

  m_Helper = m_HelperThread->object();

  if (m_Helper) {
    connect(m_Helper.data(), &QxrdTestScanPlotDataVisHelper::newChartCurveAvailable,
            this,            &QxrdTestScanPlotDataVis::onNewChartCurveAvailable);
  }
}

QxrdTestScanPlotDataVisHelperPtr QxrdTestScanPlotDataVis::helper()
{
  return m_Helper;
}

void QxrdTestScanPlotDataVis::onNewChartCurveAvailable()
{
}

#endif // HAVE_DATAVIS
