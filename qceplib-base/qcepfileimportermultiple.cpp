#include "qcepfileimportermultiple.h"

QcepFileImporterMultiple::QcepFileImporterMultiple(QcepDatasetModelPtr model,
                                                   QModelIndexList &indexes) :
  inherited(model, indexes, ""),
  m_Progress()
{
}

void QcepFileImporterMultiple::append(QcepFileImporterPtr importer)
{
  m_Importers.append(importer);

  connect(importer.data(), &QcepFileImporter::importProgress,
          this, &QcepFileImporterMultiple::individualProgress);
}

void QcepFileImporterMultiple::exec()
{
  m_Progress.resize(m_Importers.count());
  m_Progress.fill(0);

  emit importStarted();

  foreach(QcepFileImporterPtr importer, m_Importers) {
    if (importer) {
      importer->exec();
    }
  }

  emit importCompleted();
}

void QcepFileImporterMultiple::individualProgress(double progress)
{
  QcepFileImporter* obj = qobject_cast<QcepFileImporter*>(sender());

  if (obj) {
    for (int i=0; i<m_Progress.count(); i++) {
      QcepFileImporterPtr importer = m_Importers.value(i);

      if (importer && (importer.data() == obj)) {
        m_Progress[i] = progress;
      }
    }

    double sum = 0;

    for (int i=0; i<m_Progress.count(); i++) {
      sum += m_Progress.value(i);
    }

    emit importProgress(sum/m_Progress.count());
  }
}
