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
#include "qxrdroicoordinateslist-ptr.h"
#include "qxrdroicoordinates-ptr.h"

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
  QxrdROICoordinatesListPtr coordinates();
  QxrdROICoordinatesPtr     coordinate(int i);

private:
  QMutex                    m_Mutex;
  QcepSettingsSaverWPtr     m_Saver;
  QxrdExperimentWPtr        m_Experiment;
  QxrdDetectorProcessorWPtr m_Processor;

  QxrdROICoordinatesListPtr m_ROICoordinates;

public:
};

#endif // QXRDROICALCULATOR_H
