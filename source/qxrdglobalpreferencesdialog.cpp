#include "qxrdglobalpreferencesdialog.h"
#include "ui_qxrdglobalpreferencesdialog.h"

QxrdGlobalPreferencesDialog::QxrdGlobalPreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QxrdGlobalPreferencesDialog)
{
    ui->setupUi(this);
}

QxrdGlobalPreferencesDialog::~QxrdGlobalPreferencesDialog()
{
    delete ui;
}

void QxrdGlobalPreferencesDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
