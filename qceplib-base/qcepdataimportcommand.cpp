#include "qcepdataimportcommand.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"
#include "qcepdataimportparameters.h"
#include "qcepdataimportdialog.h"
#include "qcepfileimporter.h"
#include <QtConcurrentRun>

QcepDataImportCommand::QcepDataImportCommand(QcepDatasetModelPtr model, const QModelIndexList &idx, QStringList files) :
  QcepObject("dataExport", NULL),
  m_Model(model),
  m_Indexes(idx),
  m_Files(files)
{
  if (m_Model) {
    m_Experiment = m_Model->experiment();

    if (m_Experiment) {
      m_Parameters = m_Experiment->dataImportParameters();

      m_ImportedData = QcepDatasetPtr(new QcepDataset(QcepSettingsSaverWPtr(), "import", NULL));
      m_ImportedDataset = QcepDatasetModelPtr(
            new QcepDatasetModel(m_Experiment, QcepDataProcessorBaseWPtr(), m_ImportedData));

      if (files.count() == 1) {
        m_FileImporter = QcepFileImporter::importFile(m_ImportedDataset,
                                                      m_ImportedIndexes,
                                                      files.value(0));

      } else {
        m_FileImporter = QcepFileImporter::importFiles(m_ImportedDataset,
                                                       m_ImportedIndexes,
                                                       files);
      }
    }
  }
}

bool QcepDataImportCommand::exec()
{
  bool res = false;
  QcepDataImportDialog dlog(m_ImportedDataset,
                            m_ImportedIndexes,
                            m_Model,
                            m_Indexes,
                            m_Files,
                            m_Experiment,
                            m_Parameters);

  if (m_FileImporter) {
    connect(m_FileImporter.data(), &QcepFileImporter::importProgress,
            &dlog, &QcepDataImportDialog::importProgress);

    connect(m_FileImporter.data(), &QcepFileImporter::importCompleted,
            &dlog, &QcepDataImportDialog::importCompleted);
  }

  QtConcurrent::run(m_FileImporter.data(), &QcepFileImporter::exec);

  if (dlog.exec() == QDialog::Accepted) {
    copyResults();

    res = true;
  }

  return false;
}

void QcepDataImportCommand::copyResults()
{
  foreach (QModelIndex idx, m_ImportedIndexes) {
    QcepDataObjectPtr obj = m_ImportedDataset->indexedObject(idx);

    m_Model->append(m_Indexes.value(0), obj);
  }
}
