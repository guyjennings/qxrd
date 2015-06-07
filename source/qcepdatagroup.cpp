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

QcepDataGroup::QcepDataGroup(QcepSettingsSaverWPtr saver, QString name, QcepDataObjectWPtr parent) :
  QcepDataObject(saver, name, parent)
{
  set_Type("Data Group");
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
  QcepDataColumnScanPtr scan(new QcepDataColumnScan(saver(), path, cols, nrow));

  append(scan);

  emit dataObjectChanged();
}
