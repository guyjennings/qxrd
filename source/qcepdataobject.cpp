#include "qcepdataobject.h"
#include <QScriptEngine>

QcepDataObject::QcepDataObject(QcepSettingsSaverWPtr saver, QString name, QObject *parent) :
  QcepObject(name, parent),
  m_Type(saver, this, "type", "object", "Data object type"),
  m_Saver(saver)
{
  set_Type("object");
}

QcepSettingsSaverWPtr QcepDataObject::saver()
{
  return m_Saver;
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
