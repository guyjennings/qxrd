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
  QcepDataGroup(QcepSettingsSaverWPtr saver, QString name);

  static QcepDataGroupPtr newDataGroup(QcepSettingsSaverWPtr saver, QString name);

public slots:
  QcepDataObjectPtr item(int n) const;
  QcepDataObjectPtr item(QString nm) const;
  int                count() const;

  void append(QcepDataObjectPtr obj);
  void remove(QcepDataObjectPtr obj);

  void addGroup(QString path);
  void addArray(QString path, QVector<int> dims);
  void addColumn(QString path, int nrow);
  void addColumnScan(QString path, int nrow, QStringList cols);

private:
  QVector<QcepDataObjectPtr> m_Objects;
};

#endif // QCEPDATAGROUP_H
