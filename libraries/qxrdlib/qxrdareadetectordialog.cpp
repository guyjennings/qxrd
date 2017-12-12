#include "qxrdareadetectordialog.h"
#include "ui_qxrdareadetectordialog.h"

QxrdAreaDetectorDialog::QxrdAreaDetectorDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QxrdAreaDetectorDialog)
{
  ui->setupUi(this);
}

QxrdAreaDetectorDialog::~QxrdAreaDetectorDialog()
{
  delete ui;
}
