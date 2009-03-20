#include "qxrdacquisitionthread.h"

#include "qxrdacquisition.h"
#include "qxrdwindow.h"

QxrdAcquisitionThread::QxrdAcquisitionThread(QxrdApplication *app, QxrdWindow *win)
  : QThread(),
    m_Application(app),
    m_Window(win),
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
  emit printMessage(tr("Acquisition thread %1\n").arg((long) QThread::currentThread()));

  m_Acquisition = new QxrdAcquisition(m_Application, m_Window, this);

  connect(this, SIGNAL(_acquire(QString,QString,int,int,int,int)),
          m_Acquisition, SLOT(acquire(QString,QString,int,int,int,int)));
  connect(this, SIGNAL(_acquireDark(QString,QString,int,int,int)),
          m_Acquisition, SLOT(acquireDark(QString,QString,int,int,int)));
  connect(m_Acquisition, SIGNAL(printMessage(QString)), this, SIGNAL(printMessage(QString)));
  connect(m_Acquisition, SIGNAL(acquireComplete()), this, SIGNAL(acquireComplete()));
  connect(m_Acquisition, SIGNAL(acquiredFrame(QString,int,int,int,int,int)), 
	  this, SIGNAL(acquiredFrame(QString,int,int,int,int,int)));
  connect(m_Acquisition, SIGNAL(summedFrameCompleted(QString,int)),
	  this, SIGNAL(summedFrameCompleted(QString,int)));
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

//void QxrdAcquisitionThread::saveData(QString name)
//{
//  m_Acquisition -> saveData(name);
//}
//
//void QxrdAcquisitionThread::loadData(QString name)
//{
//  m_Acquisition -> loadData(name);
//}

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

void QxrdAcquisitionThread::setWindow(QxrdWindow *win)
{
  m_Window = win;

  m_Acquisition -> setWindow(win);
}

QxrdWindow *QxrdAcquisitionThread::window()
{
  return m_Window;
}

void QxrdAcquisitionThread::enqueue(QxrdImageData *img)
{
  m_Acquisition -> enqueue(img);
}

QxrdImageData *QxrdAcquisitionThread::nextAvailableImage()
{
  return m_Acquisition -> nextAvailableImage();
}
