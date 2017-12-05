#include "qxrddetectorinterface.h"
#include "qcepallocator.h"

QxrdDetectorInterface::QxrdDetectorInterface(QString name)
  : QcepObject(name)
{
}

void QxrdDetectorInterface::ping()
{
  static QcepDoubleImageDataPtr img =
      QcepAllocator::newDoubleImage("junk", 2048, 2048, QcepAllocator::AlwaysAllocate);

  emit imageDataAvailable(img);
}

QcepDoubleImageDataPtr QxrdDetectorInterface::newImage()
{
  static QcepDoubleImageDataPtr img =
      QcepAllocator::newDoubleImage("junk", 2048, 2048, QcepAllocator::AlwaysAllocate);

  printMessage(tr("QxrdDetectorInterface::newImage"));

  return img;
}
