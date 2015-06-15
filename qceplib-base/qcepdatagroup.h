#ifndef QCEPDATAGROUP_H
#define QCEPDATAGROUP_H

#include <QObject>
#include <QVector>
#include "qcepdataobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepdatagroup-ptr.h"
#include "qcepdataarray-ptr.h"
#include "qcepdatacolumn-ptr.h"
#include "qcepdatacolumnscan-ptr.h"

class QcepDataGroup : public QcepDataObject
{
  Q_OBJECT

public:
  QcepDataGroup(QcepSettingsSaverWPtr saver, QString name);

  static QcepDataGroupPtr newDataGroup(QcepSettingsSaverWPtr saver, QString name);

public slots:
  virtual QString description() const;

  QcepDataObjectPtr item(int n);
  QcepDataObjectPtr item(QString nm);
  int                count() const;

  void append(QcepDataObjectPtr obj);
  void append(QString path, QcepDataObjectPtr obj);
  void remove(QcepDataObjectPtr obj);
  void remove(QString path);

  void addGroup(QString path);
  void addArray(QString path, QVector<int> dims);
  void addColumn(QString path, int nrow);
  void addColumnScan(QString path, int nrow, QStringList cols);

  QcepDataGroupPtr newGroup(QString path);
  QcepDataArrayPtr newArray(QString path, QVector<int> dims);
  QcepDataColumnPtr newColumn(QString path, int nrow);
  QcepDataColumnScanPtr newColumnScan(QString path, int nrow, QStringList cols);

  static QScriptValue toGroupScriptValue(QScriptEngine *engine, const QcepDataGroupPtr &data);
  static void fromGroupScriptValue(const QScriptValue &obj, QcepDataGroupPtr &data);

  QcepDataGroupPtr containingGroup(QString path);
  QcepDataObjectPtr referencedObject(QString path);

private:
  QVector<QcepDataObjectPtr> m_Objects;
};

#endif // QCEPDATAGROUP_H
