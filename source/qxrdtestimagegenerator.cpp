#include "qxrdtestimagegenerator.h"
#include "qcepallocator.h"

QxrdTestImageGenerator::QxrdTestImageGenerator(QString name)
  : QxrdTestGenerator(name)
{
}

void QxrdTestImageGenerator::start()
{
  printMessage("Test Image Generator Started");
}

void QxrdTestImageGenerator::generateTest()
{
  int ind = get_TestIndex();

  prop_TestIndex()->incValue(1);

  QcepDoubleImageDataPtr img = QcepAllocator::newDoubleImage("newData", 2048, 2048, QcepAllocator::NullIfNotAvailable);

  if (img) {
    img->set_ImageNumber(ind);
  }

  emit newImageAvailable(img);
}
