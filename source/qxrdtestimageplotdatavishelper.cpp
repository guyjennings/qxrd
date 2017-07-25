#include "qxrdtestimageplotdatavishelper.h"

QxrdTestImagePlotDataVisHelper::QxrdTestImagePlotDataVisHelper()
  : QcepSerializableObject("imagePlotHelper")
{

}

void QxrdTestImagePlotDataVisHelper::onNewImageAvailable(QcepDoubleImageDataPtr data)
{
  int ind = -1;

  if (data) {
    ind = data->get_ImageNumber();
  }

//  m_Messages->append(tr("New Image Received : Index %1 : Addr %2").arg(ind).HEXARG(data.data()));
}
