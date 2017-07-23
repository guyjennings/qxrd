#include "qxrdtestimagewindowdatavis.h"
#include "qcepdebug.h"
#include "qcepimagedata.h"

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
  int ind = -1;

  if (data) {
    ind = data->get_ImageNumber();
  }

  m_Messages->append(tr("New Image Received : Index %1 : Addr %2").arg(ind).HEXARG(data.data()));
}
