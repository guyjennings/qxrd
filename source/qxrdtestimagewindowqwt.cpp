#include "qxrdtestimagewindowqwt.h"
#include "qxrddebug.h"
#include "qcepimagedata.h"

QxrdTestImageWindowQwt::QxrdTestImageWindowQwt(QWidget *parent) :
  QxrdTestImageWindow(parent)
{
  setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
}

QxrdTestImageWindowQwt::~QxrdTestImageWindowQwt()
{
}

QxrdTestImagePlotQwtHelperPtr QxrdTestImageWindowQwt::helper()
{
  return m_Graph->helper();
}
