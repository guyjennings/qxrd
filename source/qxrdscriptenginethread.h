#ifndef QXRDSCRIPTENGINETHREAD_H
#define QXRDSCRIPTENGINETHREAD_H

#include "qxrdthread.h"
#include "qxrdscriptengine.h"
#include <QDateTime>

class QxrdApplication;
class QxrdExperiment;
typedef QWeakPointer<QxrdExperiment> QxrdExperimentWPtr;

class QxrdScriptEngineThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdScriptEngineThread(QxrdApplication* app, QxrdExperimentWPtr exp);
  ~QxrdScriptEngineThread();

  void shutdown();
  QxrdScriptEnginePtr scriptEngine() const;

protected:
  void run();

private:
  QxrdScriptEnginePtr m_ScriptEngine;
  QxrdApplication* m_Application;
  QxrdExperimentWPtr  m_Experiment;
};

typedef QSharedPointer<QxrdScriptEngineThread> QxrdScriptEngineThreadPtr;

#endif // QXRDSCRIPTENGINETHREAD_H
