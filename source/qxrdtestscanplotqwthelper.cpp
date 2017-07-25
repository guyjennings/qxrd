#include "qxrdtestscanplotqwthelper.h"

QxrdTestScanPlotQwtHelper::QxrdTestScanPlotQwtHelper()
  : QcepSerializableObject("scanPlotHelper")
{

}

void QxrdTestScanPlotQwtHelper::onNewScanAvailable(QcepDataColumnScanPtr data)
{
//  m_Messages->append(tr("New Scan Received : %1").HEXARG(data.data()));
}
