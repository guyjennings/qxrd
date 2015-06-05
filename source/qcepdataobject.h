#ifndef QCEPDATAOBJECT_H
#define QCEPDATAOBJECT_H

#include <QObject>
#include "qcepobject.h"
#include "qcepdataobject-ptr.h"
#include <QScriptValue>
#include "qcepproperty.h"

class QcepDataObject : public QcepObject
{
  Q_OBJECT

public:
  QcepDataObject(QcepSettingsSaverWPtr saver, QString name, QObject *parent = 0);

  QcepSettingsSaverWPtr saver();

public:
  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepDataObjectPtr &data);
  static void fromScriptValue(const QScriptValue &obj, QcepDataObjectPtr &data);

public:
  Q_PROPERTY(QString type READ get_Type WRITE set_Type)
  QCEP_STRING_PROPERTY(Type)

private:
  QcepSettingsSaverWPtr m_Saver;
};

#endif // QCEPDATAOBJECT_H
