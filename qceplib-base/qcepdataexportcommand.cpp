#include "qcepdataexportcommand.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"
#include "qcepdataexportparameters.h"
#include "qcepdataexportdialog.h"

QcepDataExportCommand::QcepDataExportCommand(QcepDatasetModelWPtr model, const QModelIndexList &idx, QString file) :
  inherited("dataExport"),
  m_Model(model),
  m_Indexes(idx),
  m_File(file)
{
  QcepDatasetModelPtr m(m_Model);

  if (m) {
    m_Experiment = m->experiment();

    QcepExperimentPtr expt(m_Experiment);

    if (expt) {
      m_Parameters = expt->dataExportParameters();
    }
  }
}

bool QcepDataExportCommand::exec()
{
  bool res = false;
  QcepDataExportDialog dlog(m_Model,
                            m_Indexes,
                            m_File,
                            m_Experiment,
                            m_Parameters);

  if (dlog.exec()) {
    res = true;
  }

  return res;
}
