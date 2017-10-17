#ifndef QXRDDETECTORINTERFACEPTR_H
#define QXRDDETECTORINTERFACEPTR_H

#include <QSharedPointer>

class QxrdDetectorInterface;

typedef QSharedPointer<QxrdDetectorInterface> QxrdDetectorInterfacePtr;
typedef QWeakPointer<QxrdDetectorInterface>   QxrdDetectorInterfaceWPtr;

#endif // QXRDDETECTORINTERFACEPTR_H
