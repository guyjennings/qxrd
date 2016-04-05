#include "qcepdataimportcommand.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"
#include "qcepdataimportparameters.h"
#include "qcepdataimportdialog.h"
#include "qcepfileimporter.h"

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

      m_FileImporter = QcepFileImporter::importFiles(m_ImportedDataset,
                                                     m_ImportedIndexes,
                                                     files);
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

  if (dlog.exec()) {
    res = true;
  }

  return false;
}
