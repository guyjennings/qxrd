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

signals:

public slots:
  virtual void onExposureTimeChanged(double newTime);
  virtual void onBinningModeChanged(int newMode);
  virtual void onCameraGainChanged(int newGain);

  virtual void setupExposureMenu(QDoubleSpinBox *cb);
  virtual void setupCameraGainMenu(QComboBox *cb);
  virtual void setupCameraBinningModeMenu(QComboBox *cb);

  virtual void beginAcquisition();
  virtual void endAcquisition();
  virtual void shutdownAcquisition();

protected:
  QxrdExperimentWPtr  m_Experiment;
  QxrdAcquisitionWPtr m_Acquisition;
};

#endif // QXRDDETECTOR_H
