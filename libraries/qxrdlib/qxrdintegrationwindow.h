#ifndef QXRDINTEGRATIONWINDOW_H
#define QXRDINTEGRATIONWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdintegrationwindow.h"
#include "qxrdintegrator-ptr.h"
#include "qxrduserscriptdialog.h"
#include <QPointer>
#include "qxrdintegrationwindowsettings-ptr.h"

//TODO: implement
class QXRD_EXPORT QxrdIntegrationWindow : public QxrdMainWindow, public Ui::QxrdIntegrationWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdIntegrationWindow(QString name);
  void initialize(QcepObjectWPtr parent);
  ~QxrdIntegrationWindow();

protected:
  void changeEvent(QEvent *e);

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

#endif // QXRDINTEGRATIONWINDOW_H
