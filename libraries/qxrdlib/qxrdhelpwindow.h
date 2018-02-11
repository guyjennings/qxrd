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

public:
  explicit QxrdHelpWindow(QxrdHelpWindowSettingsWPtr set,
                          QString name,
                          QxrdAppCommonWPtr app,
                          QxrdExperimentWPtr expt,
                          QxrdAcqCommonWPtr acqw,
                          QxrdProcessorWPtr procw);
  ~QxrdHelpWindow();

protected:
  void changeEvent(QEvent *e);

  QxrdHelpWindowSettingsWPtr m_HelpWindowSettings;
};

#endif // QXRDHELPWINDOW_H
