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

//  QcepObjectPtr p(m_Parent);

//  if (p) {
//    p->addChildPtr(sharedFromThis());
//  }
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

void QcepObject::setParentPtr(QcepObjectWPtr parent)
{
  m_Parent = parent;
}

QcepObjectWPtr QcepObject::parentPtr()
{
  return m_Parent;
}

const QcepObjectWPtr QcepObject::parentPtr() const
{
  return m_Parent;
}

int QcepObject::childCount() const
{
  return m_Children.count();
}

QcepObjectPtr QcepObject::childPtr(int n) const
{
  QcepObjectPtr p = m_Children.value(n);

  if (p) {
    return p;
  } else {
    return QcepObjectPtr();
  }
}


#ifndef QT_NO_DEBUG
void QcepObject::checkPointerMatchCount(QcepObjectWPtr ptr)
{
  QcepObjectPtr p(ptr);

  if (p) {
    if (m_PointerMatchCount > 0) {
      printMessage(tr("%1:%2 matches multiple times").arg(p->get_Name()).arg(p->className()));
    }
    m_PointerMatchCount++;
  }
}
#endif


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

  for(int i=0; i<childCount(); i++) {
    QcepObjectPtr child = childPtr(i);

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

void QcepObject::addChildPtr(QcepObjectPtr child)
{
  QcepObjectWPtr myself = sharedFromThis();

  if (m_Children.contains(child)) {
    printMessage("Added same child more than once");
  } else {
    m_Children.append(child);
  }

  if (child) {
    child->setParentPtr(myself);
  }
//  if (sharedFromThis()) {
//  } else {
//    printMessage("Adding child when sharedFromThis() == NULL");
//  }

#ifndef QT_NO_DEBUG
  m_PointerMatchCount = 0;
#endif
}

void QcepObject::removeChildPtr(QcepObjectPtr child)
{
  if (m_Children.contains(child)) {
    m_Children.removeAll(child);
  } else {
    printMessage("Removing object which is not a child");
  }
}

void QcepObject::clearChildren()
{
  int n = childCount();

  for (int i=n-1; i>=0; i--) {
    removeChildPtr(childPtr(i));
  }
}

void QcepObject::prependChildPtr(QcepObjectPtr child)
{
  addChildPtr(child);

  int n = childCount();

  for (int i=n-2; i>=0; i--) {
    m_Children[i+1] = m_Children[i];
  }

  m_Children[0] = child;
}

void QcepObject::insertChildPtr(int atRow, QcepObjectPtr child)
{
  addChildPtr(child);

  int n = childCount();

  for (int i=n-2; i>=atRow; i--) {
    m_Children[i+1] = m_Children[i];
  }

  m_Children[atRow] = child;
}

void QcepObject::propertyChanged(QcepProperty *prop)
{
  if (prop == NULL || prop->isStored()) {
    m_ChangeCount.fetchAndAddOrdered(1);
    m_LastChanged.store(prop);
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

void QcepObject::setChanged(int /*ct*/)
{
  m_ChangeCount.fetchAndStoreOrdered(0);
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

QString QcepObject::className() const
{
  return metaObject()->className();
}

QString QcepObject::get_Type() const
{
  return metaObject()->className();
}

void QcepObject::printLine(QString line) const
{
  QcepObjectPtr p(m_Parent);

  if (p) {
    p->printLine(line);
  } else {
    QcepObject *p = qobject_cast<QcepObject*>(parent());

    if (p) {
      p->printLine(line);
    } else {
      printf("%s\n", qPrintable(line));
    }
  }
}

void QcepObject::printMessage(QString msg, QDateTime dt) const
{
  QcepObjectPtr p(m_Parent);

  if (p) {
    p->printMessage(msg, dt);
  } else {
    QcepObject *p = qobject_cast<QcepObject*>(parent());

    if (p) {
      p->printMessage(msg, dt);
    } else {
      printf("MESSAGE: %s %s\n",
             qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
    }
  }
}

void QcepObject::criticalMessage(QString msg, QDateTime dt) const
{
  QcepObjectPtr p(m_Parent);

  if (p) {
    p->criticalMessage(msg, dt);
  } else {
    QcepObject *p = qobject_cast<QcepObject*>(parent());

    if (p) {
      p->criticalMessage(msg, dt);
    } else {
      printf("CRITICAL: %s %s\n",
             qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
    }
  }
}

void QcepObject::statusMessage(QString msg, QDateTime dt) const
{
  QcepObjectPtr p(parentPtr());

  if (p) {
    p->statusMessage(msg, dt);
  } else {
    QcepObject *p = qobject_cast<QcepObject*>(parent());

    if (p) {
      p->statusMessage(msg, dt);
    } else {
      printf("STATUS: %s %s\n",
             qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
    }
  }
}

void QcepObject::writeSettings(QSettings *set)
{
  if (set) {
    set->setValue("name", get_Name());
    set->setValue("class", className());
  }

  set->beginGroup("properties");
  QcepProperty::writeSettings(this, set);
  set->endGroup();

  m_ChangeCount.fetchAndStoreOrdered(0);
  m_LastChanged.store(NULL);
}

void QcepObject::readSettings(QSettings *set)
{
  if (QThread::currentThread() != thread()) {
//    INVOKE_CHECK(QMetaObject::invokeMethod(this, "readObjectSettings", Qt::BlockingQueuedConnection, Q_ARG(QSettings*, set)));
    readObjectSettings(set);
  } else {
    readObjectSettings(set);
  }
}

void QcepObject::readObjectSettings(QSettings *set)
{
  set->beginGroup("properties");
  QcepProperty::readSettings(this, set);
  set->endGroup();
}

QcepObjectPtr QcepObject::readObject(QSettings *set)
{
  QcepObjectPtr res;

  if (set) {
    QStringList keys  = set->childKeys();
    QStringList allk  = set->allKeys();

    QString className = set->value("class", "").toString();
    QString name      = set->value("name",  "").toString();

    if (className.length() && name.length()) {
      res = construct(name, className);

      if (res) {
        int nChildren = set -> beginReadArray("children");

        for (int i=0; i<nChildren; i++) {
          set->setArrayIndex(i);

          QcepObjectPtr child = readObject(set);

          if (child) {
            res->addChildPtr(child);
          }
        }

        set->endArray();

        res -> readSettings(set);
      }
    }
  }

  return res;
}

QcepObjectPtr QcepObject::construct(QString name, QString className)
{
  QcepObjectPtr res;

  int typeId = QMetaType::type(qPrintable(className+"*"));

  if (typeId == QMetaType::UnknownType) {
    printf("Type %s* is unknown\n", qPrintable(className));
  } else {
    const QMetaObject *metaObj = QMetaType::metaObjectForType(typeId);

    if (metaObj == NULL) {
      printf("Metaobject is NULL\n");
    } else {
      QObject *obj = metaObj->newInstance(Q_ARG(QString, name));

      if (obj == NULL) {
        printf("qObject == NULL\n");
      } else {
        QcepObject *qcepobj = qobject_cast<QcepObject*>(obj);

        if (qcepobj == NULL) {
          printf("QcepObject == NULL\n");
        } else {
          res = QcepObjectPtr(qcepobj);
        }
      }
    }
  }

  return res;
}

void QcepObject::writeObject(QSettings *set)
{
  writeSettings(set);

  int nChildren = childCount();

  set->beginWriteArray("children", nChildren);

  for (int i=0; i<nChildren; i++) {
    set->setArrayIndex(i);

    QcepObjectPtr child = childPtr(i);

    if (child) {
      child->writeObject(set);
    }
  }

  set->endArray();
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
    } else if (str[i] == '\r') {
      newStr.append("\\");
      newStr.append("r");
    } else {
      newStr.append(str[i]);
    }
  }

  return newStr;
}

QString QcepObject::removeSlashes(QString str)
{
  QString newStr;

  return str;
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

QString QcepObject::toScriptLiteral(QVariant v)
{
  if (v.type() == QVariant::Type(QMetaType::QString)) {
    return "\"" + addSlashes(v.toString()) + "\"";
  }

  else if (v.type() == QVariant::Type(QMetaType::QStringList)) {
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

  else if (v.type() == QVariant::Type(QMetaType::QPointF)) {
    QPointF p = v.toPointF();
    return tr("@QPointF(%1,%2)").arg(p.x()).arg(p.y());
  }

  else if (v.type() == QVariant::Type(QMetaType::QByteArray)) {
    QByteArray comp = qCompress(v.toByteArray());
    return tr("@QByteArray(\"%1\")").arg(QString(comp.toBase64()));
  }

  else if (v.type() == QVariant::Type(QMetaType::QDateTime)) {
    return tr("@QDateTime(\"%1\")").arg(v.toString());
  }

  else if (v.type() == QVariant::Type(QMetaType::QDate)) {
    return tr("@QDate(\"%1\")").arg(v.toString());
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

QVariant QcepObject::fromScriptLiteral(QString /*lit*/)
{
  return QVariant();
}

QVariant QcepObject::parseVariant(QString str)
{
  if (str.startsWith("@")) {

  }

  return QVariant();
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

QString QcepObject::methodSignatures()
{
  QString res = "";

  for (int i=0; i<methodCount(); i++) {
    if (i > 0) res += ", ";
    res += methodSignature(i);
  }

  return res;
}

QString QcepObject::methodSignature(int n)
{
  QString res = "";

  const QMetaObject *meta = metaObject();

  if (meta) {
    QMetaMethod m = meta->method(n);

    if (m.isValid()) {
      res = m.methodSignature();
    }
  }

  return res;
}

int QcepObject::methodCount()
{
  int res = -1;

  const QMetaObject *meta = metaObject();

  if (meta) {
    res = meta->methodCount();
  }

  return res;
}
