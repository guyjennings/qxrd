#include "qcepdatagroup.h"
#include <QScriptEngine>
#include "qcepdataarray.h"
#include "qcepdataarray-ptr.h"
#include "qcepdatagroup.h"
#include "qcepdatagroup-ptr.h"
#include "qcepdatacolumn.h"
#include "qcepdatacolumn-ptr.h"
#include "qcepdatacolumnscan.h"
#include "qcepdatacolumnscan-ptr.h"
#include <QFileInfo>
#include <QDir>

QcepDataGroup::QcepDataGroup(QcepSettingsSaverWPtr saver, QString name) :
  QcepDataObject(saver, name)
{
  set_Type("Data Group");
}

QString QcepDataGroup::description() const
{
  return tr("%1 Items").arg(count());
}

QcepDataGroupPtr QcepDataGroup::newDataGroup(QcepSettingsSaverWPtr saver, QString name)
{
  QcepDataGroupPtr res(new QcepDataGroup(saver, name));

  return res;
}

QcepDataObjectPtr QcepDataGroup::item(int n)
{
  return m_Objects.value(n);
}

QcepDataObjectPtr QcepDataGroup::item(QString nm)
{
  QFileInfo info(nm);

  if (info.isRoot()) {
    return rootItem();
  } else if (info.fileName() == nm) {
    foreach(QcepDataObjectPtr p, m_Objects) {
      if (p && (p->get_Name() == nm)) {
        return p;
      }
    }
  } else {
    QDir dir = info.dir();

    QcepDataObjectPtr obj = item(dir.path());

    if (obj) {
      QcepDataGroupPtr grp = qSharedPointerCast<QcepDataGroup>(obj);

      if (grp) {
        return grp->item(info.fileName());
      }
    }
  }
  return QcepDataObjectPtr();
}

int QcepDataGroup::count() const
{
  return m_Objects.count();
}

QcepDataGroupPtr QcepDataGroup::containingGroup(QString path)
{
  QFileInfo info(path);

  if (info.isAbsolute()) {
    if (parentItem()) {
      return rootItem()->containingGroup(path);
    } else {
      QcepDataObjectPtr obj = referencedObject(path);

      return qSharedPointerCast<QcepDataGroup>(obj);
    }
  } else {
    QcepDataObjectPtr obj = item(path);

    return qSharedPointerCast<QcepDataGroup>(obj);
  }
}

QcepDataObjectPtr QcepDataGroup::referencedObject(QString path)
{
  QFileInfo info(path);

  if (info.isAbsolute()) {
    return rootItem()->item(path);
  } else {
    return item(path);
  }
}

void QcepDataGroup::append(QcepDataObjectPtr obj)
{
  if (obj) {
    m_Objects.append(obj);

    QcepDataGroupPtr me = qSharedPointerCast<QcepDataGroup>(sharedFromThis());

    if (me) {
      obj -> setParentItem(me);
    } else {
      printf("Can't cast to QcepDataGroupPtr");
    }

    connect(obj.data(), SIGNAL(dataObjectChanged()), this, SIGNAL(dataObjectChanged()));

    emit dataObjectChanged();
  }
}

void QcepDataGroup::append(QString path, QcepDataObjectPtr obj)
{
  QcepDataGroupPtr group = containingGroup(path);

  if (group) {
    group->append(obj);
  }
}

void QcepDataGroup::remove(QcepDataObjectPtr obj)
{
  int n = m_Objects.indexOf(obj);

  if (n >= 0) {
    m_Objects.remove(n);

    emit dataObjectChanged();
  }
}

void QcepDataGroup::remove(QString path)
{
  QcepDataObjectPtr obj = referencedObject(path);

  if (obj) {
    QcepDataGroupPtr parent = obj->parentItem();

    if (parent) {
      parent->remove(obj);
    }
  }
}

void QcepDataGroup::addGroup(QString path)
{
  QcepDataGroupPtr group(new QcepDataGroup(saver(), path));

  append(group);

  emit dataObjectChanged();
}

void QcepDataGroup::addArray(QString path, QVector<int> dims)
{
  QcepDataArrayPtr array(new QcepDataArray(saver(), path, dims));

  append(array);

  emit dataObjectChanged();
}

void QcepDataGroup::addColumn(QString path, int nrows)
{
  QcepDataColumnPtr column(new QcepDataColumn(saver(), path, nrows));

  append(column);

  emit dataObjectChanged();
}

void QcepDataGroup::addColumnScan(QString path, int nrow, QStringList cols)
{
  QcepDataColumnScanPtr scan(QcepDataColumnScan::newDataColumnScan(saver(), path, cols, nrow));

  append(scan);

  emit dataObjectChanged();
}

QcepDataGroupPtr QcepDataGroup::newGroup(QString path)
{
  QcepDataGroupPtr group(new QcepDataGroup(saver(), path));

  append(group);

  emit dataObjectChanged();

  return group;
}

QcepDataArrayPtr QcepDataGroup::newArray(QString path, QVector<int> dims)
{
  QcepDataArrayPtr array(new QcepDataArray(saver(), path, dims));

  append(array);

  emit dataObjectChanged();

  return array;
}

QcepDataColumnPtr QcepDataGroup::newColumn(QString path, int nrows)
{
  QcepDataColumnPtr column(new QcepDataColumn(saver(), path, nrows));

  append(column);

  emit dataObjectChanged();

  return column;
}

QcepDataColumnScanPtr QcepDataGroup::newColumnScan(QString path, int nrow, QStringList cols)
{
  QcepDataColumnScanPtr scan(QcepDataColumnScan::newDataColumnScan(saver(), path, cols, nrow));

  append(scan);

  emit dataObjectChanged();

  return scan;
}

QScriptValue QcepDataGroup::toGroupScriptValue(QScriptEngine *engine, const QcepDataGroupPtr &data)
{
  return engine->newQObject(data.data());
}

void QcepDataGroup::fromGroupScriptValue(const QScriptValue &obj, QcepDataGroupPtr &data)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepDataGroup *qdobj = qobject_cast<QcepDataGroup*>(qobj);

    if (qdobj) {
      QcepDataObjectPtr p = qdobj->sharedFromThis();

      if (p) {
        QcepDataGroupPtr cs = qSharedPointerCast<QcepDataGroup>(p);

        if (cs) {
          data = cs;
        }
      }
    }
  }
}

