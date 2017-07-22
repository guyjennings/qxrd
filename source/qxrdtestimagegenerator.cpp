#include "qxrdtestimagegenerator.h"

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
  emit newImageAvailable(QcepDoubleImageDataPtr());
}
