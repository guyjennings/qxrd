#ifndef QXRDSCRIPTENGINETHREAD_PTR_H
#define QXRDSCRIPTENGINETHREAD_PTR_H

#include <QSharedPointer>

class QxrdScriptEngineThread;

typedef QSharedPointer<QxrdScriptEngineThread> QxrdScriptEngineThreadPtr;
typedef QWeakPointer<QxrdScriptEngineThread>   QxrdScriptEngineThreadWPtr;

#endif // QXRDSCRIPTENGINETHREAD_PTR_H
