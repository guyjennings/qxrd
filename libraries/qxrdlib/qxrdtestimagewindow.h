#ifndef QXRDTESTIMAGEWINDOW_H
#define QXRDTESTIMAGEWINDOW_H

#include "qxrdlib_global.h"
#include <QMainWindow>
#include "qcepimagedata-ptr.h"

class QXRD_EXPORT QxrdTestImageWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit QxrdTestImageWindow(QWidget *parent = nullptr);

public slots:
//  virtual void onNewImageAvailable(QcepDoubleImageDataPtr data) = 0;
};

#endif // QXRDTESTIMAGEWINDOW_H