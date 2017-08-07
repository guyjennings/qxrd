#include "qcepfixupgainmapcommand.h"
#include "qcepallocator.h"
#include "qcepimagedata.h"

QcepFixupGainMapCommand::QcepFixupGainMapCommand()
  : QcepSerializableObject("fixupGainMapCommand")
{

}

void QcepFixupGainMapCommand::onImageAvailable(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res = exec(img);

  emit processedImageAvailable(res);
}

QcepDoubleImageDataPtr QcepFixupGainMapCommand::exec(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res;

  if (img) {
    res = QcepAllocator::newDoubleImage("dup",
                                        img->get_Width(), img->get_Height(), QcepAllocator::AlwaysAllocate);

    res->copyFrom(img);

    res->set_Name(tr("%1.fix").arg(img->get_Name()));
  }

  return res;
}
