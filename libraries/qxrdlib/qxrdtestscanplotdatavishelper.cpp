#ifdef HAVE_DATAVIS

#include "qxrdtestscanplotdatavishelper.h"

QxrdTestScanPlotDataVisHelper::QxrdTestScanPlotDataVisHelper()
  : QcepSerializableObject("scanPlotHelper")
{

}

void QxrdTestScanPlotDataVisHelper::onNewScanAvailable(QcepDataColumnScanPtr data)
{
//  m_Messages->append(tr("New Scan Received : %1").HEXARG(data.data()));
}

#endif
