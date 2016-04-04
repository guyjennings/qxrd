#include "qcepdataimportdialog.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"
#include "qcepdataimportparameters.h"

QcepDataImportDialog::QcepDataImportDialog(QcepDatasetModelPtr indata,
                                           const QModelIndexList &inselect,
                                           QcepDatasetModelPtr destdata,
                                           const QModelIndexList &destselect,
                                           QStringList files,
                                           QcepExperimentPtr expt,
                                           QcepDataImportParametersPtr parms) :
  QDialog(),
  m_InData(indata),
  m_InSelect(inselect),
  m_DestData(destdata),
  m_DestSelect(destselect),
  m_Files(files),
  m_Experiment(expt),
  m_Parameters(parms)
{
  setupUi(this);

  if (m_InData) {
    m_ImportedData->setModel(m_InData.data());

    foreach (QModelIndex idx, m_InSelect) {
      m_ImportedData->selectionModel()->select(idx, QItemSelectionModel::Select);

      QModelIndex i = idx;

      while (i.isValid()) {
        m_ImportedData->setExpanded(i, true);
        i = m_InData->parent(i);
      }
    }
  }

  if (m_DestData) {
    m_Destinations->setModel(m_DestData.data());

    QModelIndex i = m_DestSelect.value(0);

    m_Destinations->selectionModel()->select(i, QItemSelectionModel::Select);

    while (i.isValid()) {
      m_Destinations->setExpanded(i, true);
      i = m_DestData->parent(i);
    }
  }
}

QcepDataImportDialog::~QcepDataImportDialog()
{
}

void QcepDataImportDialog::accept()
{
  QDialog::accept();
}
