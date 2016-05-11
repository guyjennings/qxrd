#include "qcepobject.h"
#include "qcepproperty.h"
#include <QMetaProperty>
#include <stdio.h>
#include <QAtomicInt>
#include <QSet>
#include <QThread>
#include "qcepfileformatter.h"

static QAtomicInt s_ObjectAllocateCount(0);
static QAtomicInt s_ObjectDeleteCount(0);

#ifndef QT_NO_DEBUG
static QSet<QcepObject*> s_Allocated;
#endif

QcepObject::QcepObject(QString name, QcepObjectWPtr parent) :
  QObject(NULL),
  m_Parent(parent),
  m_ObjectNamer(this, name)
{
  s_ObjectAllocateCount.fetchAndAddOrdered(1);

#ifndef QT_NO_DEBUG
  s_Allocated.insert(this);
#endif

  QcepObject *p = parent.data();

  if (p) {
    p->addChildPtr(this);
  }
}

QcepObject::~QcepObject()
{
#ifndef QT_NO_DEBUG
  QThread *currTh = QThread::currentThread();
  QThread *objTh  = thread();

  if (objTh && (currTh != objTh)) {
    printf("Deleting object from different thread obj:%s (curr:%s, objth:%s)\n",
           qPrintable(objectName()),
           qPrintable(currTh ? currTh->objectName() : "null"),
           qPrintable(objTh ? objTh->objectName() : "null"));
  }
#endif

  s_ObjectDeleteCount.fetchAndAddOrdered(1);

#ifndef QT_NO_DEBUG
  s_Allocated.remove(this);
#endif
}

void QcepObject::propertyChanged(QcepProperty *prop)
{
  if (prop == NULL || prop->isStored()) {
    m_ChangeCount.fetchAndAddOrdered(1);
    m_LastChanged.store(prop);

    QcepObjectPtr parent(m_Parent);

    if (parent) {
      parent -> propertyChanged(prop);
    }
  }
}

int QcepObject::isChanged() const
{
  return m_ChangeCount.load();
}

QString QcepObject::changedBy() const
{
  QcepProperty *p = m_LastChanged.load();

  if (p) {
    return p->parentName()+"."+p->name();
  } else {
    return "NULL";
  }
}

int QcepObject::childrenChanged() const
{
  if (isChanged()) {
    return true;
  } else {
    foreach(QcepObject* child, m_Children) {
      if(child) {
        int chg = child->childrenChanged();

        if (chg) return chg;
      }
    }
  }

  return 0;
}

QString QcepObject::childrenChangedBy() const
{
  if (isChanged()) {
    return changedBy();
  } else {
    foreach(QcepObject* child, m_Children) {
      if (child) {
        int chg = child->childrenChanged();

        if (chg) {
          return child->changedBy();
        }
      }
    }
  }

  return "NULL";
}

int QcepObject::checkChildren(int verbose, int level) const
{
  int ck = true;

  if (verbose) {
    const QMetaObject *meta = metaObject();

    printLine(tr("%1Checking %2 : %3 children : class %4")
              .arg("",level*2)
              .arg(get_Name())
              .arg(childCount())
              .arg(meta->className()));
  }

  foreach(QcepObject* child, m_Children) {
    if (child == NULL) {
      printLine(tr("NULL child of %1").arg(get_Name()));
      ck = false;
    } else {
      QcepObjectWPtr parent = child->parentPtr();

      if (parent != sharedFromThis()) {
        printLine(tr("parent of %1 is not %2")
                     .arg(child->get_Name())
                     .arg(get_Name()));
      }

      if (!child->checkChildren(verbose, level+1)) {
        ck = false;
      }
    }
  }

  return ck;
}

QcepObjectWPtr QcepObject::parentPtr() const
{
  return m_Parent;
}

void QcepObject::addChildPtr(QcepObject *child)
{
  m_Children.append(child);
}

int QcepObject::childCount() const
{
  return m_Children.count();
}

QcepObjectWPtr QcepObject::childPtr(int n) const
{
  QcepObject* p = m_Children.value(n);

  if (p) {
    return p->sharedFromThis();
  } else {
    return QcepObjectWPtr();
  }
}

QVector<QcepObjectWPtr> QcepObject::childrenPtr() const
{
  QVector<QcepObjectWPtr> res;

  foreach (QcepObject* child, m_Children) {
    if (child) {
      res.append(child->sharedFromThis());
    } else {
      res.append(QcepObjectWPtr());
    }
  }

  return res;
}

int QcepObject::allocatedObjects()
{
  return s_ObjectAllocateCount.load();
}

int QcepObject::deletedObjects()
{
  return s_ObjectDeleteCount.load();
}

#ifndef QT_NO_DEBUG
QSet<QcepObject*> QcepObject::allocatedObjectsSet()
{
  return s_Allocated;
}
#endif

void QcepObject::set_Name(QString name)
{
  setObjectName(name);
}

QString QcepObject::get_Name() const
{
  return objectName();
}

//void QcepObject::set_Type(QString name)
//{
//}

QString QcepObject::get_Type() const
{
  return metaObject()->className();
}

void QcepObject::printLine(QString line) const
{
  QcepObjectPtr parent(m_Parent);

  if (parent) {
    parent->printLine(line);
  }
}

void QcepObject::printMessage(QString msg, QDateTime dt) const
{
  QcepObjectPtr parent(m_Parent);

  if (parent) {
    parent->printMessage(msg, dt);
  } else {
    printf("MESSAGE: %s %s\n",
           qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
  }
}

void QcepObject::criticalMessage(QString msg, QDateTime dt) const
{
  QcepObjectPtr parent(m_Parent);

  if (parent) {
    parent->criticalMessage(msg, dt);
  } else {
    printf("MESSAGE: %s %s\n",
           qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
  }
}

void QcepObject::statusMessage(QString msg, QDateTime dt) const
{
  QcepObjectPtr parent(m_Parent);

  if (parent) {
    parent->statusMessage(msg, dt);
  } else {
    printf("MESSAGE: %s %s\n",
           qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
  }
}

void QcepObject::writeSettings(QSettings *set, QString section)
{
  if (set) {
    set->setValue("name", get_Name());
  }

  QcepProperty::writeSettings(this, set, section);

  m_ChangeCount.fetchAndStoreOrdered(0);
  m_LastChanged.store(NULL);
}

void QcepObject::readSettings(QSettings *set, QString section)
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "readObjectSettings", Qt::BlockingQueuedConnection, Q_ARG(QSettings*, set), Q_ARG(QString, section)));
  } else {
    readObjectSettings(set, section);
  }
}

void QcepObject::readObjectSettings(QSettings *set, QString section)
{
  QcepProperty::readSettings(this, set, section);
}

QString QcepObject::addSlashes(QString str)
{
  QString newStr;

  for(int i=0;i<str.length();i++) {
    if(str[i] == '\0') {
      newStr.append('\\');
      newStr.append('0');
    } else if(str[i] == '\'') {
      newStr.append('\\');
      newStr.append('\'');
    } else if(str[i] == '\"') {
      newStr.append('\\');
      newStr.append('\"');
    } else if(str[i] == '\\') {
      newStr.append('\\');
      newStr.append('\\');
    } else if (str[i] == '\n') {
      newStr.append("\\");
      newStr.append("n");
    } else {
      newStr.append(str[i]);
    }
  }

  return newStr;
}

QString QcepObject::scriptValueLiteral(QVariant v)
{
  if (v.type() == QMetaType::QString) {
    return "\"" + addSlashes(v.toString()) + "\"";
  } else {
    return v.toString();
  }
}

QString QcepObject::settingsScript()
{
  QString res = "";
  const QMetaObject* metaObject = this->metaObject();

  for (int i=1; i < metaObject->propertyCount(); i++) {
    QMetaProperty prop = metaObject->property(i);

    if (prop.isStored()) {
      res += tr("%1.%2 = %3;\n").arg(this->get_Name())
                                .arg(prop.name())
                                .arg(scriptValueLiteral(this->property(prop.name())));
    }
  }

  return res;
}

void QcepObject::dumpObjectTreePtr(int level)
{
  const QMetaObject* metaObject = this->metaObject();
  QcepObjectPtr parent(m_Parent);

  printLine(tr("%1// %2: %3 constrs, parent %4")
            .arg("", level)
            .arg(metaObject->className())
            .arg(metaObject->constructorCount())
            .arg(parent ? parent->get_Type() : "NULL"));

  int nDumped = 0;

  int nDumpedProperties = 0;

  for (int i=1; i < metaObject->propertyCount(); i++) {
    QMetaProperty prop = metaObject->property(i);

    if (prop.isStored()) {
      if (nDumped == 0) {
        printLine(tr("%1%2 {")
                  .arg("",level)
                  .arg(get_Type()));
      }

      if (nDumpedProperties == 0) {
        printLine(tr("%1properties{").arg("",level+1));
      }

      nDumped++;
      nDumpedProperties++;

      printLine(tr("%1%2 = %3")
                .arg("",level+2)
                .arg(prop.name())
                .arg(scriptValueLiteral(this->property(prop.name()))));
    }
  }

  if (nDumped > 0) {
    printLine(tr("%1}").arg("",level+1));
  }

  int nDumpedChildren = 0;

  for (int i=0; i<m_Children.count(); i++) {
    QcepObject *obj = m_Children.value(i);
    if (obj) {
      if (nDumped == 0) {
        printLine(tr("%1%2 {")
                  .arg("",level)
                  .arg(get_Type()));
      }

      if (nDumpedChildren == 0) {
        printLine(tr("%1children{").arg("",level+1));
      }

      nDumped++;
      nDumpedChildren++;

      obj->dumpObjectTreePtr(level+2);
    }
  }

  if (nDumpedChildren > 0) {
    printLine(tr("%1}").arg("",level+1));
  }

  if (nDumped > 0) {
    printLine(tr("%1}").arg("",level));
  } else {
    printLine(tr("%1// %2").arg("",level).arg(metaObject->className()));
  }
}

QcepObjectPtr QcepObject::readDataObject(QcepFileFormatterPtr fmt)
{
  return QcepObjectPtr();
}

void QcepObject::writeObject(QcepFileFormatterPtr fmt)
{
  const QMetaObject* metaObject = this->metaObject();
//  QcepObjectPtr parent(m_Parent);

  fmt->beginWriteObject(get_Name(), metaObject->className());

  int count = metaObject->propertyCount();
  int offset = QObject::staticMetaObject.propertyCount();

  fmt->beginWriteProperties();

  for (int i=offset; i<count; i++) {
    QMetaProperty metaProperty = metaObject->property(i);

    if (metaProperty.isStored()) {
      const char *name = metaProperty.name();
      QVariant value   = property(name);

      fmt->writeProperty(name, value);
    }
  }

  foreach (QByteArray name, dynamicPropertyNames()) {
    fmt->writeProperty(name.data(), property(name.data()));
  }

  fmt->endWriteProperties();

  fmt->beginWriteChildren();

  for (int i=0; i<m_Children.count(); i++) {
    QcepObject *obj = m_Children.value(i);

    if (obj) {
      obj->writeObject(fmt);
    }
  }

  fmt->endWriteChildren();

  fmt->beginWriteData();
  writeObjectData(fmt);
  fmt->endWriteData();

  fmt->endWriteObject();
}

void QcepObject::writeObjectData(QcepFileFormatterPtr fmt)
{
}

void QcepObject::readObjectData(QcepFileFormatterPtr fmt)
{
}
