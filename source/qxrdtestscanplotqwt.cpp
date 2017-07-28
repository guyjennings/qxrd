#include "qxrdtestscanplotqwt.h"

QxrdTestScanPlotQwt::QxrdTestScanPlotQwt(QWidget *parent) : QcepPlot(parent)
{
  m_HelperThread = QSharedPointer<QxrdTestThread<QxrdTestScanPlotQwtHelper> > (
        new QxrdTestThread<QxrdTestScanPlotQwtHelper>(QcepObjectWPtr()));

  m_HelperThread -> start();

  m_Helper = m_HelperThread->object();
}

QxrdTestScanPlotQwtHelperPtr QxrdTestScanPlotQwt::helper()
{
  return m_Helper;
}
