#include "qxrdacquirefilewatcherdialog.h"
#include "ui_qxrdacquirefilewatcherdialog.h"

QxrdAcquireFileWatcherDialog::QxrdAcquireFileWatcherDialog(QxrdExperimentWPtr doc,
                                                           QxrdWindow *win,
                                                           QxrdAcquisitionWPtr acq,
                                                           QxrdDataProcessorWPtr proc,
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
