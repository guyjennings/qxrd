#ifndef QXRDDATAPROCESSORTHREAD_H
#define QXRDDATAPROCESSORTHREAD_H

#include "qxrdthread.h"
#include "qxrdsettingssaver.h"
#include "qxrdacquisition.h"
#include <QDateTime>
#include <QSettings>

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
                          QxrdAcquisitionPtr acq,
                          QxrdAllocator *allocator,
                          QxrdFileSaverThread *filesaver,
                          QSettings *settings,
                          QString section);

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
  QxrdAcquisitionPtr                  m_Acquisition;
  QAtomicPointer<QxrdExperiment>      m_Experiment;
  QxrdSettingsSaver                  *m_Saver;
  QSettings                          *m_Settings;
  QString                             m_Section;
};

#endif // QXRDDATAPROCESSORTHREAD_H
