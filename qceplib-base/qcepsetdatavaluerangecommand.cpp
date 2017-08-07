#include "qcepsetdatavaluerangecommand.h"
#include "qcepallocator.h"
#include "qcepimagedata.h"

QcepSetDataValueRangeCommand::QcepSetDataValueRangeCommand() :
  QcepSerializableObject("setDataValueRangeCommand")
{

}

void QcepSetDataValueRangeCommand::onImageAvailable(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res = exec(img);

  emit processedImageAvailable(res);
}

QcepDoubleImageDataPtr QcepSetDataValueRangeCommand::exec(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res;

  if (img) {
    res = QcepAllocator::newDoubleImage("dup",
                                        img->get_Width(), img->get_Height(), QcepAllocator::AlwaysAllocate);

    res->copyFrom(img);
    res->set_Name(tr("%1.setr").arg(img->get_Name()));
  }

  return res;
}
