#ifndef QXRDINTEGRATORDIALOG_H
#define QXRDINTEGRATORDIALOG_H

#include "qcepmacros.h"

#include <QGroupBox>
#include "qxrdforwardtypes.h"
#include "ui_qxrdintegratordialog.h"

class QxrdIntegratorDialog : public QWidget, public Ui::QxrdIntegratorDialog
{
  Q_OBJECT;

public:
  QxrdIntegratorDialog(QxrdIntegratorPtr integ, QWidget *parent=0);

private:
  QxrdIntegratorPtr m_Integrator;
};

#endif // QXRDINTEGRATORDIALOG_H
