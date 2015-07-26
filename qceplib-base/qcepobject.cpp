#include "qcepobject.h"
#include "qcepproperty.h"
#include <QMetaProperty>
#include <stdio.h>
#include <QAtomicInt>
#include <QSet>

static QAtomicInt s_ObjectAllocateCount(0);
static QAtomicInt s_ObjectDeleteCount(0);

static QSet<QcepObject*> s_Allocated;

QcepObject::QcepObject(QString name, QObject *parent) :
  QObject(parent),
  m_ObjectNamer(this, name)/*,
  m_Name(QcepSettingsSaverWPtr(), this, "name", name, "Object Name")*/
{
  s_ObjectAllocateCount.fetchAndAddOrdered(1);

#ifndef QT_NO_DEBUG
  s_Allocated.insert(this);
#endif
}

QcepObject::~QcepObject()
{
  s_ObjectDeleteCount.fetchAndAddOrdered(1);

#ifndef QT_NO_DEBUG
  s_Allocated.remove(this);
#endif
}

int QcepObject::allocatedObjects()
{
  int n = s_ObjectAllocateCount.load();
  int d = s_ObjectDeleteCount.load();

  printf("%d objects allocated\n", n);
  printf("%d objects released\n", d);
  printf("%d objects still allocated\n", n-d);

#ifndef QT_NO_DEBUG
  int i=0;

  foreach (QcepObject *obj, s_Allocated) {
    printf("%d : %s\n", i++, qPrintable(obj->objectName()));
  }
#endif

  return n-d;
}

void QcepObject::set_Name(QString name)
{
  setObjectName(name);
}

QString QcepObject::get_Name() const
{
  return objectName();
}

void QcepObject::printLine(QString line)
{
  if (parent()) {
    QMetaObject::invokeMethod( parent(), "printLine", Q_ARG(QString, line));
  }
}

void QcepObject::printMessage(QString msg, QDateTime dt) const
{
  if (parent()) {
    QMetaObject::invokeMethod( parent(), "printMessage", Q_ARG(QString, msg), Q_ARG(QDateTime, dt));
  } else {
    printf("MESSAGE: %s %s\n",
           qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
  }
}

void QcepObject::criticalMessage(QString msg, QDateTime dt) const
{
  if (parent()) {
    QMetaObject::invokeMethod( parent(), "criticalMessage", Q_ARG(QString, msg), Q_ARG(QDateTime, dt));
  } else {
    printf("MESSAGE: %s %s\n",
           qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
  }
}

void QcepObject::statusMessage(QString msg, QDateTime dt) const
{
  if (parent()) {
    QMetaObject::invokeMethod( parent(), "statusMessage", Q_ARG(QString, msg), Q_ARG(QDateTime, dt));
  } else {
    printf("MESSAGE: %s %s\n",
           qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
  }
}

void QcepObject::writeSettings(QSettings *set, QString section)
{
  QcepProperty::writeSettings(this, set, section);
}

void QcepObject::readSettings(QSettings *set, QString section)
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
