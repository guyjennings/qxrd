#ifndef QXRDDATAPROCESSORTHREAD_H
#define QXRDDATAPROCESSORTHREAD_H

#include "qxrdthread.h"
#include "qxrdsettingssaver.h"
#include "qxrdacquisition.h"
#include "qxrddataprocessor.h"
#include <QDateTime>
#include <QSettings>

class QxrdDataProcessorThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdDataProcessorThread(QxrdSettingsSaverPtr saver,
                          QxrdExperimentPtr doc,
                          QxrdAcquisitionPtr acq,
                          QxrdAllocatorPtr allocator,
                          QxrdFileSaverPtr filesaver,
                          QSettings *settings,
                          QString section);

  ~QxrdDataProcessorThread();

  void shutdown();

  QxrdDataProcessorPtr dataProcessor() const;

  static void msleep(long unsigned int);

protected:
  void run();

private:
  QxrdAllocatorPtr        m_Allocator;
  QxrdFileSaverPtr        m_FileSaver;
  QxrdDataProcessorPtr    m_DataProcessor;
  QxrdAcquisitionPtr      m_Acquisition;
  QxrdExperimentPtr       m_Experiment;
  QxrdSettingsSaverPtr    m_Saver;
  QSettings              *m_Settings;
  QString                 m_Section;
};

typedef QSharedPointer<QxrdDataProcessorThread> QxrdDataProcessorThreadPtr;

#endif // QXRDDATAPROCESSORTHREAD_H
