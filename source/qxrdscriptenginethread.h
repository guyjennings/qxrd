#ifndef QXRDSCRIPTENGINETHREAD_H
#define QXRDSCRIPTENGINETHREAD_H

#include "qxrdthread.h"
#include <QAtomicPointer>
#include <QDateTime>

class QxrdApplication;
class QxrdWindow;
class QxrdAcquisition;
class QxrdDataProcessor;
class QxrdScriptEngine;

class QxrdScriptEngineThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdScriptEngineThread(QxrdApplication *app);
  ~QxrdScriptEngineThread();

  void shutdown();
  QxrdScriptEngine *scriptEngine() const;

protected:
  void run();

private:
  QAtomicPointer<QxrdScriptEngine>  m_ScriptEngine;
  QxrdApplication   *m_Application;
};

#endif // QXRDSCRIPTENGINETHREAD_H
