#ifndef QXRDTESTIMAGEWINDOWDATAVIS_H
#define QXRDTESTIMAGEWINDOWDATAVIS_H

#include "qxrdtestimagewindow.h"
#include "ui_qxrdtestimagewindowdatavis.h"
#include "qxrdtestimageplotdatavishelper-ptr.h"

class QxrdTestImageWindowDataVis : public QxrdTestImageWindow,
                                   public Ui::QxrdTestImageWindowDataVis
{
  Q_OBJECT

public:
  explicit QxrdTestImageWindowDataVis(QWidget *parent = 0);
  ~QxrdTestImageWindowDataVis();

  QxrdTestImagePlotDataVisHelperPtr helper();

private:
};

#endif // QXRDTESTIMAGEWINDOWDATAVIS_H
