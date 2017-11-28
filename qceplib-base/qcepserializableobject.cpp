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
    for(int i=0; i<childCount(); i++) {
      QcepSerializableObjectPtr child =
          qSharedPointerDynamicCast<QcepSerializableObject>(childPtr(i));

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
    for(int i=0; i<childCount(); i++) {
      QcepSerializableObjectPtr child =
          qSharedPointerDynamicCast<QcepSerializableObject>(childPtr(i));

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
//QVector<QcepSerializableObjectPtr> QcepSerializableObject::childrenPtr() const
//{
////  QVector<QcepObjectWPtr> res;

////  foreach (QcepObject* child, m_Children) {
////    if (child) {
////      res.append(child->sharedFromThis());
////    } else {
////      res.append(QcepObjectWPtr());
////    }
////  }

//  return m_Children;
//}

QcepSerializableObjectPtr QcepSerializableObject::readDataObject(QcepFileFormatterPtr fmt)
{
  if (fmt) {
    return fmt->nextObject();
  } else {
    return QcepSerializableObjectPtr();
  }
}

void QcepSerializableObject::writeObjectFmt(QcepFileFormatterPtr fmt)
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
      QcepSerializableObjectPtr obj = qSharedPointerDynamicCast<QcepSerializableObject>(childPtr(i));

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

void QcepSerializableObject::readObjectFmt(QcepFileFormatterPtr fmt)
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

void QcepSerializableObject::writeObjectData(QcepFileFormatterPtr /*fmt*/)
{
}

void QcepSerializableObject::readObjectData(QcepFileFormatterPtr /*fmt*/)
{
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

  for (int i=0; i<childCount(); i++) {
    QcepSerializableObjectPtr obj =
        qSharedPointerDynamicCast<QcepSerializableObject>(childPtr(i));

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
