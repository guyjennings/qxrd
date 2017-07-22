#ifndef QXRDTESTIMAGEWINDOWDATAVIS_H
#define QXRDTESTIMAGEWINDOWDATAVIS_H

#include "qxrdtestimagewindow.h"
#include "ui_qxrdtestimagewindowdatavis.h"

class QxrdTestImageWindowDataVis : public QxrdTestImageWindow, public Ui::QxrdTestImageWindowDataVis
{
  Q_OBJECT

public:
  explicit QxrdTestImageWindowDataVis(QWidget *parent = 0);
  ~QxrdTestImageWindowDataVis();

public slots:
  virtual void onNewImageAvailable(QcepDoubleImageDataPtr data);

private:
};

#endif // QXRDTESTIMAGEWINDOWDATAVIS_H
