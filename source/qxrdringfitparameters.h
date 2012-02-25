#ifndef QXRDRINGFITPARAMETERS_H
#define QXRDRINGFITPARAMETERS_H

#include "qxrdfitparameter.h"
#include <QScriptEngine>
#include "qxrdsettingssaver.h"

class QxrdRingSetFitParameters;

class QxrdRingFitParameters : public QObject
{
  Q_OBJECT
public:
  explicit QxrdRingFitParameters(QxrdSettingsSaverPtr saver, QxrdRingSetFitParameters *ringSet, double twoTheta=0);

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

Q_DECLARE_METATYPE(QxrdRingFitParameters*)

QScriptValue QxrdRingFitToScriptValue(QScriptEngine *engine, QxrdRingFitParameters* const &in);
void QxrdRingFitFromScriptValue(const QScriptValue &object, QxrdRingFitParameters *&out);

typedef QSharedPointer<QxrdRingFitParameters> QxrdRingFitParametersPtr;

#endif // QXRDRINGFITPARAMETERS_H
