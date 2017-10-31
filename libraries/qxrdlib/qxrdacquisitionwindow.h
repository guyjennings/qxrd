#ifndef QXRDACQUISITIONWINDOW_H
#define QXRDACQUISITIONWINDOW_H

#include "qcepdebug.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdaquisitionwindow.h"

class QXRD_EXPORT QxrdAcquisitionWindow : public QxrdMainWindow, public Ui::QxrdAquisitionWindow
{
  Q_OBJECT

public:
  explicit QxrdAcquisitionWindow(QWidget *parent = 0);
  ~QxrdAcquisitionWindow();

protected:
  void changeEvent(QEvent *e);
};

#endif // QXRDACQUISITIONWINDOW_H
