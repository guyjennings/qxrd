#include "qcepdataobject.h"
#include <QScriptEngine>
#include "qcepdatagroup.h"
#include <stdio.h>
#include "qcepapplication.h"
#include <QAtomicInteger>

static QAtomicInt s_ObjectAllocateCount(0);
static QAtomicInt s_ObjectDeleteCount(0);

QcepDataObject::QcepDataObject(QcepSettingsSaverWPtr saver, QString name) :
  QcepObject(name, NULL),
  m_Saver(saver),
  m_Type(saver,        this, "type", "object", "Data object type"),
  m_Creator(saver,     this, "creator", "Unknown", "QXRD Version Number"),
  m_Version(saver,     this, "version", "Unknown", "QXRD Version Number"),
  m_QtVersion(saver,   this, "qtVersion", QT_VERSION_STR, "QT Version Number"),
  m_Description(saver, this, "description", "", "Object Description")
{
  s_ObjectAllocateCount.fetchAndAddOrdered(1);

  set_Type("object");

  if (name.contains("/")) {
    printMessage(tr("object %1 name contains \"/\"").arg(name));
  }

  if (g_Application) {
    g_Application->setDefaultObjectData(this);
  }
}

QcepDataObject::~QcepDataObject()
{
  s_ObjectDeleteCount.fetchAndAddOrdered(1);
}

int QcepDataObject::allocatedObjects()
{
  return s_ObjectAllocateCount.load();
}

int QcepDataObject::deletedObjects()
{
  return s_ObjectDeleteCount.load();
}

QString QcepDataObject::description() const
{
  return "";
}

QString QcepDataObject::pathName() const
{
  QString suffix="";

  if (qobject_cast<const QcepDataGroup*>(this)) {
    suffix = "/";
  }

  if (parentItem()) {
    return parentItem()->pathName()+get_Name()+suffix;
  } else {
    return "/";
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

int QcepDataObject::childCount() const
{
  return 0;
}

int QcepDataObject::rowCount() const
{
  return 0;
}

int QcepDataObject::columnCount() const
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

    return qSharedPointerDynamicCast<QcepDataGroup>(obj);
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
      for (int i=0; i<parentGroup->childCount(); i++) {
        if (parentGroup->item(i).data() == this) {
          return i;
        }
      }
    }
  }

  return 0;
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
