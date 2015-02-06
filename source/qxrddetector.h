#ifndef QXRDDETECTOR_H
#define QXRDDETECTOR_H

#include <QObject>
#include <QDoubleSpinBox>
#include <QComboBox>

#include "qxrdacquisition-ptr.h"
#include "qxrdexperiment-ptr.h"

class QxrdDetector : public QObject
{
  Q_OBJECT
public:
  explicit QxrdDetector(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq);
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
  QxrdExperimentWPtr  m_Experiment;
  QxrdAcquisitionWPtr m_Acquisition;
};

#endif // QXRDDETECTOR_H
