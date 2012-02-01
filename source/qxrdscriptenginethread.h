#ifndef QXRDSCRIPTENGINETHREAD_H
#define QXRDSCRIPTENGINETHREAD_H

#include "qxrdthread.h"
#include <QAtomicPointer>
#include <QDateTime>

class QxrdApplication;
class QxrdExperiment;
class QxrdScriptEngine;

class QxrdScriptEngineThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdScriptEngineThread(QxrdApplication *app, QxrdExperiment *exp);
  ~QxrdScriptEngineThread();

  void shutdown();
  QxrdScriptEngine *scriptEngine() const;

protected:
  void run();

private:
  QAtomicPointer<QxrdScriptEngine>  m_ScriptEngine;
  QxrdApplication   *m_Application;
  QxrdExperiment    *m_Experiment;
};

#endif // QXRDSCRIPTENGINETHREAD_H
