#include "qxrdacquisitionthread.h"

#include "qxrdacquisition.h"

QxrdAcquisitionThread::QxrdAcquisitionThread()
  : QThread(),
    m_Acquisition(NULL),
    m_FreeImages("Free Image Pool"),
    m_AcquiredImages("Acquired Images")
{
}

QxrdAcquisitionThread::~QxrdAcquisitionThread()
{
  shutdown();

  delete m_Acquisition;
}

void QxrdAcquisitionThread::run()
{
  emit printMessage(tr("Acquisition thread %1\n").arg((long) QThread::currentThread()));

  m_Acquisition = new QxrdAcquisition(this);

  connect(this, SIGNAL(_acquire(QString,QString,int,int,int,int)),
          m_Acquisition, SLOT(acquire(QString,QString,int,int,int,int)));
  connect(this, SIGNAL(_acquireDark(QString,QString,int,int,int)),
          m_Acquisition, SLOT(acquireDark(QString,QString,int,int,int)));
  connect(m_Acquisition, SIGNAL(printMessage(QString)), this, SIGNAL(printMessage(QString)));
  connect(m_Acquisition, SIGNAL(acquireComplete()), this, SIGNAL(acquireComplete()));
  connect(m_Acquisition, SIGNAL(acquiredFrame(QString,int,int,int,int,int)), 
	  this, SIGNAL(acquiredFrame(QString,int,int,int,int,int)));
  connect(m_Acquisition, SIGNAL(fileIndexChanged(int)), this, SIGNAL(fileIndexChanged(int)));
  connect(m_Acquisition, SIGNAL(statusMessage(QString)), this, SIGNAL(statusMessage(QString)));

  m_Acquisition -> initialize();

  emit acquisitionRunning();

  exec();
}

void QxrdAcquisitionThread::shutdown()
{
  exit();

  wait(1000);
}

void QxrdAcquisitionThread::acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes)
{
  emit _acquire(outDir, filePattern, fileIndex, integmode, nsum, nframes);
}

void QxrdAcquisitionThread::acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum)
{
  emit _acquireDark(outDir, filePattern, fileIndex, integmode, nsum);
}

void QxrdAcquisitionThread::msleep(int msec)
{
  QThread::msleep(msec);
}

void QxrdAcquisitionThread::cancel()
{
  m_Acquisition -> cancel();
}

void QxrdAcquisitionThread::cancelDark()
{
  m_Acquisition -> cancelDark();
}

QVector<double> QxrdAcquisitionThread::integrationTimes()
{
  return m_Acquisition -> integrationTimes();
}

int QxrdAcquisitionThread::acquisitionStatus(double time)
{
  return m_Acquisition -> acquisitionStatus();
}

QxrdImageData *QxrdAcquisitionThread::takeNextFreeImage()
{
  if (m_FreeImages.size() == 0) {
    printf("Allocate new image\n");
    return new QxrdImageData(2048, 2048);
  } else {
    return m_FreeImages.dequeue();
  }
}

QxrdImageData *QxrdAcquisitionThread::takeNextAcquiredImage()
{
  return m_AcquiredImages.dequeue();
}

void QxrdAcquisitionThread::newAcquiredImage(QxrdImageData *img)
{
  m_AcquiredImages.enqueue(img);

  emit acquiredImageAvailable();
}

void QxrdAcquisitionThread::returnImageToPool(QxrdImageData *img)
{
  m_FreeImages.enqueue(img);
}
