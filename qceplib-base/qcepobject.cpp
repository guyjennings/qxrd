#include "qcepobject.h"
#include "qcepproperty.h"
#include <QMetaProperty>
#include <stdio.h>

QcepObject::QcepObject(QString name, QObject *parent) :
  QObject(parent),
  m_ObjectNamer(this, name)/*,
  m_Name(QcepSettingsSaverWPtr(), this, "name", name, "Object Name")*/
{
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

void QcepObject::printMessage(QString msg, QDateTime dt)
{
  if (parent()) {
    QMetaObject::invokeMethod( parent(), "printMessage", Q_ARG(QString, msg), Q_ARG(QDateTime, dt));
  } else {
    printf("MESSAGE: %s %s\n",
           qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
  }
}

void QcepObject::writeSettings(QSettings *set, QString section)
{
  const QMetaObject *meta = metaObject();

  while (meta) {
    QcepProperty::writeSettings(this, meta, section, set);

    meta = meta->superClass();

    if (meta == &QObject::staticMetaObject) break;
  }
}

void QcepObject::readSettings(QSettings *set, QString section)
{
  QcepProperty::readSettings(this, metaObject(), section, set);
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
