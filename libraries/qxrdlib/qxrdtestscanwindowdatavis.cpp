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

QxrdTestScanPlotDataVisHelperPtr QxrdTestScanWindowDataVis::helper()
{
  return m_Graph->helper();
}
