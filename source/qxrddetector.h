#ifndef QXRDDETECTOR_H
#define QXRDDETECTOR_H

#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepobject.h"
#include <QDoubleSpinBox>
#include <QComboBox>

#include "qxrdacquisition-ptr.h"
#include "qxrdexperiment-ptr.h"

class QxrdDetector : public QcepObject
{
  Q_OBJECT

public:
  explicit QxrdDetector(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detType);
  virtual ~QxrdDetector();

  virtual void initialize();
  virtual int detectorType() const;
  virtual QString detectorTypeName() const;
  virtual int detectorSubType() const;
  virtual QString detectorAddress() const;

signals:

public slots:
  virtual void onExposureTimeChanged();
  virtual void onBinningModeChanged();
  virtual void onCameraGainChanged();

  virtual void setupExposureMenu(QDoubleSpinBox *cb, double initialExposure);
  virtual void setupCameraGainMenu(QComboBox *cb, int initialGain);
  virtual void setupCameraBinningModeMenu(QComboBox *cb, int initialBinning);

  virtual void beginAcquisition();
  virtual void endAcquisition();
  virtual void shutdownAcquisition();

protected:
  QcepSettingsSaverWPtr m_Saver;
  QxrdExperimentWPtr    m_Experiment;
  QxrdAcquisitionWPtr   m_Acquisition;

public:
  Q_PROPERTY(int detectorType READ get_DetectorType WRITE set_DetectorType)
  QCEP_INTEGER_PROPERTY(DetectorType)
};

#endif // QXRDDETECTOR_H
