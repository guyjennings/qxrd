#include "qcepobject.h"
#include "qcepproperty.h"
#include <QMetaProperty>
#include <stdio.h>
#include <QAtomicInt>
#include <QSet>
#include <QThread>
#include "qcepfileformatter.h"
#include <QScriptEngine>

static QAtomicInt s_ObjectAllocateCount(0);
static QAtomicInt s_ObjectDeleteCount(0);

#ifndef QT_NO_DEBUG
static QSet<QcepObject*> s_Allocated;
#endif

QcepObject::QcepObject(QString name) :
  QObject(NULL),
  m_ObjectNamer(this, name)
{
  s_ObjectAllocateCount.fetchAndAddOrdered(1);

#ifndef QT_NO_DEBUG
  s_Allocated.insert(this);
#endif

  QcepObjectPtr p(m_Parent);

  if (p) {
    p->addChildPtr(sharedFromThis());
  }
}

//QcepObject::QcepObject() :
//  QObject(NULL),
//  m_Parent(),
//  m_ObjectNamer(this, "object")
//{
//}

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
    foreach(QcepObjectPtr child, m_Children) {
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
    foreach(QcepObjectPtr child, m_Children) {
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

  foreach(QcepObjectPtr child, m_Children) {
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

void QcepObject::setParentPtr(QcepObjectWPtr parent)
{
  if (m_Parent != parent) {
    QcepObjectPtr oldParent(m_Parent);
    QcepObjectPtr newParent(parent);

    if (newParent) {
      m_Parent = newParent;
    } else {
      printMessage("Attempt to set parent to non-existing object");
    }

    if (oldParent) {
      oldParent->removeChildPtr(sharedFromThis());
    }

    if (newParent) {
      newParent->addChildPtr(sharedFromThis());
    }
  }
}

QcepObjectWPtr QcepObject::parentPtr() const
{
  return m_Parent;
}

void QcepObject::addChildPtr(QcepObjectPtr child)
{
  if (m_Children.contains(child)) {
    printMessage("Added same child more than once");
  } else {
    m_Children.append(child);
  }

  if (sharedFromThis()) {
    child->setParentPtr(sharedFromThis());
  } else {
    printMessage("Adding child when sharedFromThis() == NULL");
  }
}

void QcepObject::removeChildPtr(QcepObjectPtr child)
{
  if (m_Children.contains(child)) {
    m_Children.removeAll(child);
  } else {
    printMessage("Removing object which is not a child");
  }
}

template <typename T>
bool QcepObject::checkPointer(QcepObjectWPtr ptr, QSharedPointer<T> &field)
{
  QSharedPointer<T> fp = qSharedPointerDynamicCast<T>(ptr);

  if (fp) {
    field = fp;
    return true;
  } else {
    return false;
  }
}

template <typename T>
bool QcepObject::checkPointer(QcepObjectWPtr ptr, QWeakPointer<T> &field)
{
  QWeakPointer<T> fp = qSharedPointerDynamicCast<T>(ptr);

  if (fp) {
    field = fp;
    return true;
  } else {
    return false;
  }
}

int QcepObject::childCount() const
{
  return m_Children.count();
}

QcepObjectWPtr QcepObject::childPtr(int n) const
{
  QcepObjectPtr p = m_Children.value(n);

  if (p) {
    return p;
  } else {
    return QcepObjectWPtr();
  }
}

QVector<QcepObjectPtr> QcepObject::childrenPtr() const
{
//  QVector<QcepObjectWPtr> res;

//  foreach (QcepObject* child, m_Children) {
//    if (child) {
//      res.append(child->sharedFromThis());
//    } else {
//      res.append(QcepObjectWPtr());
//    }
//  }

  return m_Children;
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

QString QcepObject::settingsScript()
{
  QString res = "";
  const QMetaObject* metaObject = this->metaObject();

  for (int i=1; i < metaObject->propertyCount(); i++) {
    QMetaProperty prop = metaObject->property(i);

    if (prop.isStored()) {
      res += tr("%1.%2 = %3;\n").arg(this->get_Name())
                                .arg(prop.name())
                                .arg(toScriptLiteral(this->property(prop.name())));
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
                .arg(toScriptLiteral(this->property(prop.name()))));
    }
  }

  if (nDumped > 0) {
    printLine(tr("%1}").arg("",level+1));
  }

  int nDumpedChildren = 0;

  for (int i=0; i<m_Children.count(); i++) {
    QcepObjectPtr obj = m_Children.value(i);
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
  if (fmt) {
    return fmt->nextObject();
  } else {
    return QcepObjectPtr();
  }
}

void QcepObject::writeObject(QcepFileFormatterPtr fmt)
{
  const QMetaObject* metaObject = this->metaObject();
//  QcepObjectPtr parent(m_Parent);

  fmt->beginWriteObject(get_Name(), metaObject->className());

  if (metaObject->constructorCount() == 0) {
    fmt->writeComment("Has no invokable constructors");
  }

  int count = metaObject->propertyCount();
  int offset = QObject::staticMetaObject.propertyCount();
  int nProperties = 0;

  for (int i=offset; i<count; i++) {
    QMetaProperty metaProperty = metaObject->property(i);

    if (metaProperty.isStored()) {
      nProperties++;
    }
  }

  nProperties += dynamicPropertyNames().length();

  if (nProperties > 0) {
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
  }

  int nChildren = 0;

  for (int i=0; i<m_Children.count(); i++) {
    QcepObjectPtr obj = m_Children.value(i);

    if (obj) {
      nChildren++;
    }
  }

  if (nChildren > 0) {
    fmt->beginWriteChildren();

    for (int i=0; i<m_Children.count(); i++) {
      QcepObjectPtr obj = m_Children.value(i);

      if (obj) {
        obj->writeObject(fmt);
      }
    }

    fmt->endWriteChildren();
  }

  fmt->beginWriteData();
  writeObjectData(fmt);
  fmt->endWriteData();

  fmt->endWriteObject();
}

void QcepObject::readObject(QcepFileFormatterPtr fmt)
{
  fmt->printMessage("QcepObject::readObject");

  fmt->beginReadObject(sharedFromThis());

  if (fmt->beginReadProperties()) {
    QString propName;
    QVariant propVal;

    do {
      fmt->printMessage("QcepObject::readObject : read property");

      propName = fmt->nextPropertyName();

      if (propName.length() > 0) {
        propVal = fmt->nextPropertyValue();
        setProperty(qPrintable(propName), propVal);
      }
    } while (propName.length()>0);

    fmt->endReadProperties();
  }

  if (fmt->beginReadChildren()) {
    QcepObjectPtr child;

    do {
      fmt->printMessage("QcepObject::readObject : read child");

      child = fmt->nextChild();

      if (child) {
        addChildPtr(child);
      }
    } while (child);

    fmt->endReadChildren();
  }

  if (fmt->beginReadData()) {
    fmt->printMessage("QcepObject::readObject : read data");

    readObjectData(fmt);

    fmt->endReadData();
  }

  fmt->endReadObject();
}

void QcepObject::writeObjectData(QcepFileFormatterPtr fmt)
{
}

void QcepObject::readObjectData(QcepFileFormatterPtr fmt)
{
}

QString QcepObject::toScriptLiteral(QVariant v)
{
  if (v.type() == QMetaType::QString) {
    return "\"" + addSlashes(v.toString()) + "\"";
  }

  else if (v.type() == QMetaType::QStringList) {
    QStringList l = v.toStringList();
    QString res = "@QStringList(";
    for(int i=0; i<l.length(); i++) {
      if (i > 0) {
        res += ", ";
      }
      res += toScriptLiteral(l.value(i));
    }
    res += ")";
    return res;
  }

  else if (v.type() == QMetaType::QPointF) {
    QPointF p = v.toPointF();
    return tr("@QPointF(%1,%2)").arg(p.x()).arg(p.y());
  }

  else if (v.type() == QMetaType::QByteArray) {
    QByteArray comp = qCompress(v.toByteArray());
    return tr("@QByteArray(\"%1\")").arg(QString(comp.toBase64()));
  }

  else {
    QString s = v.toString();

    if (s.length() > 0) {
      return s;
    } else {
      return tr("@%1()").arg(v.typeName());
    }
  }
}

QVariant QcepObject::fromScriptLiteral(QString lit)
{
}

QScriptValue QcepObject::toScriptValue(QScriptEngine *engine, const QcepObjectPtr &data)
{
  return engine->newQObject(data.data());
}

void QcepObject::fromScriptValue(const QScriptValue &obj, QcepObjectPtr &data)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepObject *qcobj = qobject_cast<QcepObject*>(qobj);

    if (qcobj) {
      QcepObjectPtr p = qcobj->sharedFromThis();

      if (p) {
        data = p;
      }
    }
  }
}

QcepObjectPtr QcepObject::construct(QString className)
{
  QcepObjectPtr res;

  int typeId = QMetaType::type(qPrintable(className+"*"));

  if (typeId == QMetaType::UnknownType) {
    printMessage(tr("Type %1 is unknown").arg(className));
  } else {
    const QMetaObject *obj = QMetaType::metaObjectForType(typeId);

    if (obj == NULL) {
      printMessage(tr("Metaobject is NULL"));
    } else {
      QObject *qobj = obj->newInstance();

      if (qobj == NULL) {
        printMessage(tr("qObject == NULL"));
      } else {
        QcepObject *qcobj = qobject_cast<QcepObject*>(qobj);

        if (qcobj == NULL) {
          printMessage(tr("QcepObject == NULL"));
        } else {
          res= QcepObjectPtr(qcobj);
        }
      }
    }
  }

  return res;
}
