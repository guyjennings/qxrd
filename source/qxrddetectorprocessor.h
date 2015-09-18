#ifndef QXRDDETECTORPROCESSOR_H
#define QXRDDETECTORPROCESSOR_H

#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepobject.h"

#include "qcepsettingssaver-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddetector-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qcepallocator-ptr.h"
#include "qxrddetectorprocessor-ptr.h"
#include "qxrdcenterfinder-ptr.h"
#include "qxrdintegrator-ptr.h"
#include "qxrdroicalculator-ptr.h"

#include <QScriptEngine>

class QxrdDetectorProcessor : public QcepObject, public QEnableSharedFromThis<QxrdDetectorProcessor>
{
    Q_OBJECT

public:
  QxrdDetectorProcessor(QcepSettingsSaverWPtr saver,
                        QxrdExperimentWPtr    doc,
                        QxrdDetectorWPtr      det);
  virtual ~QxrdDetectorProcessor();
  void initialize();

signals:

public slots:
  QxrdCenterFinderPtr  centerFinder();
  QxrdIntegratorPtr    integrator();
  QxrdROICalculatorPtr roiCalculator();

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdDetectorProcessorPtr &proc);
  static void fromScriptValue(const QScriptValue &obj, QxrdDetectorProcessorPtr &proc);

private:
  QcepSettingsSaverWPtr m_Saver;

public:
  // Properties...

private:
  QMutex                m_Mutex;

  QxrdExperimentWPtr    m_Experiment;
  QxrdDetectorWPtr      m_Detector;

  QxrdCenterFinderPtr   m_CenterFinder;
  QxrdIntegratorPtr     m_Integrator;
  QxrdROICalculatorPtr  m_ROICalculator;
};

#endif // QXRDACQUISITIONPROCESSOR_H
