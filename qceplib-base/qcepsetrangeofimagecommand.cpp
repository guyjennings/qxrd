#include "qcepsetrangeofimagecommand.h"
#include "qcepallocator.h"
#include "qcepimagedata.h"

QcepSetRangeOfImageCommand::QcepSetRangeOfImageCommand()
  : QcepSerializableObject("setRangeOfImageCommand")
{

}

void QcepSetRangeOfImageCommand::onImageAvailable(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res = exec(img);

  emit processedImageAvailable(res);
}

QcepDoubleImageDataPtr QcepSetRangeOfImageCommand::exec(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res;

  if (img) {
    res = QcepAllocator::newDoubleImage("dup",
                                        img->get_Width(), img->get_Height(), QcepAllocator::AlwaysAllocate);

    res->copyFrom(img);
    res->set_Name(tr("%1.seti").arg(img->get_Name()));
  }

  return res;
}
