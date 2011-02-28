#ifndef QXRDINTEGRATORDIALOG_H
#define QXRDINTEGRATORDIALOG_H

#include <QDockWidget>
#include "ui_qxrdintegratordialog.h"

class QxrdIntegrator;

class QxrdIntegratorDialog : public QDockWidget, public Ui::QxrdIntegratorDialog
{
  Q_OBJECT;

public:
  QxrdIntegratorDialog(QxrdIntegrator *integ, QWidget *parent=0);

private:
  QxrdIntegrator *m_Integrator;
};

#endif // QXRDINTEGRATORDIALOG_H
