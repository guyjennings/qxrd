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
  explicit QxrdWatcherWindow(QString name);
  void initialize(QcepObjectWPtr parent);
  ~QxrdWatcherWindow();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdWatcherWPtr               m_Watcher;
};

#endif // QXRDWATCHERWINDOW_H
