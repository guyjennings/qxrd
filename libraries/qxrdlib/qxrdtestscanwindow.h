#ifndef QXRDTESTSCANWINDOW_H
#define QXRDTESTSCANWINDOW_H

#include "qxrdlib_global.h"
#include <QMainWindow>
#include "qcepdatacolumnscan-ptr.h"

class QXRD_EXPORT QxrdTestScanWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit QxrdTestScanWindow(QWidget *parent = 0);
};

#endif // QXRDTESTSCANWINDOW_H
