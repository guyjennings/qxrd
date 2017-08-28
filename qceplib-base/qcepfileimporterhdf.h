#ifndef QCEPFILEIMPORTERHDF_H
#define QCEPFILEIMPORTERHDF_H

#include "qceplib_global.h"
#include "qcepfileimporter.h"

#ifdef HAVE_HDF5
#include "hdf5.h"
#endif

#include "qcepdatacolumn-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepdataarray-ptr.h"

class QCEP_EXPORT QcepFileImporterHDF : public QcepFileImporter
{
  Q_OBJECT

public:
  QcepFileImporterHDF(QcepDatasetModelPtr model,
                      QModelIndexList &indexes,
                      QString path);

  void exec();

private:
#ifdef HAVE_HDF5
  void scanGroup(QModelIndex dest, hid_t gid, int level=0);
  void scanDataset(QModelIndex dest, hid_t dsid, char *name);
  void scanLink(QModelIndex dest, hid_t gid, char *name);

  void readColumnData(QcepDataColumnPtr col, hid_t dsid);
  void readImageData(QcepDoubleImageDataPtr img, hid_t dsid);
  void readArrayData(QcepDataArrayPtr arr, hid_t dsid);
#endif
};

#endif // QCEPFILEIMPORTERHDF_H
