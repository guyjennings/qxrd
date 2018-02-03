#ifndef QXRDROICALCULATOR_H
#define QXRDROICALCULATOR_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include "qxrdprocessor-ptr.h"
#include "qxrdexperiment-ptr.h"
#include <QScriptEngine>
#include "qxrdroicalculator-ptr.h"
#include "qxrdroimodel-ptr.h"
#include "qxrdroi-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"

class QXRD_EXPORT QxrdROICalculator : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdROICalculator(QString name);
  virtual ~QxrdROICalculator();
  void initialize(QObjectWPtr parent);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdROICalculatorPtr &proc);
  static void fromScriptValue(const QScriptValue &obj, QxrdROICalculatorPtr &proc);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

signals:

public slots:
  int roiCount();
  QxrdROIModelWPtr roiModel();
  QxrdROIWPtr roi(int i);

  QVector<double>           values(QcepImageDataBasePtr img, QcepMaskDataPtr mask);

private:
  QMutex           m_Mutex;
  QxrdROIModelPtr  m_ROIModel;

public:
};

Q_DECLARE_METATYPE(QxrdROICalculator*)

#endif // QXRDROICALCULATOR_H
