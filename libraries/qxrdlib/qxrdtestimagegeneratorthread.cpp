#include "qxrddebug.h"
#include "qxrdtestimagegeneratorthread.h"
#include "qxrdtestimagegenerator.h"
#include "qxrdexperiment.h"

QxrdTestImageGeneratorThread::QxrdTestImageGeneratorThread(QxrdExperimentWPtr doc) :
  QxrdThread(doc)
{

}

QxrdTestImageGeneratorThread::~QxrdTestImageGeneratorThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdTestImageGeneratorThread::~QxrdTestImageGeneratorThread(%p)\n", this);
  }
}

void QxrdTestImageGeneratorThread::shutdown()
{
  exit();

  wait();
}

void QxrdTestImageGeneratorThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Test Scan Generator Thread Started\n");
  }

  m_Generator = QxrdTestImageGeneratorPtr(
        new QxrdTestImageGenerator("imageGen"));

  m_Generator -> start();

  int rc = exec();

  m_Generator = QxrdTestImageGeneratorPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Test Image Generator Thread Terminated with rc %d\n", rc);
  }
}

QxrdTestImageGeneratorPtr QxrdTestImageGeneratorThread::generator()
{
  while (isRunning()) {
    if (m_Generator) return m_Generator;

    msleep(50);
  }

  return QxrdTestImageGeneratorPtr();
}
