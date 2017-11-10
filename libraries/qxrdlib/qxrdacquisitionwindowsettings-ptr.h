#ifndef QXRDACQUISITIONWINDOWSETTINGSPTR_H
#define QXRDACQUISITIONWINDOWSETTINGSPTR_H

#include <QSharedPointer>

class QxrdAcquisitionWindowSettings;

typedef QSharedPointer<QxrdAcquisitionWindowSettings> QxrdAcquisitionWindowSettingsPtr;
typedef QWeakPointer<QxrdAcquisitionWindowSettings>   QxrdAcquisitionWindowSettingsWPtr;

#endif // QXRDACQUISITIONWINDOWSETTINGSPTR_H
