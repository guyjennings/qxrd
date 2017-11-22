#include "qxrdwelcomerecentitem.h"
#include "ui_qxrdwelcomerecentitem.h"

QxrdWelcomeRecentItem::QxrdWelcomeRecentItem(QWidget *parent) :
    QCommandLinkButton(parent)
{
    setupUi(this);
}

QxrdWelcomeRecentItem::~QxrdWelcomeRecentItem()
{
}

void QxrdWelcomeRecentItem::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
