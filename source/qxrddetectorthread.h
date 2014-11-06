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
  virtual ~QxrdDetectorThread();

  QxrdDetectorPtr detector() const;
  void shutdown();

  typedef enum {
    NoDetector,
    SimulatedDetector,
    PerkinElmerDetector,
    PilatusDetector,
    EpicsAreaDetector,
    FileWatcherDetector
  } DetectorType;

  typedef enum {
    PCI_SubType,
    GBIF_IP_SubType,
    GBIF_MAC_SubType,
    GBIF_Name_SubType,
    GBIF_Scan_SubType
  } DetectorSubType;

  static QString detectorTypeName(int detectorType);
  static QStringList detectorTypeNames();

  static QString detectorSubTypeName(int detectorSubType);
  static QStringList detectorSubTypeNames();

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
