#ifndef QXRDCENTERFINDERDIALOG_H
#define QXRDCENTERFINDERDIALOG_H

#include <QGroupBox>
#include "ui_qxrdcenterfinderdialog.h"

class QxrdCenterFinderDialog : public QGroupBox, public Ui::QxrdCenterFinderDialog
{
  Q_OBJECT;

public:
  QxrdCenterFinderDialog(QWidget *parent=0);

public slots:
  void onCenterChanged(double cx, double cy);

private:
};

#endif // QXRDCENTERFINDERDIALOG_H
