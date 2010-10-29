#include "qxrdacquiredialog.h"
#include "ui_qxrdacquiredialog.h"
#include "qxrdwindow.h"

QxrdAcquireDialog::QxrdAcquireDialog(QxrdWindowPtr win, QWidget *parent) :
    QWidget(parent),
    m_Window(win)
{
  setupUi(this);
}

QxrdAcquireDialog::~QxrdAcquireDialog()
{
}

void QxrdAcquireDialog::changeEvent(QEvent *e)
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
