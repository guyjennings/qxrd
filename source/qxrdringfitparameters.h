#ifndef QXRDRINGFITPARAMETERS_H
#define QXRDRINGFITPARAMETERS_H

#include <QScriptEngine>

#include "qxrdfitparameter.h"
#include "qxrdringsetfitparameters-ptr.h"

class QxrdRingFitParameters : public QObject
{
  Q_OBJECT
public:
  explicit QxrdRingFitParameters(QxrdSettingsSaverWPtr saver, QxrdRingSetFitParameters *ringSet, double twoTheta=0);

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

#endif // QXRDRINGFITPARAMETERS_H
