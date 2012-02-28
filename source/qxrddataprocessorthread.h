#ifndef QXRDDATAPROCESSORTHREAD_H
#define QXRDDATAPROCESSORTHREAD_H

#include "qxrdthread.h"
#include "qxrdsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdallocator-ptr.h"
#include "qxrdfilesaver-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddataprocessor-ptr.h"
#include <QSettings>

class QxrdDataProcessorThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdDataProcessorThread(QxrdSettingsSaverWPtr saver,
                          QxrdExperimentWPtr doc,
                          QxrdAcquisitionWPtr acq,
                          QxrdAllocatorWPtr allocator,
                          QxrdFileSaverWPtr filesaver);

  ~QxrdDataProcessorThread();

  void shutdown();

  QxrdDataProcessorPtr dataProcessor() const;

  static void msleep(long unsigned int);

protected:
  void run();

private:
  QxrdAllocatorWPtr       m_Allocator;
  QxrdFileSaverWPtr       m_FileSaver;
  QxrdDataProcessorPtr    m_DataProcessor;
  QxrdAcquisitionWPtr     m_Acquisition;
  QxrdExperimentWPtr      m_Experiment;
  QxrdSettingsSaverWPtr   m_Saver;
};

typedef QSharedPointer<QxrdDataProcessorThread> QxrdDataProcessorThreadPtr;

#endif // QXRDDATAPROCESSORTHREAD_H
