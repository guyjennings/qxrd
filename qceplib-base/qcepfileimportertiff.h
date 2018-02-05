#ifndef QCEPFILEIMPORTERTIFF_H
#define QCEPFILEIMPORTERTIFF_H

#include "qceplib_global.h"
#include "qcepfileimporter.h"

class QCEP_EXPORT QcepFileImporterTIFF : public QcepFileImporter
{
  Q_OBJECT

private:
  typedef QcepFileImporter inherited;

public:
  QcepFileImporterTIFF(QcepDatasetModelPtr model,
                       QModelIndexList &indexes,
                       QString path);
};

#endif // QCEPFILEIMPORTERTIFF_H
