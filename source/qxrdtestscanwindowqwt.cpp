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

QxrdTestScanPlotQwtHelperPtr QxrdTestScanWindowQwt::helper()
{
  return m_Graph->helper();
}
