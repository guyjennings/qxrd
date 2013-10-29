#ifndef QXRDINTEGRATORDIALOG_H
#define QXRDINTEGRATORDIALOG_H

#include <QDockWidget>
#include "ui_qxrdintegratordialog.h"

#include "qxrdintegrator.h"

class QxrdIntegratorDialog : public QDockWidget, public Ui::QxrdIntegratorDialog
{
  Q_OBJECT

public:
  QxrdIntegratorDialog(QxrdIntegratorWPtr integw, QWidget *parent=0);
  virtual ~QxrdIntegratorDialog();

private:
  QxrdIntegratorWPtr m_Integrator;
  QString            m_GeometryPath;
  QString            m_IntensityPath;
};

#endif // QXRDINTEGRATORDIALOG_H