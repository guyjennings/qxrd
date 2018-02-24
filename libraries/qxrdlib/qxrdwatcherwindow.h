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

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdWatcherWindow(QxrdWatcherWindowSettingsWPtr set,
                             QString name,
                             QxrdAppCommonWPtr app,
                             QxrdExperimentWPtr expt,
                             QxrdAcqCommonWPtr acqw,
                             QxrdProcessorWPtr procw,
                             QxrdWatcherWPtr watcher);
  void initialize(QcepObjectWPtr parent);
  ~QxrdWatcherWindow();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdWatcherWindowSettingsWPtr m_WatcherWindowSettings;
  QxrdWatcherWPtr               m_Watcher;
};

#endif // QXRDWATCHERWINDOW_H
