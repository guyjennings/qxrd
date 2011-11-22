#include "qxrdacquirefilewatcherdialog.h"
#include "ui_qxrdacquirefilewatcherdialog.h"

QxrdAcquireFileWatcherDialog::QxrdAcquireFileWatcherDialog(QxrdDocument *doc,
                                                           QxrdWindow *win,
                                                           QxrdAcquisition *acq,
                                                           QxrdDataProcessor *proc,
                                                           QWidget *parent) :
    QxrdAcquireDialogBase(doc, win, acq, proc, parent)
{
  setupUi(this);
}

QxrdAcquireFileWatcherDialog::~QxrdAcquireFileWatcherDialog()
{
}

void QxrdAcquireFileWatcherDialog::changeEvent(QEvent *e)
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

void QxrdAcquireFileWatcherDialog::setupAcquireMenu(QMenu *menu)
{
}

void QxrdAcquireFileWatcherDialog::onAcquisitionInit()
{
}

void QxrdAcquireFileWatcherDialog::acquisitionReady()
{
}
