#include "qcepdatagroup.h"
#include <QScriptEngine>
#include "qcepdataarray.h"
#include "qcepdatagroup.h"
#include "qcepdatacolumn.h"
#include "qcepdatacolumnscan.h"
#include "qcepimagedata.h"
#include "qcepintegrateddata.h"
#include "qcepallocator.h"
#include "qcepmutexlocker.h"
#include <QFileInfo>
#include <QDir>

QcepDataGroup::QcepDataGroup(QString name) :
  QcepDataObject(name, 0)
{
}

void QcepDataGroup::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

void QcepDataGroup::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

  if (settings) {
    settings->beginWriteArray("items");

    int n = childCount();

    for (int i=0; i<n; i++) {
      settings->setArrayIndex(i);

      QcepDataObjectPtr p =
          qSharedPointerDynamicCast<QcepDataObject>(childPtr(i));

      if (p) {
        p-> writeSettings(settings);
      }
    }

    settings->endArray();
  }
}

void QcepDataGroup::readSettings(QSettings *settings)
{
  QString id = get_Type();

  inherited::readSettings(settings);

#ifndef QT_NO_DEBUG
  if (get_Type() != id) {
    printMessage(tr("Data object type ID changed from %1 to %2 on reading").arg(id).arg(get_Type()));
  }
#endif

  if (settings) {
    int n = settings->beginReadArray("items");

    for (int i=0; i<n; i++) {
      settings->setArrayIndex(i);

      QcepObjectPtr obj = QcepObject::readObject(sharedFromThis(), settings);

      if (obj) {
        QcepDataObjectPtr dobj =
            qSharedPointerDynamicCast<QcepDataObject>(obj);

        if (dobj) {
          append(dobj);
        }
      }
    }

    settings->endArray();
  }
}

QString QcepDataGroup::description() const
{
  return tr("%1 Items").arg(childCount());
}

QcepDataObjectPtr QcepDataGroup::item(int n)
{
//  QcepObjectPtr      ch  = childPtr(n);
  QcepDataObjectPtr  res = m_Objects.value(n);
  return res;
}

QcepDataObjectPtr QcepDataGroup::item(QString nm)
{
  QFileInfo info(nm);

  if (info.isRoot()) {
    return rootItem();
  } else if (nm == ".") {
    return qSharedPointerDynamicCast<QcepDataGroup>(sharedFromThis());
  } else if (info.fileName() == nm) {
    int n = childCount();
    for(int i=0; i<n; i++) {
      QcepDataObjectPtr p = item(i);

      if (p && (p->get_Name() == nm)) {
        return p;
      }
    }
  } else {
    QDir dir = info.dir();

    QcepDataObjectPtr obj = item(dir.path());

    if (obj) {
      QcepDataGroupPtr grp = qSharedPointerDynamicCast<QcepDataGroup>(obj);

      if (grp) {
        return grp->item(info.fileName());
      }
    }
  }
  return QcepDataObjectPtr();
}

//int QcepDataGroup::childCount() const
//{
//  return m_Objects.count();
//}

QcepDataGroupPtr  QcepDataGroup::group(QString path)
{
  QcepDataObjectPtr obj = item(path);

  return qSharedPointerDynamicCast<QcepDataGroup>(obj);
}

QcepDataGroupPtr  QcepDataGroup::group(int n)
{
  QcepDataObjectPtr obj = item(n);

  return qSharedPointerDynamicCast<QcepDataGroup>(obj);
}

QcepDataArrayPtr  QcepDataGroup::array(QString path)
{
  QcepDataObjectPtr obj = item(path);

  return qSharedPointerDynamicCast<QcepDataArray>(obj);
}

QcepDataArrayPtr  QcepDataGroup::array(int n)
{
  QcepDataObjectPtr obj = item(n);

  return qSharedPointerDynamicCast<QcepDataArray>(obj);
}

QcepDataColumnPtr QcepDataGroup::column(QString path)
{
  QcepDataObjectPtr obj = item(path);

  return qSharedPointerDynamicCast<QcepDataColumn>(obj);
}

QcepDataColumnPtr QcepDataGroup::column(int n)
{
  QcepDataObjectPtr obj = item(n);

  return qSharedPointerDynamicCast<QcepDataColumn>(obj);
}

QcepDataColumnScanPtr QcepDataGroup::columnScan(QString path)
{
  QcepDataObjectPtr obj = item(path);

  return qSharedPointerDynamicCast<QcepDataColumnScan>(obj);
}

QcepDataColumnScanPtr QcepDataGroup::columnScan(int n)
{
  QcepDataObjectPtr obj = item(n);

  return qSharedPointerDynamicCast<QcepDataColumnScan>(obj);
}

QcepDoubleImageDataPtr QcepDataGroup::image(QString path)
{
  QcepDataObjectPtr obj = item(path);

  return qSharedPointerDynamicCast<QcepDoubleImageData>(obj);
}

QcepDoubleImageDataPtr QcepDataGroup::image(int n)
{
  QcepDataObjectPtr obj = item(n);

  return qSharedPointerDynamicCast<QcepDoubleImageData>(obj);
}

QcepIntegratedDataPtr QcepDataGroup::integratedData(QString path)
{
  QcepDataObjectPtr obj = item(path);

  return qSharedPointerDynamicCast<QcepIntegratedData>(obj);
}

QcepIntegratedDataPtr QcepDataGroup::integratedData(int n)
{
  QcepDataObjectPtr obj = item(n);

  return qSharedPointerDynamicCast<QcepIntegratedData>(obj);
}

QString QcepDataGroup::directoryName(QString path)
{
  QFileInfo info(path);

  if (info.isAbsolute()) {
    return info.dir().absolutePath();
  } else {
    return info.dir().path();
  }
}

QString QcepDataGroup::object(QString path)
{
  QFileInfo info(path);

  return info.fileName();
}

QcepDataGroupPtr QcepDataGroup::containingGroup(QString path)
{
  QFileInfo info(path);

  if (info.isAbsolute()) {
    if (parentItem()) {
      return rootItem()->containingGroup(path);
    } else {
      QcepDataObjectPtr obj = referencedObject(path);

      return qSharedPointerDynamicCast<QcepDataGroup>(obj);
    }
  } else {
    QcepDataObjectPtr obj = item(path);

    return qSharedPointerDynamicCast<QcepDataGroup>(obj);
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

void QcepDataGroup::clear()
{
  clearChildren();

  m_Objects.clear();

  emit dataObjectChanged();
}

void QcepDataGroup::insert(int atRow, QcepDataObjectPtr obj)
{
  if (obj) {
    if (atRow <= 0) {
      prependChildPtr(obj);
      m_Objects.insert(0, obj);
    } else if (atRow >= childCount()) {
      addChildPtr(obj);
      m_Objects.append(obj);
    } else {
      insertChildPtr(atRow, obj);
      m_Objects.insert(atRow+1, obj);
    }

    obj->setParentItem(qSharedPointerDynamicCast<QcepDataGroup>(sharedFromThis()));
  }
}

void QcepDataGroup::append(QcepDataObjectPtr obj)
{
  if (obj) {
//    addChildPtr(obj);

    m_Objects.append(obj);

    connect(obj.data(), &QcepDataObject::dataObjectChanged, this, &QcepDataObject::dataObjectChanged);

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

void QcepDataGroup::remove(int n)
{
  if (n >= 0 && n < childCount()) {
    removeChildPtr(childPtr(n));

    m_Objects.remove(n);

    emit dataObjectChanged();
  }
}

void QcepDataGroup::remove(QcepDataObjectPtr obj)
{
  removeChildPtr(obj);

  m_Objects.removeOne(obj);
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

QcepDataGroupPtr QcepDataGroup::createGroup(QString path)
{
  QcepDataGroupPtr gr = group(path);

  if (gr) {
    return gr;
  } else {
    QcepDataObjectPtr obj = item(path);

    if (obj) {
      printMessage(tr("Item %1 exists and is not a data group").arg(path));
    } else {
      QcepDataGroupPtr sgr = createGroup(directoryName(path));

      if (sgr) {
        QcepDataGroupPtr ng =
            QcepAllocator::newGroup(sharedFromThis(),
                                    object(path));

        if (ng) {
          sgr->append(ng);

          return group(path);
        }
      }
    }
  }

  return QcepDataGroupPtr();
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
      QcepObjectPtr p = qdobj->sharedFromThis();

      if (p) {
        QcepDataGroupPtr cs = qSharedPointerDynamicCast<QcepDataGroup>(p);

        if (cs) {
          data = cs;
        }
      }
    }
  }
}

