#ifndef QXRDINTEGRATIONWINDOW_H
#define QXRDINTEGRATIONWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdintegrationwindow.h"

class QXRD_EXPORT QxrdIntegrationWindow : public QxrdMainWindow, public Ui::QxrdIntegrationWindow
{
  Q_OBJECT

public:
  explicit QxrdIntegrationWindow(QString name,
                                 QxrdApplicationPtr app,
                                 QxrdExperimentPtr expt);
  ~QxrdIntegrationWindow();

protected:
  void changeEvent(QEvent *e);
};

#endif // QXRDINTEGRATIONWINDOW_H
