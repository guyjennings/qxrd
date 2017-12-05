#ifndef QXRDMAINWINDOWSETTINGS_H
#define QXRDMAINWINDOWSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepmainwindowsettings.h"
#include "qxrdapplication-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdmainwindow-ptr.h"

class QXRD_EXPORT QxrdMainWindowSettings : public QcepMainWindowSettings
{
  Q_OBJECT

public:
  QxrdMainWindowSettings(QString name);

  virtual void initialize(QxrdApplicationWPtr app, QxrdExperimentWPtr expt);

  void openWindow();
  void closeWindow();
  virtual QxrdMainWindowPtr newWindow() = 0;

  QxrdMainWindowPtr  window();

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

protected:
  QxrdApplicationWPtr m_Application;
  QxrdExperimentWPtr  m_Experiment;
  QxrdMainWindowPtr   m_Window;

public:
  Q_PROPERTY(int windowOpen READ get_WindowOpen WRITE set_WindowOpen)
  QCEP_INTEGER_PROPERTY(WindowOpen)

  Q_PROPERTY(QRectF windowRect READ get_WindowRect WRITE set_WindowRect)
  QCEP_DOUBLE_RECT_PROPERTY(WindowRect)
};

Q_DECLARE_METATYPE(QxrdMainWindowSettings*)

#endif // QXRDMAINWINDOWSETTINGS_H
