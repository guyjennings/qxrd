#ifndef QXRDTESTSCANPLOTDATAVIS_H
#define QXRDTESTSCANPLOTDATAVIS_H

#include <QtCharts>

class QxrdTestScanPlotDataVis : public QtCharts::QChartView
{
  Q_OBJECT

public:
  explicit QxrdTestScanPlotDataVis(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // QXRDTESTSCANPLOTDATAVIS_H
