#ifndef QXRDEXPERIMENTSETTINGSPTR_H
#define QXRDEXPERIMENTSETTINGSPTR_H

#include <QSharedPointer>

class QxrdExperimentSettings;

typedef QSharedPointer<QxrdExperimentSettings> QxrdExperimentSettingsPtr;
typedef QWeakPointer<QxrdExperimentSettings>   QxrdExperimentSettingsWPtr;

#endif // QXRDEXPERIMENTSETTINGSPTR_H
