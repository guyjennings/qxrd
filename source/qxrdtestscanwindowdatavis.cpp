#include "qxrdtestscanwindowdatavis.h"
#include "ui_qxrdtestscandataviswindow.h"

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
  m_Messages->append("New Scan Received");
}
