#ifndef QXRDSCRIPTENGINE_PTR_H
#define QXRDSCRIPTENGINE_PTR_H

#include <QSharedPointer>

class QxrdScriptEngine;

typedef QSharedPointer<QxrdScriptEngine> QxrdScriptEnginePtr;
typedef QWeakPointer<QxrdScriptEngine>   QxrdScriptEngineWPtr;

#endif // QXRDSCRIPTENGINE_PTR_H
