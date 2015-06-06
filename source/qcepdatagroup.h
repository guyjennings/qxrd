#ifndef QCEPDATAGROUP_H
#define QCEPDATAGROUP_H

#include <QObject>
#include <QVector>
#include "qcepdataobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepdatagroup-ptr.h"

class QcepDataGroup : public QcepDataObject
{
  Q_OBJECT

public:
  QcepDataGroup(QcepSettingsSaverWPtr saver, QString name, QcepDataObjectWPtr parent = QcepDataObjectWPtr());

public slots:
  QcepDataObjectPtr item(int n) const;
  QcepDataObjectPtr item(QString nm) const;
  int                count() const;

  void append(QcepDataObjectPtr obj);
  void remove(QcepDataObjectPtr obj);

public:
  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepDataGroupPtr &data);
  static void fromScriptValue(const QScriptValue &obj, QcepDataGroupPtr &data);

private:
  QVector<QcepDataObjectPtr> m_Objects;
};

#endif // QCEPDATAGROUP_H
