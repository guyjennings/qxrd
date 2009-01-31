#include "qxrdacquisitionthread.h"

#include "qxrdacquisition.h"

QxrdAcquisitionThread::QxrdAcquisitionThread(QxrdApplication *app)
  : QThread(),
    m_Application(app),
    m_Acquisition(NULL)
{
}

QxrdAcquisitionThread::~QxrdAcquisitionThread()
{
  shutdown();

  delete m_Acquisition;
}

void QxrdAcquisitionThread::run()
{
  printf("Acquisition thread %p\n", QThread::currentThread());

  m_Acquisition = new QxrdAcquisition(m_Application);

  emit acquisitionRunning();

  exec();
}

void QxrdAcquisitionThread::shutdown()
{
  exit();

  wait(1000);
}
