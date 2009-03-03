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

  connect(this, SIGNAL(_acquire(int,int,int)), m_Acquisition, SLOT(acquire(int,int,int)));
  connect(m_Acquisition, SIGNAL(printMessage(QString)), this, SIGNAL(printMessage(QString)));
  connect(m_Acquisition, SIGNAL(acquireComplete()), this, SIGNAL(acquireComplete()));

  m_Acquisition -> initialize();

  emit acquisitionRunning();

  exec();
}

void QxrdAcquisitionThread::shutdown()
{
  exit();

  wait(1000);
}

void QxrdAcquisitionThread::saveData(QString name)
{
  m_Acquisition -> saveData(name);
}

void QxrdAcquisitionThread::acquire(int integmode, int nsum, int nframes)
{
  emit _acquire(integmode, nsum, nframes);
}

void QxrdAcquisitionThread::msleep(int msec)
{
  QThread::msleep(msec);
}

void QxrdAcquisitionThread::cancel()
{
  m_Acquisition -> cancel();
}

QVector<double> QxrdAcquisitionThread::integrationTimes()
{
  return m_Acquisition -> integrationTimes();
}
