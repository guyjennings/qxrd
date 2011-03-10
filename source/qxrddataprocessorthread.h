#ifndef QXRDDATAPROCESSORTHREAD_H
#define QXRDDATAPROCESSORTHREAD_H

#include <QThread>
#include <QDateTime>

class QxrdAcquisition;
class QxrdAllocator;
class QxrdFileSaverThread;
class QxrdDataProcessor;

class QxrdDataProcessorThread : public QThread
{
  Q_OBJECT;

public:
  QxrdDataProcessorThread(QxrdAcquisition *acq, QxrdAllocator *allocator, QxrdFileSaverThread *saver);
  ~QxrdDataProcessorThread();

  void shutdown();

  QxrdDataProcessor *dataProcessor() const;

  static void msleep(long unsigned int);

signals:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

protected:
  void run();

private:
  QAtomicPointer<QxrdAllocator>       m_Allocator;
  QAtomicPointer<QxrdFileSaverThread> m_FileSaverThread;
  QAtomicPointer<QxrdDataProcessor>   m_DataProcessor;
  QAtomicPointer<QxrdAcquisition>     m_Acquisition;
};

#endif // QXRDDATAPROCESSORTHREAD_H
