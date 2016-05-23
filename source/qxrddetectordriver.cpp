#include "qxrddetectordriver.h"

QxrdDetectorDriver::QxrdDetectorDriver(QString name) :
  QcepObject(name)
{
}

bool QxrdDetectorDriver::startDetectorDriver()
{
  return false;
}

bool QxrdDetectorDriver::stopDetectorDriver()
{
  return false;
}
