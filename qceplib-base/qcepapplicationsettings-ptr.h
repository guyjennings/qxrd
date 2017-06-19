#ifndef QCEPAPPLICATIONSETTINGSPTR_H
#define QCEPAPPLICATIONSETTINGSPTR_H

#include <QSharedPointer>

class QcepApplicationSettings;

typedef QSharedPointer<QcepApplicationSettings> QcepApplicationSettingsPtr;
typedef QWeakPointer<QcepApplicationSettings>   QcepApplicationSettingsWPtr;

#endif // QCEPAPPLICATIONSETTINGSPTR_H
