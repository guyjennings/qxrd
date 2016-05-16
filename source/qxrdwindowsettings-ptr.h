#ifndef QXRDWINDOWSETTINGSPTR_H
#define QXRDWINDOWSETTINGSPTR_H

#include <QSharedPointer>

class QxrdWindowSettings;

typedef QSharedPointer<QxrdWindowSettings> QxrdWindowSettingsPtr;
typedef QWeakPointer<QxrdWindowSettings>   QxrdWindowSettingsWPtr;

#endif // QXRDWINDOWSETTINGSPTR_H
