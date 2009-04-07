#ifndef QXRDCENTERFINDERDIALOG_H
#define QXRDCENTERFINDERDIALOG_H

#include <QGroupBox>
#include "ui_qxrdcenterfinderdialog.h"

class QxrdCenterFinderDialog : public QGroupBox, public Ui::QxrdCenterFinderDialog
{
  Q_OBJECT;

public:
    QxrdCenterFinderDialog(QWidget *parent=0);
};

#endif // QXRDCENTERFINDERDIALOG_H
