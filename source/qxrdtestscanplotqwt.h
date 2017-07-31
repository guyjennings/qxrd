#ifndef QXRDTESTSCANPLOTQWT_H
#define QXRDTESTSCANPLOTQWT_H

#include "qcepplot.h"
#include "qxrdtestscanplotqwthelper-ptr.h"
#include "qxrdtestscanplotqwthelper.h"
#include "qxrdtestthread.h"

class QxrdTestScanPlotQwt : public QcepPlot
{
  Q_OBJECT

public:
  explicit QxrdTestScanPlotQwt(QWidget *parent = nullptr);

  QxrdTestScanPlotQwtHelperPtr helper();

signals:

public slots:
  void onNewPlotCurve(QwtPlotCurvePtr pc);

private:
  QSharedPointer<QxrdTestThread<QxrdTestScanPlotQwtHelper> > m_HelperThread;
  QxrdTestScanPlotQwtHelperPtr                               m_Helper;
  QwtPlotCurvePtr                                            m_Curve;
};

#endif // QXRDTESTSCANPLOTQWT_H
