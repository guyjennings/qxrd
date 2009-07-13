/******************************************************************
*
*  $Id: qxrdacquisitionthread.cpp,v 1.32 2009/07/13 23:19:37 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionthread.h"

#include "qxrdacquisition.h"
#include <QFuture>
#include <QVariant>
#include <QMetaObject>

QxrdAcquisitionThread::QxrdAcquisitionThread(QxrdDataProcessor *proc)
  : QThread(),
    m_Debug(true),
    m_Acquisition(NULL),
    SOURCE_IDENT("$Id: qxrdacquisitionthread.cpp,v 1.32 2009/07/13 23:19:37 jennings Exp $")
{
  m_Acquisition = new QxrdAcquisition(/*this, */proc);
  m_Acquisition -> moveToThread(this);
}

QxrdAcquisitionThread::~QxrdAcquisitionThread()
{
  shutdown();

  delete m_Acquisition;
}

void QxrdAcquisitionThread::run()
{
  m_Acquisition -> initialize();

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

//QVariant QxrdAcquisitionThread::evaluate(QString cmd)
//{
//  QMutexLocker lock(&m_EvalMutex);
//
//  QMetaObject::invokeMethod(m_Acquisition, "evaluate",
//                            Qt::QueuedConnection,
//                            Q_ARG(QString, cmd));
//
//  waitForResult();
//
//  emit printMessage(tr("%1 = %2").arg(cmd).arg(m_EvalResult.toString()));
//
//  return m_EvalResult;
//}

//void QxrdAcquisitionThread::waitForResult()
//{
//  m_EvalWaitCondition.wait(&m_EvalMutex, 10000);
//}
//
//void QxrdAcquisitionThread::setResult(QVariant res)
//{
//  m_EvalResult = res;
//
//  m_EvalWaitCondition.wakeAll();
//}

QxrdAcquisition *QxrdAcquisitionThread::acquisition() const
{
  return m_Acquisition;
}

void QxrdAcquisitionThread::sleep(double time)
{
  QThread::usleep((int)(time*1e6));
}

/******************************************************************
*
*  $Log: qxrdacquisitionthread.cpp,v $
*  Revision 1.32  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.31  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.30  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.29  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

