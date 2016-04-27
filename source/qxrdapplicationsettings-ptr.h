#ifndef QXRDAPPLICATIONSETTINGSPTR_H
#define QXRDAPPLICATIONSETTINGSPTR_H

#include <QSharedPointer>

class QxrdApplicationSettings;

typedef QSharedPointer<QxrdApplicationSettings> QxrdApplicationSettingsPtr;
typedef QWeakPointer<QxrdApplicationSettings>   QxrdApplicationSettingsWPtr;

#endif // QXRDAPPLICATIONSETTINGSPTR_H
