#ifndef QXRDACQUISITIONWINDOW_H
#define QXRDACQUISITIONWINDOW_H

#include "qxrdmainwindow.h"
#include "ui_qxrdacquisitionwindow.h"
#include "qxrdacquisition-ptr.h"

class QxrdAcquisitionWindow : public QxrdMainWindow, public Ui::QxrdAcquisitionWindow
{
  Q_OBJECT

public:
  explicit QxrdAcquisitionWindow(QxrdAcquisitionWPtr acq, QWidget *parent = 0);
  ~QxrdAcquisitionWindow();

public slots:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

protected:
  void changeEvent(QEvent *e);

private:
  QxrdAcquisitionWPtr m_Acquisition;
};

#endif // QXRDACQUISITIONWINDOW_H
