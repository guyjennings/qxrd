#include "qxrdtestscanwindowdatavis.h"
#include "qcepdebug.h"
#include "qcepdatacolumnscan.h"

QxrdTestScanWindowDataVis::QxrdTestScanWindowDataVis(QWidget *parent) :
  QxrdTestScanWindow(parent)
{
  setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
}

QxrdTestScanWindowDataVis::~QxrdTestScanWindowDataVis()
{
}

void QxrdTestScanWindowDataVis::onNewScanAvailable(QcepDataColumnScanPtr data)
{
  m_Messages->append(tr("New Scan Received : %1").HEXARG(data.data()));
}
