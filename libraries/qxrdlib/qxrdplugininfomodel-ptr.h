#ifndef QXRDPLUGININFOMODELPTR_H
#define QXRDPLUGININFOMODELPTR_H

#include <QSharedPointer>

class QxrdPluginInfoModel;

typedef QSharedPointer<QxrdPluginInfoModel> QxrdPluginInfoModelPtr;
typedef QWeakPointer<QxrdPluginInfoModel>   QxrdPluginInfoModelWPtr;

#endif // QXRDPLUGININFOMODELPTR_H
