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
  virtual void printLine(QString line);
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  virtual void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
};

#endif // QCEPDATAOBJECTWINDOW_H
