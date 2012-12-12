#include "qxrddisplaydialog.h"
#include "qxrddebug.h"
#include <stdio.h>

QxrdDisplayDialog::QxrdDisplayDialog(QWidget *parent) :
    QDockWidget(parent)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDisplayDialog::QxrdDisplayDialog(%p)\n", this);
  }

  setupUi(this);
}

QxrdDisplayDialog::~QxrdDisplayDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDisplayDialog::~QxrdDisplayDialog(%p)\n", this);
  }
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
