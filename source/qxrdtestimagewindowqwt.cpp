#include "qxrdtestimagewindowqwt.h"
#include "ui_qxrdtestimageqwtwindow.h"

QxrdTestImageWindowQwt::QxrdTestImageWindowQwt(QWidget *parent) :
  QxrdTestImageWindow(parent)
{
  setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
}

QxrdTestImageWindowQwt::~QxrdTestImageWindowQwt()
{
}

void QxrdTestImageWindowQwt::onNewImageAvailable(QcepDoubleImageDataPtr data)
{
  m_Messages->append("New Image Received");
}
