#include "qcepdatasetbrowserdialog.h"
#include "ui_qcepdatasetbrowserdialog.h"
#include "qcepdataset.h"
#include "qcepdatasetmodel.h"

QcepDatasetBrowserDialog::QcepDatasetBrowserDialog(QcepDatasetPtr ds, QWidget *parent) :
  QDockWidget(parent),
  m_Dataset(ds)
{
  setupUi(this);

  if (m_Dataset) {
    m_DatasetModel = QcepDatasetModelPtr(
          new QcepDatasetModel(m_Dataset));

    m_DatasetBrowserView -> setModel(m_DatasetModel.data());
  }
}

QcepDatasetBrowserDialog::~QcepDatasetBrowserDialog()
{
}
