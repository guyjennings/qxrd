#ifndef QXRDDATAPROCESSORTHREAD_H
#define QXRDDATAPROCESSORTHREAD_H

#include "qxrdthread.h"
#include "qxrdsettingssaver.h"
#include <QDateTime>

class QxrdAcquisition;
class QxrdAllocator;
class QxrdFileSaverThread;
class QxrdDataProcessor;
class QxrdExperiment;

class QxrdDataProcessorThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdDataProcessorThread(QxrdSettingsSaver *saver,
                          QxrdExperiment *doc,
                          QxrdAcquisition *acq,
                          QxrdAllocator *allocator,
                          QxrdFileSaverThread *filesaver);
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
  QAtomicPointer<QxrdExperiment>      m_Experiment;
  QxrdSettingsSaver                  *m_Saver;
};

#endif // QXRDDATAPROCESSORTHREAD_H
