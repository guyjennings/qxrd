#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include <QThread>
#include <QVector>
#include <QVariant>
#include <QMutex>
#include <QWaitCondition>

class QxrdAcquisition;

class QxrdAcquisitionThread : public QThread
{
  Q_OBJECT;

 public:
  QxrdAcquisitionThread();
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
};

#endif
