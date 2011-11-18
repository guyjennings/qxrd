#include "qxrdringsampleddata.h"
#include <QVector>
#include "qxrdmutexlocker.h"
#include "qcepproperty.h"

QxrdRingSampledData::QxrdRingSampledData(QObject *parent) :
    QObject(parent)
{
}

QScriptValue QxrdRingSampledDataToScriptValue(QScriptEngine *engine, QxrdRingSampledData* const &in)
{
  return engine->newQObject(in);
}

void QxrdRingSampledDataFromScriptValue(const QScriptValue &object, QxrdRingSampledData* &out)
{
  out = qobject_cast<QxrdRingSampledData*>(object.toQObject());
}

void QxrdRingSampledData::writeSettings(QSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

  settings.beginWriteArray(section+"/rings", count());

  for (int i=0; i<count(); i++) {
    QwtDoublePoint pt = point(i);

    settings.setArrayIndex(i);
    settings.setValue("x", pt.x());
    settings.setValue("y", pt.y());
  }

  settings.endArray();
}

void QxrdRingSampledData::readSettings(QSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  clear();

  int sz = settings.beginReadArray(section+"/rings");

  for (int i=0; i<sz; i++) {
    settings.setArrayIndex(i);

    QwtDoublePoint pt(settings.value("x").toDouble(), settings.value("y").toDouble());

    append(pt);
  }

  settings.endArray();
}

void QxrdRingSampledData::append(QwtDoublePoint pt)
{
  m_Points.append(pt);
}

void QxrdRingSampledData::remove(int n)
{
  m_Points.remove(n);
}

void QxrdRingSampledData::clear()
{
  m_Points.clear();
}

void QxrdRingSampledData::setPoint(int n, QwtDoublePoint pt)
{
  m_Points.replace(n, pt);
}

int  QxrdRingSampledData::count() const
{
  return m_Points.count();
}

QwtDoublePoint QxrdRingSampledData::point(int n) const
{
  return m_Points.value(n);
}
