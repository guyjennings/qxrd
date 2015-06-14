#include "qcepdataobject.h"
#include <QScriptEngine>
#include "qcepdatagroup.h"
#include <stdio.h>

QcepDataObject::QcepDataObject(QcepSettingsSaverWPtr saver, QString name) :
  QcepObject(name, NULL),
  m_Saver(saver),
  m_Type(saver, this, "type", "object", "Data object type"),
  m_Description(saver, this, "description", "", "Data object description")
{
  set_Type("object");
}

QcepDataObjectPtr QcepDataObject::newDataObject(QcepSettingsSaverWPtr saver, QString name)
{
  QcepDataObjectPtr res(new QcepDataObject(saver, name));

  return res;
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
      QcepDataObjectPtr p = qdobj->sharedFromThis();

      if (p) {
        data = p;
      } else {
        printf("QcepDataObject::fromScriptValue returns NULL\n");
      }
    }
  }
}

int QcepDataObject::count() const
{
  return 0;
}

QcepDataObjectPtr QcepDataObject::item(int n) const
{
  return QcepDataObjectPtr();
}

QcepDataObjectPtr QcepDataObject::item(QString nm) const
{
  return QcepDataObjectPtr();
}

QcepDataObjectPtr QcepDataObject::parentItem() const
{
  return m_Parent;
}

void QcepDataObject::setParentItem(QcepDataObjectWPtr parent)
{
  m_Parent = parent;
}

int QcepDataObject::indexInParent() const
{
  QcepDataObjectPtr parent = m_Parent;

  if (parent) {
    QcepDataGroup *parentGroup =
          qobject_cast<QcepDataGroup*>(parent.data());

    if (parentGroup) {
      for (int i=0; i<parentGroup->count(); i++) {
        if (parentGroup->item(i).data() == this) {
          return i;
        }
      }
    }
  }

  return 0;
}

int QcepDataObject::columnCount() const
{
  return 3;
}

QVariant QcepDataObject::columnData(int col) const
{
  if (col == 0) {
    return get_Name();
  } else if (col == 1) {
    return get_Type();
  } else {
    return get_Description();
  }
}

QString QcepDataObject::metaTypeName(int id)
{
  return QMetaType::typeName(id);
}
