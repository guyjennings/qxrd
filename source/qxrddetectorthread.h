#ifndef QXRDDETECTORTHREAD_H
#define QXRDDETECTORTHREAD_H

#include "qxrdthread.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddetector-ptr.h"
#include "qxrddetector.h"

class QxrdDetectorThread : public QxrdThread
{
  Q_OBJECT
public:
  explicit QxrdDetectorThread(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq);

  QxrdDetectorPtr detector() const;
  void shutdown();

  typedef enum {
    SimulatedDetector,
    PerkinElmerDetector,
    PilatusDetector,
    EpicsAreaDetector,
    FileWatcherDetector
  } DetectorKind;

  static QString detectorKindName(int detectorKind);

signals:

public slots:

protected:
  void run();

private:
  QxrdExperimentWPtr  m_Experiment;
  QxrdAcquisitionWPtr m_Acquisition;
  QxrdDetectorPtr     m_Detector;
};

#endif // QXRDDETECTORTHREAD_H
