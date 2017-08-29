#ifndef QXRDTESTSCANWINDOWQWT_H
#define QXRDTESTSCANWINDOWQWT_H

#include "qxrdlib_global.h"
#include "qxrdtestscanwindow.h"
#include "ui_qxrdtestscanwindowqwt.h"
#include "qxrdtestscanplotqwthelper-ptr.h"

class QXRD_EXPORT QxrdTestScanWindowQwt : public QxrdTestScanWindow, public Ui::QxrdTestScanWindowQwt
{
  Q_OBJECT

public:
  explicit QxrdTestScanWindowQwt(QWidget *parent = 0);
  ~QxrdTestScanWindowQwt();

  QxrdTestScanPlotQwtHelperPtr helper();

private:
};

#endif // QXRDTESTSCANWINDOWQWT_H
