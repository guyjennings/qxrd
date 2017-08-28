#ifndef QCEPFILEIMPORTERMULTIPLE_H
#define QCEPFILEIMPORTERMULTIPLE_H

#include "qceplib_global.h"
#include "qcepfileimporter.h"
#include <QVector>

class QCEP_EXPORT QcepFileImporterMultiple : public QcepFileImporter
{
  Q_OBJECT

public:
  QcepFileImporterMultiple(QcepDatasetModelPtr model,
                           QModelIndexList &indexes);

  void append(QcepFileImporterPtr importer);
  void exec();

private slots:
  void individualProgress(double progress);

private:
  QVector<QcepFileImporterPtr> m_Importers;
  QVector<double>              m_Progress;
};

#endif // QCEPFILEIMPORTERMULTIPLE_H
