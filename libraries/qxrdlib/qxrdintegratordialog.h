#ifndef QXRDINTEGRATORDIALOG_H
#define QXRDINTEGRATORDIALOG_H

#include <QDockWidget>
#include "ui_qxrdintegratordialog.h"

#include "qxrdintegrator.h"
#include "qxrduserscriptdialog.h"
#include <QPointer>

class QxrdIntegratorDialog : public QDockWidget, public Ui::QxrdIntegratorDialog
{
  Q_OBJECT

public:
  QxrdIntegratorDialog(QxrdIntegratorWPtr integw, QWidget *parent=0);
  virtual ~QxrdIntegratorDialog();

public slots:
  void onEnablePolarizationChanged(bool pol);
  void onEnableAbsorptionChanged(bool absn);

  void editUserGeometry();
  void editUserAbsorption();
  void onEditedUserGeometry();
  void onEditedUserAbsorption();

private:
  QxrdIntegratorWPtr m_Integrator;
  QString            m_GeometryPath;
  QString            m_IntensityPath;
  QPointer<QxrdUserScriptDialog> m_EditUserAbsorption;
  QPointer<QxrdUserScriptDialog> m_EditUserGeometry;
};

#endif // QXRDINTEGRATORDIALOG_H
