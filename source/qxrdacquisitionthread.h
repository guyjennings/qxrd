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
  QxrdAcquisitionThread(QxrdExperimentWPtr doc,
                        QxrdDataProcessorWPtr proc,
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
  QxrdExperimentWPtr    m_Experiment;
  QxrdDataProcessorWPtr m_Processor;
  int                   m_DetectorType;
  QxrdAcquisitionPtr    m_Acquisition;
};

#endif
