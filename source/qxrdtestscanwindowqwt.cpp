#include "qxrdtestscanwindowqwt.h"
#include "ui_qxrdtestscanqwtwindow.h"

QxrdTestScanWindowQwt::QxrdTestScanWindowQwt(QWidget *parent) :
  QxrdTestScanWindow(parent)
{
  setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
}

QxrdTestScanWindowQwt::~QxrdTestScanWindowQwt()
{
}

void QxrdTestScanWindowQwt::onNewScanAvailable(QcepDataColumnScanPtr data)
{
  m_Messages->append("New Scan Received");
}
