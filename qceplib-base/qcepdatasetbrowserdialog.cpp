#include "qcepdatasetbrowserdialog.h"

QcepDatasetBrowserDialog::QcepDatasetBrowserDialog(QcepExperimentWPtr expt, QcepDatasetModelWPtr ds, QWidget *parent) :
  inherited(parent),
  m_Experiment(expt),
  m_DatasetModel(ds)
{
  setupUi(this);

  m_DatasetBrowserView -> setExperiment(m_Experiment);
  m_DatasetBrowserView -> setDatasetModel(m_DatasetModel);
}

QcepDatasetBrowserDialog::~QcepDatasetBrowserDialog()
{
}

