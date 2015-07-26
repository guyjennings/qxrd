#ifndef QCEPSCRIPTENGINEPTR_H
#define QCEPSCRIPTENGINEPTR_H

#include <QSharedPointer>

class QcepScriptEngine;

typedef QSharedPointer<QcepScriptEngine> QcepScriptEnginePtr;
typedef QWeakPointer<QcepScriptEngine>   QcepScriptEngineWPtr;

#endif // QCEPSCRIPTENGINEPTR_H

