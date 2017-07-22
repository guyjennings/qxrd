#ifndef QXRDTESTIMAGEWINDOWQWT_H
#define QXRDTESTIMAGEWINDOWQWT_H

#include "qxrdtestimagewindow.h"
#include "ui_qxrdtestimagewindowqwt.h"

class QxrdTestImageWindowQwt : public QxrdTestImageWindow, public Ui::QxrdTestImageWindowQwt
{
  Q_OBJECT

public:
  explicit QxrdTestImageWindowQwt(QWidget *parent = 0);
  ~QxrdTestImageWindowQwt();

public slots:
  virtual void onNewImageAvailable(QcepDoubleImageDataPtr data);

private:
};

#endif // QXRDTESTIMAGEWINDOWQWT_H
