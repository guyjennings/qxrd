#ifndef QXRDDATAPROCESSORTHREAD_H
#define QXRDDATAPROCESSORTHREAD_H

#include "qxrdthread.h"
#include <QDateTime>

class QxrdAcquisition;
class QxrdAllocator;
class QxrdFileSaverThread;
class QxrdDataProcessor;
class QxrdDocument;

class QxrdDataProcessorThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdDataProcessorThread(QxrdDocument *doc,
                          QxrdAcquisition *acq,
                          QxrdAllocator *allocator,
                          QxrdFileSaverThread *saver);
  ~QxrdDataProcessorThread();

  void shutdown();

  QxrdDataProcessor *dataProcessor() const;

  static void msleep(long unsigned int);

protected:
  void run();

private:
  QAtomicPointer<QxrdAllocator>       m_Allocator;
  QAtomicPointer<QxrdFileSaverThread> m_FileSaverThread;
  QAtomicPointer<QxrdDataProcessor>   m_DataProcessor;
  QAtomicPointer<QxrdAcquisition>     m_Acquisition;
  QAtomicPointer<QxrdDocument>        m_Document;
};

#endif // QXRDDATAPROCESSORTHREAD_H
