#ifndef QXRDSCRIPTINGWINDOW_H
#define QXRDSCRIPTINGWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdscriptingwindow.h"
#include "qxrdscriptingwindowsettings-ptr.h"

//TODO: implement
class QXRD_EXPORT QxrdScriptingWindow : public QxrdMainWindow, public Ui::QxrdScriptingWindow
{
  Q_OBJECT

public:
  explicit QxrdScriptingWindow(QxrdScriptingWindowSettingsWPtr set,
                               QString name,
                               QxrdApplicationWPtr app,
                               QxrdExperimentWPtr expt,
                               QxrdAcquisitionWPtr acqw,
                               QxrdProcessorWPtr procw);
  ~QxrdScriptingWindow();

protected:
  void changeEvent(QEvent *e);

  QxrdScriptingWindowSettingsWPtr m_ScriptingWindowSettings;
};

#endif // QXRDSCRIPTINGWINDOW_H
