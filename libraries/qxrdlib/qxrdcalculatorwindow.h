#ifndef QXRDCALCULATORWINDOW_H
#define QXRDCALCULATORWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdcalculatorwindow.h"
#include "qxrdcalculatorwindowsettings-ptr.h"

class QXRD_EXPORT QxrdCalculatorWindow : public QxrdMainWindow, public Ui::QxrdCalculatorWindow
{
  Q_OBJECT

public:
  explicit QxrdCalculatorWindow(QxrdCalculatorWindowSettingsWPtr set,
                                QString name,
                                QxrdApplicationWPtr app,
                                QxrdExperimentWPtr expt);
  ~QxrdCalculatorWindow();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdCalculatorWindowSettingsWPtr m_CalculatorWindowSettings;
};

#endif // QXRDCALCULATORWINDOW_H
