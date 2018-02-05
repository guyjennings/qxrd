#include "qcepdataexportdialog.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"
#include "qcepdataexportparameters.h"

QcepDataExportDialog::QcepDataExportDialog(QcepDatasetModelPtr model,
                                           const QModelIndexList &idx,
                                           QString file,
                                           QcepExperimentPtr expt,
                                           QcepDataExportParametersPtr parms) :
  inherited(),
  m_Model(model),
  m_Indexes(idx),
  m_File(file),
  m_Experiment(expt),
  m_Parameters(parms)
{
  setupUi(this);

  if (m_Model) {
    m_ExportedData->setModel(m_Model.data());

    foreach (QModelIndex idx, m_Indexes) {
      m_ExportedData->selectionModel()->select(idx, QItemSelectionModel::Select);

      QModelIndex i = idx;

      while (i.isValid()) {
        m_ExportedData->setExpanded(i, true);
        i = m_Model->parent(i);
      }
    }
  }
}

QcepDataExportDialog::~QcepDataExportDialog()
{
}

void QcepDataExportDialog::accept()
{
  inherited::accept();
}
