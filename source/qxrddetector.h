#ifndef QXRDDETECTOR_H
#define QXRDDETECTOR_H

#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepobject.h"
#include <QDoubleSpinBox>
#include <QComboBox>

#include "qxrdacquisition-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrddetector-ptr.h"
#include "qxrddetectorproxy-ptr.h"
#include <QScriptEngine>

class QxrdDetector : public QcepObject
{
  Q_OBJECT

public:
  explicit QxrdDetector(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detType, QcepObject *parent);
  virtual ~QxrdDetector();

  virtual int detectorType() const;
  virtual QString detectorTypeName() const;
  virtual int detectorSubType() const;
  virtual QString detectorAddress() const;

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdDetectorPtr &det);
  static void fromScriptValue(const QScriptValue &obj, QxrdDetectorPtr &det);

  virtual void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  virtual void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);

  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

signals:

public slots:
  virtual void initialize();

  virtual void onExposureTimeChanged();
  virtual void onBinningModeChanged();
  virtual void onCameraGainChanged();

  virtual void setupExposureMenu(QDoubleSpinBox *cb, double initialExposure);
  virtual void setupCameraGainMenu(QComboBox *cb, int initialGain);
  virtual void setupCameraBinningModeMenu(QComboBox *cb, int initialBinning);

  virtual void acquire();
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

  Q_PROPERTY(QString detectorTypeName READ get_DetectorTypeName WRITE set_DetectorTypeName STORED false)
  QCEP_STRING_PROPERTY(DetectorTypeName)
};

#endif // QXRDDETECTOR_H
