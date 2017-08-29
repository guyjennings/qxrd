#ifndef QXRDTESTSCANWINDOWQWT_H
#define QXRDTESTSCANWINDOWQWT_H

#include "qxrdtestscanwindow.h"
#include "ui_qxrdtestscanwindowqwt.h"
#include "qxrdtestscanplotqwthelper-ptr.h"

class QxrdTestScanWindowQwt : public QxrdTestScanWindow, public Ui::QxrdTestScanWindowQwt
{
  Q_OBJECT

public:
  explicit QxrdTestScanWindowQwt(QWidget *parent = 0);
  ~QxrdTestScanWindowQwt();

  QxrdTestScanPlotQwtHelperPtr helper();

private:
};

#endif // QXRDTESTSCANWINDOWQWT_H
