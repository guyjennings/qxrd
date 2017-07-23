#include "qxrdtestimagewindowqwt.h"
#include "ui_qxrdtestimageqwtwindow.h"
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

void QxrdTestImageWindowQwt::onNewImageAvailable(QcepDoubleImageDataPtr data)
{
  int ind = -1;

  if (data) {
    ind = data->get_ImageNumber();
  }

  m_Messages->append(tr("New Image Received : Index %1 : Addr %2").arg(ind).HEXARG(data.data()));
}
