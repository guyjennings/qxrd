#ifndef QXRDTESTSCANWINDOWDATAVIS_H
#define QXRDTESTSCANWINDOWDATAVIS_H

#include "qxrdlib_global.h"
#include "qxrdtestscanwindow.h"
#include "ui_qxrdtestscanwindowdatavis.h"
#include "qxrdtestscanplotdatavishelper-ptr.h"

class QXRD_EXPORT QxrdTestScanWindowDataVis : public QxrdTestScanWindow, public Ui::QxrdTestScanWindowDataVis
{
  Q_OBJECT

public:
  explicit QxrdTestScanWindowDataVis(QWidget *parent = 0);
  ~QxrdTestScanWindowDataVis();

public:
  QxrdTestScanPlotDataVisHelperPtr helper();

private:
};

#endif // QXRDTESTSCANWINDOWDATAVIS_H
