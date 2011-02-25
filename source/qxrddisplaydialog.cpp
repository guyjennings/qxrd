#include "qxrddisplaydialog.h"

QxrdDisplayDialog::QxrdDisplayDialog(QWidget *parent) :
    QDockWidget(parent)
{
    setupUi(this);
}

QxrdDisplayDialog::~QxrdDisplayDialog()
{
}

void QxrdDisplayDialog::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
