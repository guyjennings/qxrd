#ifndef QXRDDETECTORPLUGININTERFACEPTR_H
#define QXRDDETECTORPLUGININTERFACEPTR_H

#include <QSharedPointer>

class QxrdDetectorPluginInterface;

typedef QSharedPointer<QxrdDetectorPluginInterface> QxrdDetectorPluginInterfacePtr;
typedef QWeakPointer<QxrdDetectorPluginInterface>   QxrdDetectorPluginInterfaceWPtr;

#endif // QXRDDETECTORPLUGININTERFACEPTR_H
