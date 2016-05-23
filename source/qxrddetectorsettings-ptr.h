#ifndef QXRDDETECTORPTR_H
#define QXRDDETECTORPTR_H

#include <QSharedPointer>

class QxrdDetectorSettings;

typedef QSharedPointer<QxrdDetectorSettings> QxrdDetectorSettingsPtr;
typedef QWeakPointer<QxrdDetectorSettings>   QxrdDetectorSettingsWPtr;

#endif // QXRDDETECTORPTR_H
