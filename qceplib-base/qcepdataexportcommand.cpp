#include "qcepdataexportcommand.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"
#include "qcepdataexportparameters.h"
#include "qcepdataexportdialog.h"

QcepDataExportCommand::QcepDataExportCommand(QcepDatasetModelPtr model, const QModelIndexList &idx, QString file) :
  QcepObject("dataExport", NULL),
  m_Model(model),
  m_Indexes(idx),
  m_File(file)
{
  if (m_Model) {
    m_Experiment = m_Model->experiment();

    if (m_Experiment) {
      m_Parameters = m_Experiment->dataExportParameters();
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
