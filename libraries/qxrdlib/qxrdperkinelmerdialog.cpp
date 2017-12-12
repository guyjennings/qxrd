#include "qxrdperkinelmerdialog.h"
#include "ui_qxrdperkinelmerdialog.h"

QxrdPerkinElmerDialog::QxrdPerkinElmerDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QxrdPerkinElmerDialog)
{
  ui->setupUi(this);
}

QxrdPerkinElmerDialog::~QxrdPerkinElmerDialog()
{
  delete ui;
}
