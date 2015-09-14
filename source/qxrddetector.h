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
#include "qxrdacquisitionprocessor-ptr.h"
#include <QScriptEngine>

class QxrdDetector : public QcepObject, public QEnableSharedFromThis<QxrdDetector>
{
  Q_OBJECT

public:
  explicit QxrdDetector(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detType, QcepObject *parent);
  virtual ~QxrdDetector();

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdDetectorPtr &det);
  static void fromScriptValue(const QScriptValue &obj, QxrdDetectorPtr &det);

  virtual void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  virtual void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);

  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy, int detType);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:

public slots:
  virtual void initialize();

  virtual void onExposureTimeChanged();
  virtual void setupExposureMenu(QDoubleSpinBox *cb, double initialExposure);

  virtual void acquire();
  virtual void beginAcquisition();
  virtual void endAcquisition();
  virtual void shutdownAcquisition();

  QxrdAcquisitionProcessorPtr acquisitionProcessor();

protected:
  QcepSettingsSaverWPtr       m_Saver;
  QxrdExperimentWPtr          m_Experiment;
  QxrdAcquisitionWPtr         m_Acquisition;
  QxrdAcquisitionProcessorPtr m_AcquisitionProcessor;

private:
  QMutex                      m_Mutex;

public:
  Q_PROPERTY(int detectorType READ get_DetectorType WRITE set_DetectorType)
  QCEP_INTEGER_PROPERTY(DetectorType)

  Q_PROPERTY(QString detectorTypeName READ get_DetectorTypeName WRITE set_DetectorTypeName STORED false)
  QCEP_STRING_PROPERTY(DetectorTypeName)

  Q_PROPERTY(bool enabled READ get_Enabled WRITE set_Enabled)
  QCEP_BOOLEAN_PROPERTY(Enabled)

  Q_PROPERTY(QString detectorName READ get_DetectorName WRITE set_DetectorName)
  QCEP_STRING_PROPERTY(DetectorName)
};

#endif // QXRDDETECTOR_H
