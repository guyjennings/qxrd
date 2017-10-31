#ifndef QXRDCALIBRANTWINDOW_H
#define QXRDCALIBRANTWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdcalibrantwindow.h"

class QXRD_EXPORT QxrdCalibrantWindow : public QxrdMainWindow, public Ui::QxrdCalibrantWindow
{
  Q_OBJECT

public:
  explicit QxrdCalibrantWindow(QWidget *parent = 0);
  ~QxrdCalibrantWindow();

protected:
  void changeEvent(QEvent *e);
};

#endif // QXRDCALIBRANTWINDOW_H
