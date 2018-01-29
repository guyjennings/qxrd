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

public:
  explicit QxrdCalculatorWindow(QxrdCalculatorWindowSettingsWPtr set,
                                QString name,
                                QxrdAppCommonWPtr app,
                                QxrdExperimentWPtr expt,
                                QxrdAcquisitionWPtr acqw,
                                QxrdProcessorWPtr procw);
  ~QxrdCalculatorWindow();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdCalculatorWindowSettingsWPtr m_CalculatorWindowSettings;
};

#endif // QXRDCALCULATORWINDOW_H
