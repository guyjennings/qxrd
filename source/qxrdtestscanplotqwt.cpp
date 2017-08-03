#include "qxrdtestscanplotqwt.h"
#include "qxrdtestscanplotqwthelper.h"

QxrdTestScanPlotQwt::QxrdTestScanPlotQwt(QWidget *parent) : QcepPlot(parent)
{
  m_HelperThread = QSharedPointer<QxrdTestThread<QxrdTestScanPlotQwtHelper> > (
        new QxrdTestThread<QxrdTestScanPlotQwtHelper>(QcepObjectWPtr()));

  m_HelperThread -> start();

  m_Helper = m_HelperThread->object();

  if (m_Helper) {
    connect(m_Helper.data(), &QxrdTestScanPlotQwtHelper::newPlotCurve,
            this,            &QxrdTestScanPlotQwt::onNewPlotCurve);
  }
}

QxrdTestScanPlotQwtHelperPtr QxrdTestScanPlotQwt::helper()
{
  return m_Helper;
}

void QxrdTestScanPlotQwt::onNewPlotCurve(QwtPlotCurvePtr pc)
{
  if (m_Curve) {
    removeItem(m_Curve.data());
  }

  m_Curve = pc;

  if (m_Curve) {
    m_Curve->attach(this);
  }

  replot();
}
