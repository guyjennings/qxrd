#ifndef QCEPDATAOBJECTWINDOW_H
#define QCEPDATAOBJECTWINDOW_H

#include "qcepmainwindow.h"

class QcepDataObjectWindow : public QcepMainWindow
{
  Q_OBJECT
public:
  explicit QcepDataObjectWindow(QWidget *parent = 0);

signals:

public slots:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
};

#endif // QCEPDATAOBJECTWINDOW_H
