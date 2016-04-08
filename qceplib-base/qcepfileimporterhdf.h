#ifndef QCEPFILEIMPORTERHDF_H
#define QCEPFILEIMPORTERHDF_H

#include "qcepfileimporter.h"
#include "hdf5.h"

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
  void scanDataset(QModelIndex dest, hid_t dsid);
  void scanLink(QModelIndex dest, hid_t gid, char *name);
};

#endif // QCEPFILEIMPORTERHDF_H
