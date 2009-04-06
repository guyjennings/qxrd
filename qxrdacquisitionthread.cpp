#include "qxrdacquisitionthread.h"

#include "qxrdacquisition.h"

QxrdAcquisitionThread::QxrdAcquisitionThread()
  : QThread(),
    m_Debug(true),
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
  connect(m_Acquisition, SIGNAL(acquireStarted(int)), this, SIGNAL(acquireStarted(int)));
  connect(m_Acquisition, SIGNAL(acquireComplete(int)), this, SIGNAL(acquireComplete(int)));
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

void QxrdAcquisitionThread::doAcquire()
{
  QString outDir   = outputDirectory();
  QString filePatt = filePattern();
  int    index     = fileIndex();
  int    integmode = integrationMode();
  int    nsum      = nSummed();
  int    nframes   = nFrames();

  acquire(outDir, filePatt, index, integmode, nsum, nframes);
}

void QxrdAcquisitionThread::acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes)
{
  if (m_Acquisition -> canStart()) {
    emit _acquire(outDir, filePattern, fileIndex, integmode, nsum, nframes);
  } else {
    printf("Attempting to start acquisition while it is already running..\n");
  }
}

void QxrdAcquisitionThread::doAcquireDark()
{
  QString outDir   = outputDirectory();
  QString filePatt = filePattern();
  int    index     = fileIndex();
  int    integmode = integrationMode();
  int     nsum     = darkNSummed();

  acquireDark(outDir, filePatt, index, integmode, nsum);
}

void QxrdAcquisitionThread::acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum)
{
  if (m_Acquisition -> canStart()) {
    emit _acquireDark(outDir, filePattern, fileIndex, integmode, nsum);
  } else {
    printf("Attempting to start acquisition while it is already running..\n");
  }

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
  return m_Acquisition -> acquisitionStatus(time);
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

void QxrdAcquisitionThread::setExposureTime(double t)
{
  QWriteLocker lock(&m_Lock);

  if (m_Debug) {
    printf("QxrdAcquisitionThread::setExposureTime(%g->%g)\n", m_ExposureTime, t);
  }

  if (m_ExposureTime != t) {
    m_ExposureTime = t;

    emit exposureTimeChanged(t);
  }
}

void QxrdAcquisitionThread::setIntegrationMode(int mode)
{
  QWriteLocker lock(&m_Lock);

  if (m_Debug) {
    printf("QxrdAcquisitionThread::setIntegrationMode(%d->%d)\n", m_IntegrationMode, mode);
  }

  if (m_IntegrationMode != mode) {
    m_IntegrationMode = mode;

    emit integrationModeChanged(mode);
  }
}

void QxrdAcquisitionThread::setNSummed(int nsummed)
{
  QWriteLocker lock(&m_Lock);

  if (m_Debug) {
    printf("QxrdAcquisitionThread::setNSummed(%d->%d)\n", m_NSummed, nsummed);
  }

  if (m_NSummed != nsummed) {
    m_NSummed = nsummed;

    emit nSummedChanged(nsummed);
  }
}

void QxrdAcquisitionThread::setNFrames(int nframes)
{
  QWriteLocker lock(&m_Lock);

  if (m_Debug) {
    printf("QxrdAcquisitionThread::setNFrames(%d->%d)\n", m_NFrames, nframes);
  }

  if (m_NFrames != nframes) {
    m_NFrames = nframes;

    emit nFramesChanged(nframes);
  }
}

void QxrdAcquisitionThread::setFileIndex(int index)
{
  QWriteLocker lock(&m_Lock);

  if (m_Debug) {
    printf("QxrdAcquisitionThread::setFileIndex(%d->%d)\n", m_FileIndex, index);
  }

  if (m_FileIndex != index) {
    m_FileIndex = index;

    emit fileIndexChanged(index);
  }
}

void QxrdAcquisitionThread::setFilePattern(QString pattern)
{
  QWriteLocker lock(&m_Lock);

  if (m_Debug) {
    printf("QxrdAcquisitionThread::setFilePattern(%s->%s)\n", qPrintable(m_FilePattern), qPrintable(pattern));
  }

  if (m_FilePattern != pattern) {
    m_FilePattern = pattern;

    emit filePatternChanged(pattern);
  }
}

void QxrdAcquisitionThread::setOutputDirectory(QString path)
{
  QWriteLocker lock(&m_Lock);

  if (m_Debug) {
    printf("QxrdAcquisitionThread::setOutputDirectory(%s->%s)\n", qPrintable(m_OutputDirectory), qPrintable(path));
  }

  if (m_OutputDirectory != path) {
    m_OutputDirectory = path;

    emit outputDirectoryChanged(path);
  }
}

void QxrdAcquisitionThread::setDarkNSummed(int nsummed)
{
  QWriteLocker lock(&m_Lock);

  if (m_Debug) {
    printf("QxrdAcquisitionThread::setDarkNSummed(%d->%d)\n", m_DarkNSummed, nsummed);
  }

  if (m_DarkNSummed != nsummed) {
    m_DarkNSummed = nsummed;

    emit darkNSummedChanged(nsummed);
  }
}

double  QxrdAcquisitionThread::exposureTime()
{
  QReadLocker lock(&m_Lock);

  return m_ExposureTime;
}

int     QxrdAcquisitionThread::integrationMode()
{
  QReadLocker lock(&m_Lock);

  return m_IntegrationMode;
}

int     QxrdAcquisitionThread::nSummed()
{
  QReadLocker lock(&m_Lock);

  return m_NSummed;
}

int     QxrdAcquisitionThread::nFrames()
{
  QReadLocker lock(&m_Lock);

  return m_NFrames;
}

int     QxrdAcquisitionThread::fileIndex()
{
  QReadLocker lock(&m_Lock);

  return m_FileIndex;
}

QString QxrdAcquisitionThread::filePattern()
{
  QReadLocker lock(&m_Lock);

  return m_FilePattern;
}

QString QxrdAcquisitionThread::outputDirectory()
{
  QReadLocker lock(&m_Lock);

  return m_OutputDirectory;
}

int     QxrdAcquisitionThread::darkNSummed()
{
  QReadLocker lock(&m_Lock);

  return m_DarkNSummed;
}

