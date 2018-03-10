#ifndef QXRDSCRIPTENGINETHREAD_H
#define QXRDSCRIPTENGINETHREAD_H

#include "qxrdlib_global.h"
#include "qxrdthread.h"
#include <QDateTime>
#include "qxrdscriptengine-ptr.h"
#include "qxrdappcommon-ptr.h"
#include "qxrdexperiment-ptr.h"

class QXRD_EXPORT QxrdScriptEngineThread : public QxrdThread
{
  Q_OBJECT

private:
  typedef QxrdThread inherited;

public:
  Q_INVOKABLE QxrdScriptEngineThread(QString name);
  ~QxrdScriptEngineThread();

  void initialize(QcepObjectWPtr parent);

  void shutdown();
  QxrdScriptEnginePtr scriptEngine() const;

protected:
  void run();

private:
  QxrdScriptEnginePtr m_ScriptEngine;
  QxrdAppCommonWPtr   m_Application;
  QxrdExperimentWPtr  m_Experiment;
};

#endif // QXRDSCRIPTENGINETHREAD_H
