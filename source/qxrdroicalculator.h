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
#include "qxrdroicoordinateslistmodel-ptr.h"
#include "qxrdroicoordinates-ptr.h"
#include "qcepimagedata-ptr.h"

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
  int roiCount();
  QxrdROICoordinatesListModelPtr roiModel();
  QxrdROICoordinatesPtr roi(int i);

  QVector<double>           values(QcepImageDataBasePtr img);
  double                    value(QcepImageDataBasePtr img, int i);

private:
  QMutex                         m_Mutex;
  QcepSettingsSaverWPtr          m_Saver;
  QxrdExperimentWPtr             m_Experiment;
  QxrdDetectorProcessorWPtr      m_Processor;

  QxrdROICoordinatesListModelPtr m_ROICoordinatesModel;

public:
};

#endif // QXRDROICALCULATOR_H
