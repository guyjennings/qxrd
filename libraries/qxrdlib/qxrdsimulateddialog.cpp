#include "qxrdsimulateddialog.h"
#include "ui_qxrdsimulateddialog.h"

QxrdSimulatedDialog::QxrdSimulatedDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QxrdSimulatedDialog)
{
  ui->setupUi(this);
}

QxrdSimulatedDialog::~QxrdSimulatedDialog()
{
  delete ui;
}
