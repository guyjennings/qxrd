#include "qcepdatagroup.h"
#include <QScriptEngine>

QcepDataGroup::QcepDataGroup(QString name, QObject *parent) :
  QcepDataObject(name, parent)
{

}

QcepDataObjectPtr QcepDataGroup::item(int n)
{
  return m_Objects.value(n);
}

QcepDataObjectPtr QcepDataGroup::item(QString nm)
{
  foreach(QcepDataObjectPtr p, m_Objects) {
    if (p && (p->get_Name() == nm)) {
      return p;
    }
  }

  return QcepDataObjectPtr();
}

int QcepDataGroup::nitems()
{
  return m_Objects.count();
}

void QcepDataGroup::append(QcepDataObjectPtr obj)
{
  if (obj) {
    m_Objects.append(obj);
  }
}

void QcepDataGroup::remove(QcepDataObjectPtr obj)
{
  int n = m_Objects.indexOf(obj);

  if (n >= 0) {
    m_Objects.remove(n);
  }
}

QScriptValue QcepDataGroup::toScriptValue(QScriptEngine *engine, const QcepDataGroupPtr &data)
{
  return engine->newQObject(data.data());
}

void QcepDataGroup::fromScriptValue(const QScriptValue &obj, QcepDataGroupPtr &data)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepDataGroup *qdobj = qobject_cast<QcepDataGroup*>(qobj);

    if (qdobj) {
      data = QcepDataGroupPtr(qdobj);
    }
  }
}
