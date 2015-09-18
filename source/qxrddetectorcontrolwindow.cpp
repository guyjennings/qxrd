#include "qxrddetectorcontrolwindow.h"
#include "ui_qxrddetectorcontrolwindow.h"
#include "qxrdacquisition.h"

QxrdDetectorControlWindow::QxrdDetectorControlWindow(QxrdAcquisitionWPtr acq, QxrdDetectorProcessorWPtr proc, QWidget *parent) :
  QxrdMainWindow(parent),
  m_Acquisition(acq),
  m_Processor(proc)
{
  setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose, false);
}

QxrdDetectorControlWindow::~QxrdDetectorControlWindow()
{
#ifndef QT_NO_DEBUG
  printf("Deleting detector control window\n");
#endif
}

void QxrdDetectorControlWindow::changeEvent(QEvent *e)
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

void QxrdDetectorControlWindow::printMessage(QString msg, QDateTime ts)
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq->printMessage(msg, ts);
  }
}
