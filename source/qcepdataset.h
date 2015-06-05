#ifndef QCEPDATASET_H
#define QCEPDATASET_H

#include <QObject>
#include "qcepdatagroup.h"

class QcepDataset : public QcepDataGroup
{
  Q_OBJECT

public:
  QcepDataset(QString name, QObject *parent = 0);

public slots:
  void addGroup(QString path);
  void addArray(QString path, QVector<int> dims);
  void addColumn(QString path, int nrow);
  void addColumnScan(QString path, int nrow, QStringList cols);
};

#endif // QCEPDATASET_H
