#include "qxrdacquisitionthread.h"

#include "qxrdacquisition.h"
#include <QFuture>
#include <QVariant>
#include <QMetaObject>

QxrdAcquisitionThread::QxrdAcquisitionThread()
  : QThread(),
    m_Debug(true),
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
  m_Acquisition = new QxrdAcquisition(this);

//  connect(this,          SIGNAL(_evaluate(QString)),
//          m_Acquisition, SLOT(_evaluate(QString)));

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
  QMetaObject::invokeMethod(m_Acquisition, "acquire", Qt::QueuedConnection);
}

void QxrdAcquisitionThread::doAcquireDark()
{
  QMetaObject::invokeMethod(m_Acquisition, "acquireDark", Qt::QueuedConnection);
}

void QxrdAcquisitionThread::msleep(int msec)
{
  QThread::msleep(msec);
}

void QxrdAcquisitionThread::cancel()
{
  QMetaObject::invokeMethod(m_Acquisition, "cancel", Qt::QueuedConnection);
}

void QxrdAcquisitionThread::cancelDark()
{
  QMetaObject::invokeMethod(m_Acquisition, "cancelDark", Qt::QueuedConnection);
}

QVector<double> QxrdAcquisitionThread::readoutTimes()
{
  return m_Acquisition -> readoutTimes();
}

QVariant QxrdAcquisitionThread::evaluate(QString cmd)
{
  QMutexLocker lock(&m_EvalMutex);

  QMetaObject::invokeMethod(m_Acquisition, "evaluate",
                            Qt::QueuedConnection,
                            Q_ARG(QString, cmd));

  waitForResult();

  return m_EvalResult;
}

void QxrdAcquisitionThread::waitForResult()
{
  m_EvalWaitCondition.wait(&m_EvalMutex, 10000);
}

void QxrdAcquisitionThread::setResult(QVariant res)
{
  m_EvalResult = res;

  m_EvalWaitCondition.wakeAll();
}

QxrdAcquisition *QxrdAcquisitionThread::acquisition() const
{
  return m_Acquisition;
}

void QxrdAcquisitionThread::sleep(double time)
{
  QThread::usleep((int)(time*1e6));
}
