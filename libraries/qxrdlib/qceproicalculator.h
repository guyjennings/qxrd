#ifndef QCEPROICALCULATOR_H
#define QCEPROICALCULATOR_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include <QScriptEngine>
#include "qceproicalculator-ptr.h"
#include "qceproimodel-ptr.h"
#include "qceproivector-ptr.h"
#include "qceproi-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"

class QCEP_EXPORT QcepROICalculator : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QcepROICalculator(QString name,
                                QcepROIVectorWPtr rois,
                                QcepROIModelWPtr model);
  virtual ~QcepROICalculator();
  void initialize(QcepObjectWPtr parent);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepROICalculatorPtr &proc);
  static void fromScriptValue(const QScriptValue &obj, QcepROICalculatorPtr &proc);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

signals:

public slots:
  int roiCount();
  QcepROIModelWPtr roiModel();
  QcepROIWPtr roi(int i);

  QVector<double>           values(QcepImageDataBasePtr img, QcepMaskDataPtr mask);

private:
  QcepROIVectorWPtr m_ROIVector;
  QcepROIModelWPtr  m_ROIModel;

public:
};

Q_DECLARE_METATYPE(QcepROICalculator*)

#endif // QCEPROICALCULATOR_H
