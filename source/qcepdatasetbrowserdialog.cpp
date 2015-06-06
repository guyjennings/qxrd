#include "qcepdatasetbrowserdialog.h"
#include "ui_qcepdatasetbrowserdialog.h"

QcepDatasetBrowserDialog::QcepDatasetBrowserDialog(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::QcepDatasetBrowserDialog)
{
  ui->setupUi(this);
}

QcepDatasetBrowserDialog::~QcepDatasetBrowserDialog()
{
  delete ui;
}
