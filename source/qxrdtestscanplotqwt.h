#ifndef QXRDTESTSCANPLOTQWT_H
#define QXRDTESTSCANPLOTQWT_H

#include "qcepplot.h"
#include "qxrdtestscanplotqwthelper-ptr.h"
#include "qxrdtestthread.h"

class QxrdTestScanPlotQwt : public QcepPlot
{
  Q_OBJECT

public:
  explicit QxrdTestScanPlotQwt(QWidget *parent = nullptr);

  QxrdTestScanPlotQwtHelperPtr helper();

signals:

public slots:

private:
  QSharedPointer<QxrdTestThread<QxrdTestScanPlotQwtHelper> > m_HelperThread;
  QxrdTestScanPlotQwtHelperPtr                               m_Helper;
};

#endif // QXRDTESTSCANPLOTQWT_H
