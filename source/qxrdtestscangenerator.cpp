#include "qxrdtestscangenerator.h"

QxrdTestScanGenerator::QxrdTestScanGenerator(QString name)
  : QxrdTestGenerator(name)
{
}

void QxrdTestScanGenerator::start()
{
  printMessage("Test Scan Generator Started");
}

void QxrdTestScanGenerator::generateTest()
{
  emit newScanAvailable(QcepDataColumnScanPtr());
}
