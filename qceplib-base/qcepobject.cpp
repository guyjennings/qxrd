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
  QcepObject *p = qobject_cast<QcepObject*>(parent());

  if (p) {
    p->printLine(line);
  } else {
    printf("%s\n", qPrintable(line));
  }
}

void QcepObject::printMessage(QString msg, QDateTime dt) const
{
  QcepObject *p = qobject_cast<QcepObject*>(parent());

  if (p) {
    p->printMessage(msg, dt);
  } else {
    printf("MESSAGE: %s %s\n",
           qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
  }
}

void QcepObject::criticalMessage(QString msg, QDateTime dt) const
{
  QcepObject *p = qobject_cast<QcepObject*>(parent());

  if (p) {
    p->criticalMessage(msg, dt);
  } else {
    printf("CRITICAL: %s %s\n",
           qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
  }
}

void QcepObject::statusMessage(QString msg, QDateTime dt) const
{
  QcepObject *p = qobject_cast<QcepObject*>(parent());

  if (p) {
    p->statusMessage(msg, dt);
  } else {
    printf("STATUS: %s %s\n",
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
    } else if (str[i] == '\r') {
      newStr.append("\\");
      newStr.append("r");
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
