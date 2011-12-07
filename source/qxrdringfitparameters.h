#ifndef QXRDRINGFITPARAMETERS_H
#define QXRDRINGFITPARAMETERS_H

#include "qxrdfitparameter.h"
#include <QScriptEngine>

class QxrdRingSetFitParameters;

class QxrdRingFitParameters : public QObject
{
  Q_OBJECT
public:
  explicit QxrdRingFitParameters(QxrdSettingsSaver *saver, QxrdRingSetFitParameters *ringSet = 0, double twoTheta=0);

public:
  Q_PROPERTY(double twoTheta READ get_TwoTheta WRITE set_TwoTheta)
  QXRD_FIT_PARAMETER(TwoTheta)

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

private:
  mutable QMutex         m_Mutex;

  QxrdRingSetFitParameters *m_RingSet;
};

typedef QSharedPointer<QxrdRingFitParameters> QxrdRingFitParametersPtr;
//typedef QxrdRingFitParameters *QxrdRingFitParametersPtr;

Q_DECLARE_METATYPE(QxrdRingFitParameters*)

QScriptValue QxrdRingFitToScriptValue(QScriptEngine *engine, QxrdRingFitParameters* const &in);
void QxrdRingFitFromScriptValue(const QScriptValue &object, QxrdRingFitParameters* &out);

#endif // QXRDRINGFITPARAMETERS_H
