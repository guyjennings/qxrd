#ifndef QXRDNIDAQPLUGININTERFACE_PTR_H
#define QXRDNIDAQPLUGININTERFACE_PTR_H

#include <QSharedPointer>

class QxrdNIDAQPluginInterface;

typedef QSharedPointer<QxrdNIDAQPluginInterface> QxrdNIDAQPluginInterfacePtr;
typedef QWeakPointer<QxrdNIDAQPluginInterface> QxrdNIDAQPluginInterfaceWPtr;

#endif // QXRDNIDAQPLUGININTERFACE_H
