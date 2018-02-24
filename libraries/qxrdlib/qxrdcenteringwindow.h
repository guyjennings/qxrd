#ifndef QXRDCENTERINGWINDOW_H
#define QXRDCENTERINGWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdcenteringwindow.h"
#include "qxrdcenteringwindowsettings-ptr.h"

//TODO: implement
class QXRD_EXPORT QxrdCenteringWindow : public QxrdMainWindow, public Ui::QxrdCenteringWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdCenteringWindow(QString name);
  void initialize(QcepObjectWPtr parent);
  ~QxrdCenteringWindow();

protected:
  void changeEvent(QEvent *e);
  void onImplementTiltChanged(bool imp);
  void onStepSizesChanged();

private:
};

#endif // QXRDCENTERINGWINDOW_H
