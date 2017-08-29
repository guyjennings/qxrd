#ifndef QXRDTESTIMAGEWINDOWQWT_H
#define QXRDTESTIMAGEWINDOWQWT_H

#include "qxrdtestimagewindow.h"
#include "ui_qxrdtestimagewindowqwt.h"
#include "qxrdtestimageplotqwthelper-ptr.h"

class QxrdTestImageWindowQwt : public QxrdTestImageWindow, public Ui::QxrdTestImageWindowQwt
{
  Q_OBJECT

public:
  explicit QxrdTestImageWindowQwt(QWidget *parent = 0);
  ~QxrdTestImageWindowQwt();

  QxrdTestImagePlotQwtHelperPtr helper();

private:
  QxrdImagePlotSettingsPtr m_GraphSettings;
};

#endif // QXRDTESTIMAGEWINDOWQWT_H
