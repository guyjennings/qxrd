#include "qxrdtestdockwidget.h"
#include "ui_qxrdtestdockwidget.h"

QxrdTestDockWidget::QxrdTestDockWidget(QWidget *parent) :
    QDockWidget(parent)
{
  setupUi(this);
}

QxrdTestDockWidget::~QxrdTestDockWidget()
{
}

void QxrdTestDockWidget::changeEvent(QEvent *e)
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
