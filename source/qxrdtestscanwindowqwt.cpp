#include "qxrdtestscanwindowqwt.h"
#include "qxrddebug.h"

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
  m_Messages->append(tr("New Scan Received : %1").HEXARG(data.data()));
}
