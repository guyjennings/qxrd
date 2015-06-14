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

QcepDataGroup::QcepDataGroup(QcepSettingsSaverWPtr saver, QString name) :
  QcepDataObject(saver, name)
{
  set_Type("Data Group");
}

QcepDataGroupPtr QcepDataGroup::newDataGroup(QcepSettingsSaverWPtr saver, QString name)
{
  QcepDataGroupPtr res(new QcepDataGroup(saver, name));

  return res;
}

QcepDataObjectPtr QcepDataGroup::item(int n) const
{
  return m_Objects.value(n);
}

QcepDataObjectPtr QcepDataGroup::item(QString nm) const
{
  foreach(QcepDataObjectPtr p, m_Objects) {
    if (p && (p->get_Name() == nm)) {
      return p;
    }
  }

  return QcepDataObjectPtr();
}

int QcepDataGroup::count() const
{
  return m_Objects.count();
}

void QcepDataGroup::append(QcepDataObjectPtr obj)
{
  if (obj) {
    m_Objects.append(obj);

    obj -> setParentItem(sharedFromThis());

    connect(obj.data(), SIGNAL(dataObjectChanged()), this, SIGNAL(dataObjectChanged()));

    emit dataObjectChanged();
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

