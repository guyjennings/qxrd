#include "qcepdataobject.h"
#include <QScriptEngine>
#include "qcepdatagroup.h"
#include <stdio.h>

QcepDataObject::QcepDataObject(QcepSettingsSaverWPtr saver, QString name) :
  QcepObject(name, NULL),
  m_Saver(saver),
  m_Type(saver, this, "type", "object", "Data object type")/*,
  m_Description(saver, this, "description", "", "Data object description")*/
{
  set_Type("object");
}

QString QcepDataObject::description() const
{
  return "";
}

QString QcepDataObject::pathName() const
{
  if (parentItem()) {
    return parentItem()->pathName()+"/"+get_Name();
  } else {
    return get_Name();
  }
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

QcepDataObjectPtr QcepDataObject::item(int n)
{
  return QcepDataObjectPtr();
}

QcepDataObjectPtr QcepDataObject::item(QString nm)
{
  return QcepDataObjectPtr();
}

QcepDataGroupPtr QcepDataObject::parentItem() const
{
  return m_Parent;
}

QcepDataGroupPtr QcepDataObject::rootItem()
{
  QcepDataGroupPtr parent(m_Parent);

  if (parent) {
    return parent->rootItem();
  } else {
    QcepDataObjectPtr obj = sharedFromThis();

    return qSharedPointerCast<QcepDataGroup>(obj);
  }
}

void QcepDataObject::setParentItem(QcepDataGroupWPtr parent)
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
    return QVariant(description());
  }
}

QString QcepDataObject::metaTypeName(int id) const
{
  return QMetaType::typeName(id);
}
