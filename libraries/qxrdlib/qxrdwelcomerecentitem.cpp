#include "qxrdwelcomerecentitem.h"
#include "ui_qxrdwelcomerecentitem.h"

QxrdWelcomeRecentItem::QxrdWelcomeRecentItem(QWidget *parent) :
    QCommandLinkButton(parent),
    ui(new Ui::QxrdWelcomeRecentItem)
{
    ui->setupUi(this);
}

QxrdWelcomeRecentItem::~QxrdWelcomeRecentItem()
{
    delete ui;
}

void QxrdWelcomeRecentItem::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
