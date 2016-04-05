#ifndef QCEPFILEIMPORTERTEXT_H
#define QCEPFILEIMPORTERTEXT_H

#include "qcepfileimporter.h"

class QcepFileImporterText : public QcepFileImporter
{
  Q_OBJECT

public:
  QcepFileImporterText(QcepDatasetModelPtr model,
                       QModelIndexList &indexes,
                       QString path);
};

#endif // QCEPFILEIMPORTERTEXT_H
