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
#include "qcepmaskdata-ptr.h"

class QxrdROICalculator : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdROICalculator(QString name);
  QxrdROICalculator();
  virtual ~QxrdROICalculator();

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdROICalculatorPtr &proc);
  static void fromScriptValue(const QScriptValue &obj, QxrdROICalculatorPtr &proc);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:

public slots:
  int roiCount();
  QxrdROICoordinatesListModelWPtr roiModel();
  QxrdROICoordinatesWPtr roi(int i);

  QVector<double>           values(QcepImageDataBasePtr img, QcepMaskDataPtr mask);

private:
  QMutex                         m_Mutex;
  QxrdROICoordinatesListModelPtr m_ROICoordinatesModel;

public:
};

Q_DECLARE_METATYPE(QxrdROICalculator*)

#endif // QXRDROICALCULATOR_H
