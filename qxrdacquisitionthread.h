/******************************************************************
*
*  $Id: qxrdacquisitionthread.h,v 1.30 2009/09/20 21:18:53 jennings Exp $
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
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public:
  QxrdAcquisition* acquisition() const;

public:
  void sleep(double time);

//public:
//  QVariant evaluate(QString cmd);
//  void setResult(QVariant res);
//
//private:
//  void waitForResult();

protected:
  void run();

private:
  QMutex                 m_EvalMutex;
  QWaitCondition         m_EvalWaitCondition;
  QVariant               m_EvalResult;
  int                    m_Debug;
  QxrdAcquisition       *m_Acquisition;
  HEADER_IDENT("$Id: qxrdacquisitionthread.h,v 1.30 2009/09/20 21:18:53 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdacquisitionthread.h,v $
*  Revision 1.30  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.29  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.28  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.27  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

