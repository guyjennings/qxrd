#include "qcepnewdataarraydialog.h"
#include "ui_qcepnewdataarraydialog.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"

QcepNewDataArrayDialog::QcepNewDataArrayDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  QDialog(),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    setWindowTitle(tr("Create new data array in %1").arg(m_Model->pathName(idx)));

    QcepExperimentPtr expt = m_Model -> experiment();

    if (expt) {
      m_ArrayName     -> setText(expt->get_NewArrayName());
      m_NumDimensions -> setValue(expt->get_NewArrayNumDimensions());
      m_XDimension    -> setValue(expt->get_NewArrayXDimension());
      m_YDimension    -> setValue(expt->get_NewArrayYDimension());
      m_ZDimension    -> setValue(expt->get_NewArrayZDimension());
      m_TDimension    -> setValue(expt->get_NewArrayTDimension());
    }
  }
}

QcepNewDataArrayDialog::~QcepNewDataArrayDialog()
{
}

void QcepNewDataArrayDialog::accept()
{
  if (m_Model) {
    QcepExperimentPtr expt = m_Model -> experiment();

    if (expt) {
      QString newArrayName          = m_ArrayName -> text();
      int     newArrayNumDimensions = m_NumDimensions -> value();
      int     newArrayXDimension    = m_XDimension -> value();
      int     newArrayYDimension    = m_YDimension -> value();
      int     newArrayZDimension    = m_ZDimension -> value();
      int     newArrayTDimension    = m_TDimension -> value();

      expt -> set_NewArrayName(newArrayName);
      expt -> set_NewArrayNumDimensions(newArrayNumDimensions);
      expt -> set_NewArrayXDimension(newArrayXDimension);
      expt -> set_NewArrayYDimension(newArrayYDimension);
      expt -> set_NewArrayZDimension(newArrayZDimension);
      expt -> set_NewArrayTDimension(newArrayTDimension);

      QVector<int> dims;

      if (newArrayNumDimensions >= 1) {
        dims << newArrayXDimension;
      }

      if (newArrayNumDimensions >= 2) {
        dims << newArrayYDimension;
      }

      if (newArrayNumDimensions >= 3) {
        dims << newArrayZDimension;
      }

      if (newArrayNumDimensions >= 4) {
        dims << newArrayTDimension;
      }

      m_Model -> newArray(m_Index, newArrayName, dims);
    }
  }

  QDialog::accept();
}
