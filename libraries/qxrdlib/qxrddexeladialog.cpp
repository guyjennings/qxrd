#include "qxrddexeladialog.h"
#include "ui_qxrddexeladialog.h"

QxrdDexelaDialog::QxrdDexelaDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QxrdDexelaDialog)
{
  ui->setupUi(this);
}

QxrdDexelaDialog::~QxrdDexelaDialog()
{
  delete ui;
}
