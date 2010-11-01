#ifndef QXRDDATAPROCESSORTHREAD_H
#define QXRDDATAPROCESSORTHREAD_H

#include "qcepmacros.h"

#include <QThread>
#include "qxrdforwardtypes.h"
#include "qxrddataprocessor.h"

class QxrdDataProcessorThread : public QThread
{
  Q_OBJECT;

public:
  QxrdDataProcessorThread(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver);
  ~QxrdDataProcessorThread();

  void shutdown();

  QxrdDataProcessorPtr dataProcessor() const;

  static void msleep(long unsigned int);

signals:
  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

protected:
  void run();

private:
  QAtomicPointer<QxrdAllocator>       m_Allocator;
  QAtomicPointer<QxrdFileSaverThread> m_FileSaverThread;
  QAtomicPointer<QxrdDataProcessor>   m_DataProcessor;
  QAtomicPointer<QxrdAcquisition>     m_Acquisition;
};

#endif // QXRDDATAPROCESSORTHREAD_H
