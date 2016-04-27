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
  explicit QxrdDetectorThread(QxrdExperimentWPtr expt,
                              QxrdAcquisitionWPtr acq,
                              int detType,
                              int detNum,
                              QcepObjectWPtr parent);
  virtual ~QxrdDetectorThread();

  QxrdDetectorWPtr detector() const;
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

  typedef enum {
    Binning1x1,
    Binning2x2Average,
    Binning2x2Summed,
    BinningModeCount
  } BinningModePE;

  static int detectorTypeCount();
  static QString detectorTypeName(int detectorType);
  static QStringList detectorTypeNames();

  static QString detectorSubTypeNamePE(int detectorSubType);
  static QStringList detectorSubTypeNamesPE();

  static QStringList gainModeNamesPE();

  static QString     binningModeNamePE(int binningMode);
  static QStringList binningModeNamesPE();

  static void pushDefaultsToProxy(int detectorType, QxrdDetectorProxyPtr proxy);

signals:

public slots:

protected:
  void run();

private:
  QxrdExperimentWPtr    m_Experiment;
  QxrdAcquisitionWPtr   m_Acquisition;
  QxrdDetectorPtr       m_Detector;
  int                   m_DetectorType;
  int                   m_DetectorNumber;
};

#endif // QXRDDETECTORTHREAD_H
