#ifndef QCEPFILEIMPORTERHDF_H
#define QCEPFILEIMPORTERHDF_H

#include "qcepfileimporter.h"
#include "hdf5.h"
#include "qcepdatacolumn-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepdataarray-ptr.h"

class QcepFileImporterHDF : public QcepFileImporter
{
  Q_OBJECT

public:
  QcepFileImporterHDF(QcepDatasetModelPtr model,
                      QModelIndexList &indexes,
                      QString path);

  void exec();

private:
  void scanGroup(QModelIndex dest, hid_t gid, int level=0);
  void scanDataset(QModelIndex dest, hid_t dsid, char *name);
  void scanLink(QModelIndex dest, hid_t gid, char *name);

  void readColumnData(QcepDataColumnPtr col, hid_t dsid);
  void readImageData(QcepDoubleImageDataPtr img, hid_t dsid);
  void readArrayData(QcepDataArrayPtr arr, hid_t dsid);
};

#endif // QCEPFILEIMPORTERHDF_H
