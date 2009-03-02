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

  m_Acquisition = new QxrdAcquisition(m_Application, this);

  connect(this, SIGNAL(_acquire(double,int,int)), m_Acquisition, SLOT(acquire(double,int,int)));
  connect(m_Acquisition, SIGNAL(printMessage(QString)), this, SIGNAL(printMessage(QString)));
  connect(m_Acquisition, SIGNAL(acquireComplete()), this, SIGNAL(acquireComplete()));

  emit acquisitionRunning();

  m_Acquisition -> initialize();

  exec();
}

void QxrdAcquisitionThread::shutdown()
{
  exit();

  wait(1000);
}

void QxrdAcquisitionThread::acquire(double expos, int nsum, int nframes)
{
  emit _acquire(expos, nsum, nframes);
}

void QxrdAcquisitionThread::msleep(int msec)
{
  QThread::msleep(msec);
}

void QxrdAcquisitionThread::cancel()
{
  m_Acquisition -> cancel();
}
