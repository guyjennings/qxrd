#include "qcepdebug.h"
#include "qcepserializableobject.h"
#include "qcepfileformatter.h"
#include <QMetaProperty>

QcepSerializableObject::QcepSerializableObject(QString name) :
  QcepObject(name)
{
}

QcepSerializableObject::~QcepSerializableObject()
{
}

void QcepSerializableObject::propertyChanged(QcepProperty *prop)
{
  if (prop == NULL || prop->isStored()) {
    QcepObject::propertyChanged(prop);

    QcepObjectPtr parent(parentPtr());

    if (parent) {
      parent->propertyChanged(prop);
    }
  }
}

int QcepSerializableObject::childrenChanged() const
{
  if (isChanged()) {
    return true;
  } else {
    foreach(QcepSerializableObjectPtr child, m_Children) {
      if(child) {
        int chg = child->childrenChanged();

        if (chg) return chg;
      }
    }
  }

  return 0;
}

QString QcepSerializableObject::childrenChangedBy() const
{
  if (isChanged()) {
    return changedBy();
  } else {
    foreach(QcepSerializableObjectPtr child, m_Children) {
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

int QcepSerializableObject::checkChildren(int verbose, int level) const
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

  foreach(QcepSerializableObjectPtr child, m_Children) {
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

int QcepSerializableObject::childCount() const
{
  return m_Children.count();
}

QcepSerializableObjectWPtr QcepSerializableObject::childPtr(int n) const
{
  QcepSerializableObjectPtr p = m_Children.value(n);

  if (p) {
    return p;
  } else {
    return QcepSerializableObjectWPtr();
  }
}

QVector<QcepSerializableObjectPtr> QcepSerializableObject::childrenPtr() const
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

QcepSerializableObjectPtr QcepSerializableObject::readDataObject(QcepFileFormatterPtr fmt)
{
  if (fmt) {
    return fmt->nextObject();
  } else {
    return QcepSerializableObjectPtr();
  }
}

void QcepSerializableObject::writeObject(QcepFileFormatterPtr fmt)
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

  for (int i=0; i<m_Children.count(); i++) {
    QcepObjectPtr obj = m_Children.value(i);

    if (obj) {
      nChildren++;
    }
  }

  if (nChildren > 0) {
    fmt->beginWriteChildren();

    for (int i=0; i<m_Children.count(); i++) {
      QcepSerializableObjectPtr obj = m_Children.value(i);

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

void QcepSerializableObject::readObject(QcepFileFormatterPtr fmt)
{
  QcepSerializableObjectWPtr myself =
      qSharedPointerDynamicCast<QcepSerializableObject>(sharedFromThis());

  if (qcepDebug(DEBUG_IMPORT)) {
    fmt->printMessage("QcepSerializableObject::readObject");
  }

  fmt->beginReadObject(myself);

  if (fmt->beginReadProperties()) {
    QString propName;
    QVariant propVal;

    do {
      if (qcepDebug(DEBUG_IMPORT)) {
        fmt->printMessage("QcepSerializableObject::readObject : read property");
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
    QcepSerializableObjectPtr child;

    do {
      if (qcepDebug(DEBUG_IMPORT)) {
        fmt->printMessage("QcepSerializableObject::readObject : read child");
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

void QcepSerializableObject::writeObjectData(QcepFileFormatterPtr fmt)
{
}

void QcepSerializableObject::readObjectData(QcepFileFormatterPtr fmt)
{
}

#ifndef QT_NO_DEBUG
void QcepSerializableObject::checkPointerMatchCount(QcepSerializableObjectWPtr ptr)
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

void QcepSerializableObject::addChildPtr(QcepSerializableObjectPtr child)
{
  QcepSerializableObjectWPtr myself =
      qSharedPointerDynamicCast<QcepSerializableObject>(sharedFromThis());

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

void QcepSerializableObject::removeChildPtr(QcepSerializableObjectPtr child)
{
  if (m_Children.contains(child)) {
    m_Children.removeAll(child);
  } else {
    printMessage("Removing object which is not a child");
  }
}

void QcepSerializableObject::dumpObjectTreePtr(int level)
{
  const QMetaObject* metaObject = this->metaObject();
  QcepObjectPtr parent(parentPtr());

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
    QcepSerializableObjectPtr obj = m_Children.value(i);
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

void QcepSerializableObject::clearChildren()
{
  int n = childCount();

  for (int i=n-1; i>=0; i--) {
    removeChildPtr(childPtr(i));
  }
}

void QcepSerializableObject::prependChildPtr(QcepSerializableObjectPtr child)
{
  addChildPtr(child);

  int n = childCount();

  for (int i=n-2; i>=0; i--) {
    m_Children[i+1] = m_Children[i];
  }

  m_Children[0] = child;
}

void QcepSerializableObject::insertChildPtr(int atRow, QcepSerializableObjectPtr child)
{
  addChildPtr(child);

  int n = childCount();

  for (int i=n-2; i>=atRow; i--) {
    m_Children[i+1] = m_Children[i];
  }

  m_Children[atRow] = child;
}
