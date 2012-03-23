#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include "qxrdthread.h"
#include <QSettings>
#include "qxrdallocator-ptr.h"
#include "qxrdsettingssaver-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrddataprocessor-ptr.h"

class QxrdAcquisitionThread : public QxrdThread
{
  Q_OBJECT

 public:
  QxrdAcquisitionThread(QxrdSettingsSaverWPtr saver,
                        QxrdExperimentWPtr doc,
                        QxrdDataProcessorWPtr proc,
                        QxrdAllocatorWPtr allocator,
                        int detectorType);

  ~QxrdAcquisitionThread();

  void shutdown();
  void msleep(int msec);

public:
  QxrdAcquisitionPtr acquisition() const;

public:
  void sleep(double time);

protected:
  void run();

private:
  int                   m_Debug;
  QxrdSettingsSaverWPtr m_Saver;
  QxrdExperimentWPtr    m_Experiment;
  QxrdAllocatorWPtr     m_Allocator;
  QxrdAcquisitionWPtr   m_Acquisition;
  QxrdDataProcessorWPtr m_Processor;
  int                   m_DetectorType;
};

#endif
