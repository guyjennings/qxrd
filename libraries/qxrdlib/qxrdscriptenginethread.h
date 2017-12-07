#ifndef QXRDSCRIPTENGINETHREAD_H
#define QXRDSCRIPTENGINETHREAD_H

#include "qxrdlib_global.h"
#include "qxrdthread.h"
#include <QDateTime>
#include "qxrdscriptengine-ptr.h"
#include "qxrdapplication-ptr.h"
#include "qxrdexperiment-ptr.h"

class QXRD_EXPORT QxrdScriptEngineThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdScriptEngineThread(QxrdApplicationWPtr app, QxrdExperimentWPtr exp);
  ~QxrdScriptEngineThread();

  void shutdown();
  QxrdScriptEnginePtr scriptEngine() const;

protected:
  void run();

private:
  QxrdScriptEnginePtr m_ScriptEngine;
  QxrdApplicationWPtr m_Application;
  QxrdExperimentWPtr  m_Experiment;
};

#endif // QXRDSCRIPTENGINETHREAD_H
