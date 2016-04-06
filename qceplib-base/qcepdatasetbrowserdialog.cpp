#include "qcepdatasetbrowserdialog.h"

QcepDatasetBrowserDialog::QcepDatasetBrowserDialog(QcepExperimentWPtr expt, QcepDatasetModelPtr ds, QWidget *parent) :
  QDockWidget(parent),
  m_Experiment(expt),
  m_DatasetModel(ds)
{
  setupUi(this);

  m_DatasetBrowserView->setExperiment(m_Experiment);

  if (m_DatasetModel) {
    m_DatasetBrowserView -> setDatasetModel(m_DatasetModel);
  }
}

QcepDatasetBrowserDialog::~QcepDatasetBrowserDialog()
{
}

