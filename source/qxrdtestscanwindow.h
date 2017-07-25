#ifndef QXRDTESTSCANWINDOW_H
#define QXRDTESTSCANWINDOW_H

#include <QMainWindow>
#include "qcepdatacolumnscan-ptr.h"

class QxrdTestScanWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit QxrdTestScanWindow(QWidget *parent = 0);
};

#endif // QXRDTESTSCANWINDOW_H
