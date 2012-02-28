#include "qxrdringfitparameters.h"
#include "qxrdringsetfitparameters.h"
#include "qxrdmutexlocker.h"

QxrdRingFitParameters::QxrdRingFitParameters(QxrdSettingsSaverWPtr saver, QxrdRingSetFitParameters *ringSet, double twoTheta) :
    QObject(),
    m_TwoTheta(saver, this, "twoTheta", twoTheta),
    m_RingSet(ringSet)
{
}

QScriptValue QxrdRingFitToScriptValue(QScriptEngine *engine, QxrdRingFitParameters* const&in)
{
  return engine->newQObject(in);
}

void QxrdRingFitFromScriptValue(const QScriptValue &object, QxrdRingFitParameters* &out)
{
  out = qobject_cast<QxrdRingFitParameters*>(object.toQObject());
}

void QxrdRingFitParameters::writeSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  prop_TwoTheta()->writeSettings(settings, section);
}

void QxrdRingFitParameters::readSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  prop_TwoTheta()->readSettings(settings, section);
}
