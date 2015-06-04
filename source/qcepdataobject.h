#ifndef QCEPDATAOBJECT_H
#define QCEPDATAOBJECT_H

#include <QObject>
#include "qcepobject.h"
#include "qcepdataobject-ptr.h"
#include <QScriptValue>

class QcepDataObject : public QcepObject
{
  Q_OBJECT

public:
  QcepDataObject(QString name, QObject *parent = 0);

public:
  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepDataObjectPtr &data);
  static void fromScriptValue(const QScriptValue &obj, QcepDataObjectPtr &data);
};

#endif // QCEPDATAOBJECT_H
