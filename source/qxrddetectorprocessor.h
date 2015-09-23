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
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
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

  void processAcquiredImage(QString filePattern,
                            int fileIndex,
                            int phase,
                            int nPhases,
                            bool trig,
                            QcepInt32ImageDataPtr image,
                            QcepMaskDataPtr overflow);

  void processDarkImage(QString filePattern,
                        int fileIndex,
                        QcepInt32ImageDataPtr image,
                        QcepMaskDataPtr overflow);

  void processIdleImage(QcepImageDataBasePtr image);

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdDetectorProcessorPtr &proc);
  static void fromScriptValue(const QScriptValue &obj, QxrdDetectorProcessorPtr &proc);

private:
  QcepSettingsSaverWPtr m_Saver;

public:
  // Properties...

  Q_PROPERTY(bool performDarkSubtraction READ get_PerformDarkSubtraction WRITE set_PerformDarkSubtraction)
  QCEP_BOOLEAN_PROPERTY(PerformDarkSubtraction)

  Q_PROPERTY(bool saveRawImages READ get_SaveRawImages WRITE set_SaveRawImages)
  QCEP_BOOLEAN_PROPERTY(SaveRawImages)

  Q_PROPERTY(bool saveDarkImages READ get_SaveDarkImages WRITE set_SaveDarkImages)
  QCEP_BOOLEAN_PROPERTY(SaveDarkImages)

  Q_PROPERTY(bool performBadPixels READ get_PerformBadPixels WRITE set_PerformBadPixels)
  QCEP_BOOLEAN_PROPERTY(PerformBadPixels)

  Q_PROPERTY(bool performGainCorrection READ get_PerformGainCorrection WRITE set_PerformGainCorrection)
  QCEP_BOOLEAN_PROPERTY(PerformGainCorrection)

  Q_PROPERTY(bool saveSubtracted READ get_SaveSubtracted WRITE set_SaveSubtracted)
  QCEP_BOOLEAN_PROPERTY(SaveSubtracted)

  Q_PROPERTY(bool performIntegration READ get_PerformIntegration WRITE set_PerformIntegration)
  QCEP_BOOLEAN_PROPERTY(PerformIntegration)

  Q_PROPERTY(bool displayIntegratedData READ get_DisplayIntegratedData WRITE set_DisplayIntegratedData)
  QCEP_BOOLEAN_PROPERTY(DisplayIntegratedData)

  Q_PROPERTY(bool saveIntegratedData READ get_SaveIntegratedData WRITE set_SaveIntegratedData)
  QCEP_BOOLEAN_PROPERTY(SaveIntegratedData)

  Q_PROPERTY(bool accumulateIntegrated2D READ get_AccumulateIntegrated2D WRITE set_AccumulateIntegrated2D)
  QCEP_BOOLEAN_PROPERTY(AccumulateIntegrated2D)

  Q_PROPERTY(bool calculateROICounts READ get_CalculateROICounts WRITE set_CalculateROICounts)
  QCEP_BOOLEAN_PROPERTY(CalculateROICounts)

private:
  QMutex                m_Mutex;

  QxrdExperimentWPtr    m_Experiment;
  QxrdDetectorWPtr      m_Detector;

  QxrdCenterFinderPtr   m_CenterFinder;
  QxrdIntegratorPtr     m_Integrator;
  QxrdROICalculatorPtr  m_ROICalculator;
};

#endif // QXRDACQUISITIONPROCESSOR_H
