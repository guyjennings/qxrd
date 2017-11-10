#ifndef QXRDHELPWINDOW_H
#define QXRDHELPWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdhelpwindow.h"
#include "qxrdhelpwindowsettings-ptr.h"

class QXRD_EXPORT QxrdHelpWindow : public QxrdMainWindow, public Ui::QxrdHelpWindow
{
  Q_OBJECT

public:
  explicit QxrdHelpWindow(QxrdHelpWindowSettingsWPtr set,
                          QString name,
                          QxrdApplicationWPtr app,
                          QxrdExperimentWPtr expt);
  ~QxrdHelpWindow();

protected:
  void changeEvent(QEvent *e);

  QxrdHelpWindowSettingsWPtr m_HelpWindowSettings;
};

#endif // QXRDHELPWINDOW_H
