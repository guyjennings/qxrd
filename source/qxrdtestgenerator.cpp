#include "qxrdtestgenerator.h"

QxrdTestGenerator::QxrdTestGenerator(QString name)
  : QcepSerializableObject(name)
{
  connect(&m_Timer, &QTimer::timeout, this, &QxrdTestGenerator::generateTest);
}

void QxrdTestGenerator::repRateZero()
{
  m_Timer.stop();
}

void QxrdTestGenerator::repRate10Hz()
{
  m_Timer.start(100);
}

void QxrdTestGenerator::repRate1Sec()
{
  m_Timer.start(1000);
}

void QxrdTestGenerator::repRate10Sec()
{
  m_Timer.start(10000);
}

void QxrdTestGenerator::generateOne()
{
  m_Timer.stop();
  generateTest();
}
