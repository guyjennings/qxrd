#ifndef QXRDINTEGRATORDIALOG_H
#define QXRDINTEGRATORDIALOG_H

#include <QGroupBox>
#include "ui_qxrdintegratordialog.h"

class QxrdIntegratorDialog : public QGroupBox, public Ui::QxrdIntegratorDialog
{
  Q_OBJECT;

public:
    QxrdIntegratorDialog(QWidget *parent=0);
};

#endif // QXRDINTEGRATORDIALOG_H
