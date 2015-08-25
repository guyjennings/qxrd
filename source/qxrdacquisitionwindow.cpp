#include "qxrdacquisitionwindow.h"
#include "ui_qxrdacquisitionwindow.h"
#include "qxrdacquisition.h"

QxrdAcquisitionWindow::QxrdAcquisitionWindow(QxrdAcquisitionWPtr acq, QWidget *parent) :
  QxrdMainWindow(parent),
  m_Acquisition(acq)
{
  setupUi(this);
}

QxrdAcquisitionWindow::~QxrdAcquisitionWindow()
{
#ifndef QT_NO_DEBUG
  printf("Deleting detector control window\n");
#endif
}

void QxrdAcquisitionWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdAcquisitionWindow::printMessage(QString msg, QDateTime ts)
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq->printMessage(msg, ts);
  }
}
