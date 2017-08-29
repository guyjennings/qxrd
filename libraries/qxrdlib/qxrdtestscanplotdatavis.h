#ifndef QXRDTESTSCANPLOTDATAVIS_H
#define QXRDTESTSCANPLOTDATAVIS_H

#include <QtCharts>
#include "qxrdtestscanplotdatavishelper-ptr.h"
#include "qxrdtestthread.h"

class QxrdTestScanPlotDataVis : public QtCharts::QChartView
{
  Q_OBJECT

public:
  explicit QxrdTestScanPlotDataVis(QWidget *parent = nullptr);

  QxrdTestScanPlotDataVisHelperPtr helper();

signals:

public slots:
  void onNewChartCurveAvailable();

private:
  QSharedPointer<QxrdTestThread<QxrdTestScanPlotDataVisHelper> > m_HelperThread;
  QxrdTestScanPlotDataVisHelperPtr                               m_Helper;
};

#endif // QXRDTESTSCANPLOTDATAVIS_H
