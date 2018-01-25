#ifndef QXRDWATCHERWINDOW_H
#define QXRDWATCHERWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdwatcherwindow.h"
#include "qxrdwatcherwindowsettings-ptr.h"
#include "qxrdwatcher-ptr.h"

class QXRD_EXPORT QxrdWatcherWindow : public QxrdMainWindow, public Ui::QxrdWatcherWindow
{
  Q_OBJECT

public:
  explicit QxrdWatcherWindow(QxrdWatcherWindowSettingsWPtr set,
                             QString name,
                             QxrdApplicationWPtr app,
                             QxrdExperimentWPtr expt,
                             QxrdAcquisitionWPtr acqw,
                             QxrdProcessorWPtr procw,
                             QxrdWatcherWPtr watcher);
  ~QxrdWatcherWindow();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdWatcherWindowSettingsWPtr m_WatcherWindowSettings;
  QxrdWatcherWPtr               m_Watcher;
};

#endif // QXRDWATCHERWINDOW_H
