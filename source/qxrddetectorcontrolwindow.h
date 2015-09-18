#ifndef QXRDDETECTORCONTROLWINDOW_H
#define QXRDDETECTORCONTROLWINDOW_H

#include "qxrdmainwindow.h"
#include "ui_qxrddetectorcontrolwindow.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddetectorprocessor-ptr.h"

class QxrdDetectorControlWindow : public QxrdMainWindow, public Ui::QxrdDetectorControlWindow
{
  Q_OBJECT

public:
  explicit QxrdDetectorControlWindow(QxrdAcquisitionWPtr       acq,
                                     QxrdDetectorProcessorWPtr proc,
                                     QWidget                  *parent = 0);
  ~QxrdDetectorControlWindow();

public slots:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

protected:
  void changeEvent(QEvent *e);

private:
  QxrdAcquisitionWPtr       m_Acquisition;
  QxrdDetectorProcessorWPtr m_Processor;
};

#endif // QXRDDETECTORCONTROLWINDOW_H
