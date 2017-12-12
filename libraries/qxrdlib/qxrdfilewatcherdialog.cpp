#include "qxrdfilewatcherdialog.h"
#include "ui_qxrdfilewatcherdialog.h"

QxrdFileWatcherDialog::QxrdFileWatcherDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QxrdFileWatcherDialog)
{
  ui->setupUi(this);
}

QxrdFileWatcherDialog::~QxrdFileWatcherDialog()
{
  delete ui;
}
