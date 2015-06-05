#include "qcepdataset.h"
#include "qcepdataarray.h"
#include "qcepdataarray-ptr.h"
#include "qcepdatagroup.h"
#include "qcepdatagroup-ptr.h"
#include "qcepdatacolumn.h"
#include "qcepdatacolumn-ptr.h"
#include "qcepdatacolumnscan.h"
#include "qcepdatacolumnscan-ptr.h"

QcepDataset::QcepDataset(QString name, QObject *parent) :
  QcepDataGroup(name, parent)
{

}

void QcepDataset::addGroup(QString path)
{
  QcepDataGroupPtr group(new QcepDataGroup(path));

  append(group);
}

void QcepDataset::addArray(QString path, QVector<int> dims)
{
  QcepDataArrayPtr array(new QcepDataArray(path, dims));

  append(array);
}

void QcepDataset::addColumn(QString path, int nrows)
{
  QcepDataColumnPtr column(new QcepDataColumn(path, nrows));

  append(column);
}

void QcepDataset::addColumnScan(QString path, int nrow, QStringList cols)
{
  QcepDataColumnScanPtr scan(new QcepDataColumnScan(path, cols, nrow));

  append(scan);
}
