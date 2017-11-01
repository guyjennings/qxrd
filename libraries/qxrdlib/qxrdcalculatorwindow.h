#ifndef QXRDCALCULATORWINDOW_H
#define QXRDCALCULATORWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdcalculatorwindow.h"

class QXRD_EXPORT QxrdCalculatorWindow : public QxrdMainWindow, public Ui::QxrdCalculatorWindow
{
  Q_OBJECT

public:
  explicit QxrdCalculatorWindow(QString name,
                                QxrdApplicationWPtr app,
                                QxrdExperimentWPtr expt);
  ~QxrdCalculatorWindow();

protected:
  void changeEvent(QEvent *e);
};

#endif // QXRDCALCULATORWINDOW_H
