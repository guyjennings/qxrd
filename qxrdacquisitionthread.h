/******************************************************************
*
*  $Id: qxrdacquisitionthread.h,v 1.28 2009/07/10 22:54:23 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include "qcepmacros.h"

#include <QThread>
#include <QVector>
#include <QVariant>
#include <QMutex>
#include <QWaitCondition>

class QxrdAcquisition;
class QxrdDataProcessor;

class QxrdAcquisitionThread : public QThread
{
  Q_OBJECT;

 public:
  QxrdAcquisitionThread(QxrdDataProcessor *proc);
  ~QxrdAcquisitionThread();

  void shutdown();
  void msleep(int msec);
  QVector<double> readoutTimes();

 public slots:
  void doAcquire();
  void cancel();

  void doAcquireDark();
  void cancelDark();

signals:
  void printMessage(QString msg);

public:
  QxrdAcquisition* acquisition() const;

public:
  void sleep(double time);

public:
  QVariant evaluate(QString cmd);
  void setResult(QVariant res);
  //signals:
  //  void _evaluate(QString cmd);
private:
  QMutex         m_EvalMutex;
  QWaitCondition m_EvalWaitCondition;
  void waitForResult();
  QVariant       m_EvalResult;

protected:
  void run();

private:
  int                    m_Debug;
  QxrdAcquisition       *m_Acquisition;
  HEADER_IDENT("$Id: qxrdacquisitionthread.h,v 1.28 2009/07/10 22:54:23 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdacquisitionthread.h,v $
*  Revision 1.28  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.27  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

