#ifndef QXRDTESTIMAGEWINDOWDATAVIS_H
#define QXRDTESTIMAGEWINDOWDATAVIS_H

#ifdef HAVE_DATAVIS

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

#endif // HAVE_DATAVIS

#endif // QXRDTESTIMAGEWINDOWDATAVIS_H