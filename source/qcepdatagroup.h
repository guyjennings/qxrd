#ifndef QCEPDATAGROUP_H
#define QCEPDATAGROUP_H

#include <QObject>
#include <QVector>
#include "qcepdataobject.h"
#include "qcepdataobject-ptr.h"

class QcepDataGroup : public QcepDataObject
{
  Q_OBJECT

public:
  QcepDataGroup(QString name, QObject *parent = 0);

public slots:
  QcepDataObjectPtr item(int n);
  QcepDataObjectPtr item(QString nm);
  int                nitems();

  void append(QcepDataObjectPtr obj);
  void remove(QcepDataObjectPtr obj);

private:
  QVector<QcepDataObjectPtr> m_Objects;
};

#endif // QCEPDATAGROUP_H
