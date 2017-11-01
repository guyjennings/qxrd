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

  virtual QxrdMainWindowPtr newWindow() = 0;

  QxrdMainWindowPtr  window();

protected:
  QxrdApplicationWPtr m_Application;
  QxrdExperimentWPtr  m_Experiment;
  QxrdMainWindowPtr   m_Window;
};

Q_DECLARE_METATYPE(QxrdMainWindowSettings*)

#endif // QXRDMAINWINDOWSETTINGS_H
