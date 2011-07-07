#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include "qxrdthread.h"
#include <QAtomicPointer>
#include <QStringList>
#include <QDateTime>

class QxrdDataProcessor;
class QxrdAllocator;
class QxrdAcquisition;

class QxrdAcquisitionThread : public QxrdThread
{
  Q_OBJECT

 public:
  QxrdAcquisitionThread(QxrdDataProcessor *proc, QxrdAllocator *allocator, int detectorType);
  ~QxrdAcquisitionThread();

  void initialize();
  void shutdown();
  void msleep(int msec);

  static QStringList detectorTypeNames();
  static int detectorType();

public slots:
  void doAcquire();
  void cancel();

  void doAcquireDark();
  void cancelDark();

public:
  QxrdAcquisition *acquisition() const;

public:
  void sleep(double time);

protected:
  void run();

private:
  int                    m_Debug;
  QAtomicPointer<QxrdAllocator>       m_Allocator;
  QAtomicPointer<QxrdAcquisition>     m_Acquisition;
  QAtomicPointer<QxrdDataProcessor>   m_Processor;
  int                    m_DetectorType;
};

#endif
