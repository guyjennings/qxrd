#ifndef QXRDHELPWINDOW_H
#define QXRDHELPWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdhelpwindow.h"
#include "qxrdhelpwindowsettings-ptr.h"

//TODO: implement
class QXRD_EXPORT QxrdHelpWindow : public QxrdMainWindow, public Ui::QxrdHelpWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdHelpWindow(QString name);
  void initialize(QcepObjectWPtr parent);
  ~QxrdHelpWindow();

protected:
  void changeEvent(QEvent *e);
};

#endif // QXRDHELPWINDOW_H
