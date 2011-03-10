#include "qxrdfilebrowsermodel.h"
#include <QTime>
#include <stdio.h>

QxrdFileBrowserModel::QxrdFileBrowserModel(QObject *parent) :
    QFileSystemModel(parent)
{
}

QVariant QxrdFileBrowserModel::data(const QModelIndex &index, int role) const
{
  QTime tic;
  tic.start();

  QVariant result = QFileSystemModel::data(index, role);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    printf("QxrdFileBrowserModel::data took %d msec\n", elapsed);
  }

  return result;
}
