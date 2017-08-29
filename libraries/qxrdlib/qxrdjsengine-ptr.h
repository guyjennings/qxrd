#ifndef QXRDJSENGINEPTR_H
#define QXRDJSENGINEPTR_H

#include <QSharedPointer>

class QxrdJSEngine;

typedef QSharedPointer<QxrdJSEngine> QxrdJSEnginePtr;
typedef QWeakPointer<QxrdJSEngine>   QxrdJSEngineWPtr;

#endif // QXRDJSENGINEPTR_H
