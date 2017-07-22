#include "qxrdtestimagewindowdatavis.h"
#include "ui_qxrdtestimagedataviswindow.h"

QxrdTestImageWindowDataVis::QxrdTestImageWindowDataVis(QWidget *parent) :
  QxrdTestImageWindow(parent)
{
  setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);
}

QxrdTestImageWindowDataVis::~QxrdTestImageWindowDataVis()
{
}

void QxrdTestImageWindowDataVis::onNewImageAvailable(QcepDoubleImageDataPtr data)
{
  m_Messages->append("New Image Received");
}
