#ifndef QXRDTESTIMAGEWINDOWDATAVIS_H
#define QXRDTESTIMAGEWINDOWDATAVIS_H

#include "qxrdlib_global.h"
#include "qxrdtestimagewindow.h"
#include "ui_qxrdtestimagewindowdatavis.h"
#include "qxrdtestimageplotdatavishelper-ptr.h"

class QXRD_EXPORT QxrdTestImageWindowDataVis : public QxrdTestImageWindow,
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
