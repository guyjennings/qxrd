#include "qxrdacquisitionthread.h"

#include "qxrdacquisition.h"
#include <QFuture>
#include <QVariant>

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

  connect(this,          SIGNAL(_evaluate(QString)),
          m_Acquisition, SLOT(_evaluate(QString)));

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
  evaluate("acquire()");
}

void QxrdAcquisitionThread::doAcquireDark()
{
  evaluate("acquiredark()");
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

QVector<double> QxrdAcquisitionThread::readoutTimes()
{
  return m_Acquisition -> readoutTimes();
}

QVariant QxrdAcquisitionThread::evaluate(QString cmd)
{
  QMutexLocker lock(&m_EvalMutex);

  emit _evaluate(cmd);

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
