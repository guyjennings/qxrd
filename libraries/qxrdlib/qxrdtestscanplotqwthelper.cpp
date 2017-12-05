#include "qxrdtestscanplotqwthelper.h"
#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"

QxrdTestScanPlotQwtHelper::QxrdTestScanPlotQwtHelper()
  : QcepObject("scanPlotHelper")
{

}

void QxrdTestScanPlotQwtHelper::onNewScanAvailable(QcepDataColumnScanPtr data)
{
//  m_Messages->append(tr("New Scan Received : %1").HEXARG(data.data()));

  if (data) {
    QwtPlotCurvePtr curve =
        QwtPlotCurvePtr(new QwtPlotCurve(data->get_Name()));

    int nPts = data->rowCount();
    double *xData = data->column(0)->data();
    double *yData = data->column(1)->data();

    curve->setSamples(xData, yData, nPts);

    emit newPlotCurve(curve);
  }
}
