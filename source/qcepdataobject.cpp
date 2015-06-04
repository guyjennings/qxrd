#include "qcepdataobject.h"
#include <QScriptEngine>

QcepDataObject::QcepDataObject(QString name, QObject *parent) :
  QcepObject(name, parent)
{

}

QScriptValue QcepDataObject::toScriptValue(QScriptEngine *engine, const QcepDataObjectPtr &data)
{
  return engine->newQObject(data.data());
}

void QcepDataObject::fromScriptValue(const QScriptValue &obj, QcepDataObjectPtr &data)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepDataObject *qdobj = qobject_cast<QcepDataObject*>(qobj);

    if (qdobj) {
      data = QcepDataObjectPtr(qdobj);
    }
  }
}
