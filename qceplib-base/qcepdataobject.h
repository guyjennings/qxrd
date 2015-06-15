#ifndef QCEPDATAOBJECT_H
#define QCEPDATAOBJECT_H

#include <QObject>
#include "qcepobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepdatagroup-ptr.h"
#include <QScriptValue>
#include "qcepproperty.h"

class QcepDataObject : public QcepObject, public QEnableSharedFromThis<QcepDataObject>
{
  Q_OBJECT

public:
  QcepDataObject(QcepSettingsSaverWPtr saver,
                 QString name);

  QcepSettingsSaverWPtr saver();

  static QcepDataObjectPtr newDataObject(QcepSettingsSaverWPtr saver,
                                         QString name);

signals:
  void dataObjectChanged();

public slots:
  QString metaTypeName(int id) const;
  QString pathName() const;
  QcepDataGroupPtr rootItem();
  virtual QcepDataGroupPtr parentItem() const;

public:
  virtual int count() const;
  virtual QcepDataObjectPtr item(int n);
  virtual QcepDataObjectPtr item(QString nm);
  virtual void setParentItem(QcepDataGroupWPtr parent);
  virtual int indexInParent() const;
  virtual int columnCount() const;
  virtual QVariant columnData(int col) const;

  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepDataObjectPtr &data);
  static void fromScriptValue(const QScriptValue &obj, QcepDataObjectPtr &data);

private:
  QcepDataGroupWPtr     m_Parent;
  QcepSettingsSaverWPtr m_Saver;

public:
  Q_PROPERTY(QString type READ get_Type WRITE set_Type)
  QCEP_STRING_PROPERTY(Type)

  Q_PROPERTY(QString description READ get_Description WRITE set_Description)
  QCEP_STRING_PROPERTY(Description)
};

#endif // QCEPDATAOBJECT_H
