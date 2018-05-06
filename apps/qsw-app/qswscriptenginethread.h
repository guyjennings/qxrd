#ifndef QSWSCRIPTENGINETHREAD_H
#define QSWSCRIPTENGINETHREAD_H

#include "qswlib_global.h"
#include "qcepthread.h"
#include "qswscriptengine-ptr.h"

class /*QSW_EXPORT*/ QswScriptEngineThread : public QcepThread
{
  Q_OBJECT

private:
  typedef QcepThread inherited;

public:
  Q_INVOKABLE QswScriptEngineThread(QString name);
  ~QswScriptEngineThread();

  void initialize(QcepObjectWPtr parent);

  QswScriptEngineWPtr scriptEngine();

protected:
  void run();

private:
  QswScriptEnginePtr m_ScriptEngine;
};

#endif // QSWSCRIPTENGINETHREAD_H
