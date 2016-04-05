#ifndef QCEPFILEIMPORTERHDF_H
#define QCEPFILEIMPORTERHDF_H

#include "qcepfileimporter.h"

class QcepFileImporterHDF : public QcepFileImporter
{
  Q_OBJECT

public:
  QcepFileImporterHDF(QcepDatasetModelPtr model,
                      QModelIndexList &indexes,
                      QString path);
};

#endif // QCEPFILEIMPORTERHDF_H
