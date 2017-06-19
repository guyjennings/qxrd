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

void QcepDataGroup::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepDataObject::writeSettings(settings, "");

  if (settings) {
    settings->beginWriteArray("items");

    for (int i=0; i<m_Objects.count(); i++) {
      settings->setArrayIndex(i);

      QcepDataObjectPtr p = m_Objects.value(i);

      if (p) {
        p-> writeSettings(settings, "");
      }
    }

    settings->endArray();
  }
}

void QcepDataGroup::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QString id = get_Type();

  QcepDataObject::readSettings(settings, "");

#ifndef QT_NODEBUG
  if (get_Type() != id) {
    printMessage(tr("Data object type ID changed from %1 to %2 on reading").arg(id).arg(get_Type()));
  }
#endif

  if (settings) {
    int n = settings->beginReadArray("items");

    for (int i=0; i<n; i++) {
      settings->setArrayIndex(i);

      QString name = settings->value("name").toString();
      QString id = settings->value("type").toString();

      QcepDataObjectPtr obj = QcepAllocator::newDataObject(id, name);

      if (obj) {
        m_Objects.append(obj);
        obj -> readSettings(settings, "");
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
  return m_Objects.value(n);
}

QcepDataObjectPtr QcepDataGroup::item(QString nm)
{
  QFileInfo info(nm);

  if (info.isRoot()) {
    return rootItem();
  } else if (nm == ".") {
    return qSharedPointerDynamicCast<QcepDataGroup>(sharedFromThis());
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
      QcepDataGroupPtr grp = qSharedPointerDynamicCast<QcepDataGroup>(obj);

      if (grp) {
        return grp->item(info.fileName());
      }
    }
  }
  return QcepDataObjectPtr();
}

int QcepDataGroup::childCount() const
{
  return m_Objects.count();
}

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
  m_Objects.clear();

  emit dataObjectChanged();
}

void QcepDataGroup::insert(int atRow, QcepDataObjectPtr obj)
{
  if (obj) {
    if (atRow <= 0) {
      m_Objects.prepend(obj);
    } else if (atRow >= m_Objects.count()) {
      m_Objects.append(obj);
    } else {
      m_Objects.insert(atRow, obj);
    }

    obj->setParentItem(qSharedPointerDynamicCast<QcepDataGroup>(sharedFromThis()));
  }
}

void QcepDataGroup::append(QcepDataObjectPtr obj)
{
  if (obj) {
    m_Objects.append(obj);

    QcepDataGroupPtr me = qSharedPointerDynamicCast<QcepDataGroup>(sharedFromThis());

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

void QcepDataGroup::remove(int n)
{
  if (n >= 0 && n < m_Objects.count()) {
    m_Objects.remove(n);

    emit dataObjectChanged();
  }
}

void QcepDataGroup::remove(QcepDataObjectPtr obj)
{
  int n = m_Objects.indexOf(obj);

  remove(n);
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
            QcepAllocator::newGroup(object(path));

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

