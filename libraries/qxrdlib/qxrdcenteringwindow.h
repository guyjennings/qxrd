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

public:
  explicit QxrdCenteringWindow(QxrdCenteringWindowSettingsWPtr set,
                               QString name,
                               QxrdApplicationWPtr app,
                               QxrdExperimentWPtr expt);
  ~QxrdCenteringWindow();

protected:
  void changeEvent(QEvent *e);

  QxrdCenteringWindowSettingsWPtr m_CenteringWindowSettings;
};

#endif // QXRDCENTERINGWINDOW_H
