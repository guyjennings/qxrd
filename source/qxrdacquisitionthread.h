#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include "qxrdthread.h"
#include <QSettings>
#include "qcepallocator-ptr.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrddataprocessor-ptr.h"

class QxrdAcquisitionThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdAcquisitionThread(QcepSettingsSaverWPtr saver,
                        QxrdExperimentWPtr doc,
                        QxrdDataProcessorWPtr proc,
                        QcepAllocatorWPtr allocator,
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
  QcepSettingsSaverWPtr m_Saver;
  QxrdExperimentWPtr    m_Experiment;
  QcepAllocatorWPtr     m_Allocator;
  QxrdDataProcessorWPtr m_Processor;
  int                   m_DetectorType;
  QxrdAcquisitionPtr    m_Acquisition;
};

#endif
