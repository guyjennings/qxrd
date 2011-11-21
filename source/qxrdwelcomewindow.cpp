#include "qxrdwelcomewindow.h"
#include "ui_qxrdwelcomewindow.h"

QxrdWelcomeWindow::QxrdWelcomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QxrdWelcomeWindow)
{
    ui->setupUi(this);
}

QxrdWelcomeWindow::~QxrdWelcomeWindow()
{
    delete ui;
}

void QxrdWelcomeWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
