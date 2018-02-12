#include "qxrddebug.h"
#include "qxrdtestscangeneratorthread.h"
#include "qxrdtestscangenerator.h"
#include "qxrdexperiment.h"

QxrdTestScanGeneratorThread::QxrdTestScanGeneratorThread(QxrdExperimentWPtr doc) :
  QxrdThread(doc)
{
  setObjectName("testScanGenerator");
}

QxrdTestScanGeneratorThread::~QxrdTestScanGeneratorThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdTestScanGeneratorThread::~QxrdTestScanGeneratorThread(%p)\n", this);
  }
}

void QxrdTestScanGeneratorThread::shutdown()
{
  exit();

  wait();
}

void QxrdTestScanGeneratorThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Test Scan Generator Thread Started\n");
  }

  m_Generator = QxrdTestScanGeneratorPtr(
        new QxrdTestScanGenerator("scanGen"));

  m_Generator -> start();

  int rc = exec();

  m_Generator = QxrdTestScanGeneratorPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Test Scan Generator Thread Terminated with rc %d\n", rc);
  }
}

QxrdTestScanGeneratorPtr QxrdTestScanGeneratorThread::generator() const
{
  while (isRunning()) {
    if (m_Generator) return m_Generator;

    msleep(50);
  }

  return QxrdTestScanGeneratorPtr();
}
