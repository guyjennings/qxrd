#ifndef QXRDCALCULATORWINDOW_H
#define QXRDCALCULATORWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdcalculatorwindow.h"
#include "qxrdcalculatorwindowsettings-ptr.h"

//TODO: implement
class QXRD_EXPORT QxrdCalculatorWindow : public QxrdMainWindow, public Ui::QxrdCalculatorWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdCalculatorWindow(QString name);
  void initialize(QcepObjectWPtr parent);
  ~QxrdCalculatorWindow();

protected:
  void changeEvent(QEvent *e);

private:
};

#endif // QXRDCALCULATORWINDOW_H
