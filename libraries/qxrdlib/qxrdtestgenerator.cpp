#include "qxrdtestgenerator.h"
#include "qxrdtestimageplotdatavishelper.h"
#include "qxrdtestimageplotqwthelper.h"
#include "qxrdtestscanplotdatavishelper.h"
#include "qxrdtestscanplotqwthelper.h"

QxrdTestGenerator::QxrdTestGenerator(QString name)
  : QcepSerializableObject(name),
    m_TestIndex(this, "testIndex", 0, "Test Data Index")
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

void QxrdTestGenerator::registerMetaTypes()
{
#ifdef HAVE_DATAVIS
  qRegisterMetaType<QSurface3DSeriesPtr>("QSurface3DSeriesPtr");
#endif

  qRegisterMetaType<QwtPlotCurvePtr>("QwtPlotCurvePtr");
}
