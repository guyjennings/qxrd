#include "qcepdataobject.h"
#include <QScriptEngine>
#include "qcepdatagroup.h"

QcepDataObject::QcepDataObject(QcepSettingsSaverWPtr saver, QString name, QcepDataObjectWPtr parent) :
  QcepObject(name, NULL),
  m_Parent(parent),
  m_Saver(saver),
  m_Type(saver, this, "type", "object", "Data object type")
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

int QcepDataObject::indexInParent() const
{
  QcepDataObjectPtr parent = m_Parent;

  if (parent) {
    QcepDataGroupPtr parentGroup = QcepDataGroupPtr(
          qobject_cast<QcepDataGroup*>(parent.data()));

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
  return 2;
}

QVariant QcepDataObject::columnData(int col) const
{
  if (col == 0) {
    return get_Name();
  } else if (col == 1) {
    return get_Type();
  } else {
    return QVariant();
  }
}
