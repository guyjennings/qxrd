#ifndef QXRDROICALCULATOR_H
#define QXRDROICALCULATOR_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrddetectorprocessor-ptr.h"
#include "qxrdexperiment-ptr.h"
#include <QScriptEngine>
#include "qxrdroicalculator-ptr.h"

class QxrdROICalculator : public QcepObject, public QEnableSharedFromThis<QxrdROICalculator>
{
  Q_OBJECT

public:
  QxrdROICalculator(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp, QxrdDetectorProcessorWPtr proc);
  virtual ~QxrdROICalculator();

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdROICalculatorPtr &proc);
  static void fromScriptValue(const QScriptValue &obj, QxrdROICalculatorPtr &proc);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:

public slots:

private:
  QMutex                    m_Mutex;
  QcepSettingsSaverWPtr     m_Saver;
  QxrdExperimentWPtr        m_Experiment;
  QxrdDetectorProcessorWPtr m_Processor;

public:
  Q_PROPERTY(int roiCount READ get_RoiCount WRITE set_RoiCount)
  QCEP_INTEGER_PROPERTY(RoiCount)
};

#endif // QXRDROICALCULATOR_H
