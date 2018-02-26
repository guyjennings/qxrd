#include "qcepobject.h"
#include "qcepproperty.h"
#include <QMetaProperty>
#include <stdio.h>
#include <QAtomicInt>
#include <QSet>
#include <QThread>
#include "qcepfileformatter.h"
#include <QScriptEngine>
#include "qcepdebug.h"
#include "qcepapplication.h"
#include "qcepmainwindowsettings.h"

static QAtomicInt s_ObjectAllocateCount(0);
static QAtomicInt s_ObjectDeleteCount(0);

#ifndef QT_NO_DEBUG
static QSet<QcepObject*> s_Allocated;
#endif

QcepObject::QcepObject(QString name) :
  QObject(NULL),
  m_ObjectNamer(this, name),
  m_Initialized(false)
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

  QcepObjectPtr parent(m_Parent);

  if (parent) {
    parent->removeChildPtr(sharedFromThis());
  }

  s_ObjectDeleteCount.fetchAndAddOrdered(1);

#ifndef QT_NO_DEBUG
  s_Allocated.remove(this);
#endif
}

QString QcepObject::hexArg(const void *p)
{
#if (QT_POINTER_SIZE==4)
  return tr("0x%1").arg((quint32)p, 8, 16, QLatin1Char('0'));
#else
  return tr("0x%1").arg((quint64)p, 16, 16, QLatin1Char('0'));
#endif
}

void QcepObject::initialize(QcepObjectWPtr parent)
{
#ifndef QT_NO_DEBUG
  if (m_Initialized) {
    printf("QcepObject %s %s initialized multiple times\n",
           qPrintable(hexArg(this)),
           metaObject()->className());
  }

  if (parent == NULL) {
    printf("QcepObect %s %s initialized with NULL parent\n",
           qPrintable(hexArg(this)),
           metaObject()->className());
  }
#endif

  m_Initialized = true;
  m_Parent      = parent;

  QcepObjectPtr parP(m_Parent);

  if (parP) {
    parP -> addChildImpl(sharedFromThis());
  }
}

//TODO: eliminate (replace by calls to 'initialize')
void QcepObject::setParentPtr(QcepObjectWPtr parent)
{
  QcepObjectPtr oldParent(m_Parent);

  if (oldParent) {
    oldParent->removeChildPtr(sharedFromThis());
  }

  m_Parent = parent;

  QcepObjectPtr newParent(m_Parent);

  if (newParent) {
    newParent -> addChildImpl(sharedFromThis());
  }
}

void QcepObject::addChildImpl(QcepObjectWPtr child)
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  if (m_Children.contains(child)) {
    printMessage("Added same child more than once");
  } else {
    m_Children.append(child);
  }
}

void QcepObject::initializeRoot()
{
  m_Initialized = true;
  m_Parent      = QcepObjectPtr();
}

bool QcepObject::isInitialized() const
{
  return m_Initialized;
}

#ifndef QT_NO_DEBUG
void QcepObject::checkObjectInitialization() const
{
  if (m_Initialized == false) {
    printf("QcepObject %s %s not initialized\n",
           qPrintable(hexArg(this)),
           metaObject()->className());
  }
}
#endif

QcepObjectWPtr QcepObject::parentPtr()
{
  return m_Parent;
}

const QcepObjectWPtr QcepObject::parentPtr() const
{
  return m_Parent;
}

//TODO: reimplement by virtual method in all classes ?
int QcepObject::childCount() const
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  return m_Children.count();
}

//TODO: reimplement by virtual method in all classes ?
QcepObjectWPtr QcepObject::childPtr(int n) const
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  QcepObjectPtr p = m_Children.value(n);

  if (p) {
    return p;
  } else {
    return QcepObjectPtr();
  }
}

int QcepObject::checkChildren(int verbose, int level) const
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

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

//TODO: move functionality into QcepDataGroup
void QcepObject::addChildPtr(QcepObjectWPtr child)
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  if (child) {
    addChildImpl(child);

    QcepObjectPtr childP(child);

    if (childP) {
      childP->setParentPtr(sharedFromThis());
    }
  } else {
#ifndef QT_NO_DEBUG
    printMessage(tr("QcepObject::addChildPtr(NULL)"));
#endif
  }
}

//TODO: move functionality into QcepDataGroup
void QcepObject::removeChildPtr(QcepObjectWPtr child)
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  if (child) {
    if (m_Children.contains(child)) {
      m_Children.removeAll(child);
    } else {
      printMessage("Removing object which is not a child");
    }
  }
}

//TODO: move functionality into QcepDataGroup
void QcepObject::clearChildren()
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  int n = childCount();

  for (int i=n-1; i>=0; i--) {
    removeChildPtr(childPtr(i));
  }
}

//TODO: move functionality into QcepDataGroup
void QcepObject::prependChildPtr(QcepObjectWPtr child)
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  addChildPtr(child);

  int n = childCount();

  for (int i=n-2; i>=0; i--) {
    m_Children[i+1] = m_Children[i];
  }

  m_Children[0] = child;
}

//TODO: move functionality into QcepDataGroup
void QcepObject::insertChildPtr(int atRow, QcepObjectWPtr child)
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  addChildPtr(child);

  int n = childCount();

  for (int i=n-2; i>=atRow; i--) {
    m_Children[i+1] = m_Children[i];
  }

  m_Children[atRow] = child;
}


//TODO: remove?
int QcepObject::childrenChanged() const
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  if (isChanged()) {
    return true;
  } else {
    for(int i=0; i<childCount(); i++) {
      QcepObjectPtr child = childPtr(i);

      if(child) {
        int chg = child->childrenChanged();

        if (chg) return chg;
      }
    }
  }

  return 0;
}

//TODO: remove?
QString QcepObject::childrenChangedBy() const
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  if (isChanged()) {
    return changedBy();
  } else {
    for(int i=0; i<childCount(); i++) {
      QcepObjectPtr child = childPtr(i);

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

void QcepObject::dumpParentage()
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  QcepObjectPtr p = sharedFromThis();

  printMessage("Object Parentage of");

  while (p) {
    const char* className = p->metaObject()->className();

    printMessage(tr("%1 : %2").HEXARG(p.data()).arg(className));

    if (p) {
      p = p->parentPtr();
    }
  }
}

void QcepObject::openObjectBrowser()
{
  if (g_Application) {
    g_Application -> openObjectBrowserWindow(sharedFromThis());
  }
}

void QcepObject::propertyChanged(QcepProperty *prop)
{
//#ifndef QT_NO_DEBUG
//  checkObjectInitialization();
//#endif

  if (prop == NULL || prop->isStored()) {
    m_ChangeCount.fetchAndAddOrdered(1);
    m_LastChanged.store(prop);

    QcepObjectPtr parent(m_Parent);

    if (parent) {
      parent->propertyChanged(prop);
    }
  }
}

int QcepObject::isChanged() const
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  return m_ChangeCount.load();
}

QString QcepObject::changedBy() const
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  QcepProperty *p = m_LastChanged.load();

  if (p) {
    return p->parentName()+"."+p->name();
  } else {
    return "NULL";
  }
}

void QcepObject::setChanged(int /*ct*/)
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

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
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  setObjectName(name);
}

QString QcepObject::get_Name() const
{
//#ifndef QT_NO_DEBUG
//  checkObjectInitialization();
//#endif

  return objectName();
}

QString QcepObject::className() const
{
//#ifndef QT_NO_DEBUG
//  checkObjectInitialization();
//#endif

  return metaObject()->className();
}

QString QcepObject::get_Type() const
{
//#ifndef QT_NO_DEBUG
//  checkObjectInitialization();
//#endif

  return metaObject()->className();
}

void QcepObject::printLine(QString line) const
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

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
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

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
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

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
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  QcepObjectPtr p(m_Parent);

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

void QcepObject::splashMessage(QString msg, QDateTime dt)
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  QcepObjectPtr p(m_Parent);

  if (p) {
    p->splashMessage(msg, dt);
  } else {
    QcepObject *p = qobject_cast<QcepObject*>(parent());

    if (p) {
      p->splashMessage(msg, dt);
    } else {
      printf("SPLASH: %s %s\n",
             qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
    }
  }
}

void QcepObject::writeSettings(QSettings *settings)
{
#ifndef QT_NO_DEBUG
  checkObjectInitialization();
#endif

  if (settings) {
    settings->setValue("name", get_Name());
    settings->setValue("class", className());

    settings->beginGroup("properties");
    QcepProperty::writeSettings(this, settings);
    settings->endGroup();
  }

  settings->beginWriteArray("windowSettings");

  for (int i=0; i<m_WindowSettings.count(); i++) {
    settings->setArrayIndex(i);
    QcepMainWindowSettingsPtr set = windowSettings(i);

    if (set) {
      set->writeSettings(settings);
    }
  }

  settings->endArray();

  m_ChangeCount.fetchAndStoreOrdered(0);
  m_LastChanged.store(NULL);
}

void QcepObject::readSettings(QSettings *settings)
{
//#ifndef QT_NO_DEBUG
//  checkObjectInitialization();
//#endif
  if (settings) {
    settings->beginGroup("properties");
    QcepProperty::readSettings(this, settings);
    settings->endGroup();
  }

  int n = settings->beginReadArray("windowSettings");

  for (int i=0; i<m_WindowSettings.count(); i++) {
    settings->setArrayIndex(i);
    QcepMainWindowSettingsPtr set = windowSettings(i);

    if (set) {
      set->readSettings(settings);
    }
  }

  settings->endArray();

  m_ChangeCount.fetchAndStoreOrdered(0);
  m_LastChanged.store(NULL);

//  readObjectSettings(set);
}

//TODO: add parent ptr for initialization
QcepObjectPtr QcepObject::readObject(QcepObjectWPtr parent, QSettings *set)
{
  QcepObjectPtr res;

  if (set) {
//    QStringList keys  = set->childKeys();
//    QStringList allk  = set->allKeys();

    QString className = set->value("class", "").toString();
    QString name      = set->value("name",  "").toString();

    if (className.length() && name.length()) {
      res = construct(name, className);

      if (res) {
        res -> initialize(parent);

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

void QcepObject::dumpObjectTreePtr(int level)
{
  const QMetaObject* metaObject = this->metaObject();
  QcepObjectPtr p(m_Parent);

  printLine(tr("%1// %2: %3 constrs, parent %4")
            .arg("", level)
            .arg(metaObject->className())
            .arg(metaObject->constructorCount())
            .arg(p ? p->get_Type() : "NULL"));

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

  for (int i=0; i<childCount(); i++) {
    QcepObjectPtr obj = childPtr(i);

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

//TODO: remove
QcepObjectPtr QcepObject::readDataObject(QcepFileFormatterPtr fmt)
{
  if (fmt) {
    return fmt->nextObject();
  } else {
    return QcepObjectPtr();
  }
}

//TODO: remove
void QcepObject::writeObjectFmt(QcepFileFormatterPtr fmt)
{
  const QMetaObject* metaObject = this->metaObject();
//  QcepObjectPtr parent(m_Parent);

  fmt->beginWriteObject(get_Name(), metaObject->className());

  int typeId = QMetaType::type(qPrintable(className()+"*"));

  if (typeId == QMetaType::UnknownType) {
    fmt -> writeComment("Has unknown type");
  }

  if (metaObject->constructorCount() == 0) {
    fmt -> writeComment("Has no invokable constructors");
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

  for (int i=0; i<childCount(); i++) {
    QcepObjectPtr obj = childPtr(i);

    if (obj) {
      nChildren++;
    }
  }

  if (nChildren > 0) {
    fmt->beginWriteChildren();

    for (int i=0; i<childCount(); i++) {
      QcepObjectPtr obj = childPtr(i);

      if (obj) {
        obj->writeObjectFmt(fmt);
      }
    }

    fmt->endWriteChildren();
  }

  fmt->beginWriteData();
  writeObjectData(fmt);
  fmt->endWriteData();

  fmt->endWriteObject();
}

//TODO: remove
void QcepObject::readObjectFmt(QcepFileFormatterPtr fmt)
{
  QcepObjectWPtr myself = sharedFromThis();

  if (qcepDebug(DEBUG_IMPORT)) {
    fmt->printMessage("QcepObject::readObject");
  }

  fmt->beginReadObject(myself);

  if (fmt->beginReadProperties()) {
    QString propName;
    QVariant propVal;

    do {
      if (qcepDebug(DEBUG_IMPORT)) {
        fmt->printMessage("QcepObject::readObject : read property");
      }

      propName = fmt->nextPropertyName();

      if (propName.length() > 0) {
        propVal = fmt->nextPropertyValue();
        setProperty(qPrintable(propName), propVal);

        if (qcepDebug(DEBUG_IMPORT)) {
          fmt->printMessage(tr("Set property %1 to %2").arg(propName).arg(propVal.toString()));
        }
      }
    } while (propName.length()>0);

    fmt->endReadProperties();
  }

  if (fmt->beginReadChildren()) {
    QcepObjectPtr child;

    do {
      if (qcepDebug(DEBUG_IMPORT)) {
        fmt->printMessage("QcepObject::readObject : read child");
      }

      child = fmt->nextChild();

      if (child) {
        addChildPtr(child);
      }
    } while (child);

    fmt->endReadChildren();
  }

  if (fmt->beginReadData()) {
    if (qcepDebug(DEBUG_IMPORT)) {
      fmt->printMessage("QcepObject::readObject : read data");
    }

    readObjectData(fmt);

    fmt->endReadData();
  }

  fmt->endReadObject();
}

//TODO: remove
void QcepObject::writeObjectData(QcepFileFormatterPtr /*fmt*/)
{
}

//TODO: remove
void QcepObject::readObjectData(QcepFileFormatterPtr /*fmt*/)
{
}

#ifndef QT_NO_DEBUG
void QcepObject::checkInitialization()
{
  foreach (QcepObject* obj, s_Allocated) {
    if (obj && !obj->isInitialized()) {
      printMessage(tr("Object %1 (%2) at %3 not initialized")
                   .arg(obj->objectName())
                   .arg(obj->get_Type())
                   .HEXARG(obj)
                   );
    }
  }
}
#endif

int QcepObject::windowSettingsCount()
{
  return m_WindowSettings.count();
}

QcepMainWindowSettingsPtr QcepObject::windowSettings(int n)
{
  return m_WindowSettings.value(n);
}

QcepMainWindowSettingsPtr QcepObject::appendWindowSettings(QcepMainWindowSettingsPtr settings)
{
  if (settings) {
//    for (int i=0; i<windowSettingsCount(); i++) {
//      QcepMainWindowSettingsPtr set = windowSettings(i);

//      if (set && set->className() == settings->className()) {
//        return;
//      }
//    }

    m_WindowSettings.append(settings);
  }

  return settings;
}

void QcepObject::defaultWindowSettings()
{

}

void QcepObject::openWindows()
{
  if (g_Application && g_Application -> get_GuiWanted()) {
    foreach (QcepObjectPtr child, m_Children) {
      QcepMainWindowSettingsPtr set =
          qSharedPointerDynamicCast<QcepMainWindowSettings>(child);

      if (set) {
        set   -> openWindow();
      } else if (child) {
        child -> openWindows();
      }
    }
  }
}

void QcepObject::closeWindows()
{
  foreach (QcepObjectPtr child, m_Children) {
    QcepMainWindowSettingsPtr set =
        qSharedPointerDynamicCast<QcepMainWindowSettings>(child);

    if (set) {
      set   -> closeWindow();
    } else if (child) {
      child -> closeWindows();
    }
  }
}
