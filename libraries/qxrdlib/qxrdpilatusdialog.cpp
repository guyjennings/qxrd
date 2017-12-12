#include "qxrdpilatusdialog.h"
#include "ui_qxrdpilatusdialog.h"

QxrdPilatusDialog::QxrdPilatusDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QxrdPilatusDialog)
{
  ui->setupUi(this);
}

QxrdPilatusDialog::~QxrdPilatusDialog()
{
  delete ui;
}
