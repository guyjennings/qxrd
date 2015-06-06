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
  QcepDataObject(QcepSettingsSaverWPtr saver,
                 QString name,
                 QcepDataObjectWPtr parent = QcepDataObjectWPtr());

  QcepSettingsSaverWPtr saver();

signals:
  void dataObjectChanged();

public:
  virtual int count() const;
  virtual QcepDataObjectPtr item(int n) const;
  virtual QcepDataObjectPtr item(QString nm) const;
  virtual QcepDataObjectPtr parentItem() const;
  virtual int indexInParent() const;
  virtual int columnCount() const;
  virtual QVariant columnData(int col) const;

  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepDataObjectPtr &data);
  static void fromScriptValue(const QScriptValue &obj, QcepDataObjectPtr &data);

private:
  QcepDataObjectWPtr m_Parent;
  QcepSettingsSaverWPtr m_Saver;

public:
  Q_PROPERTY(QString type READ get_Type WRITE set_Type)
  QCEP_STRING_PROPERTY(Type)
};

#endif // QCEPDATAOBJECT_H
