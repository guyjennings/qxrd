#ifndef QXRDSCRIPTINGWINDOW_H
#define QXRDSCRIPTINGWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdscriptingwindow.h"
#include "qxrdscriptingwindowsettings-ptr.h"
#include "qxrdscriptengine-ptr.h"
#include <QScriptValue>

//TODO: implement
class QXRD_EXPORT QxrdScriptingWindow : public QxrdMainWindow, public Ui::QxrdScriptingWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdScriptingWindow(QxrdScriptingWindowSettingsWPtr set,
                               QString name,
                               QxrdAppCommonWPtr app,
                               QxrdExperimentWPtr expt,
                               QxrdAcqCommonWPtr acqw,
                               QxrdProcessorWPtr procw);
  void initialize(QcepObjectWPtr parent);
  ~QxrdScriptingWindow();

signals:
  void evaluateCommand(int n, QString cmd);

private slots:
  void doEvaluate();
  void resultAvailable(int n, QScriptValue res);

protected:
  void changeEvent(QEvent *e);

  QxrdScriptingWindowSettingsWPtr m_ScriptingWindowSettings;

  QxrdScriptEngineWPtr            m_ScriptEngine;
  int                             m_CommandIndex;
  int                             m_DataIndex;
};

#endif // QXRDSCRIPTINGWINDOW_H
