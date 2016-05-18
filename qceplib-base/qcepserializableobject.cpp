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

void QcepSerializableObject::readObject(QcepFileFormatterPtr fmt)
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

void QcepSerializableObject::writeObjectData(QcepFileFormatterPtr fmt)
{
}

void QcepSerializableObject::readObjectData(QcepFileFormatterPtr fmt)
{
}

void QcepSerializableObject::setParentPtr(QcepSerializableObjectWPtr parent)
{
  m_Parent = parent;
//  if (m_Parent != parent) {
//    QcepObjectPtr oldParent(m_Parent);
//    QcepObjectPtr newParent(parent);

//    if (newParent) {
//      m_Parent = newParent;
//    } else {
//      printMessage("Attempt to set parent to non-existing object");
//    }

//    if (oldParent) {
//      oldParent->removeChildPtr(sharedFromThis());
//    }

//    if (newParent) {
//      newParent->addChildPtr(sharedFromThis());
//    }
//  }
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
  QcepObjectWPtr myself(sharedFromThis());

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


QcepSerializableObjectPtr QcepSerializableObject::construct(QString className)
{
  QcepSerializableObjectPtr res;

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
        QcepSerializableObject *qcobj = qobject_cast<QcepSerializableObject*>(qobj);

        if (qcobj == NULL) {
          printMessage(tr("QcepSerializableObject == NULL"));
        } else {
          res= QcepSerializableObjectPtr(qcobj);
        }
      }
    }
  }

  return res;
}
