#ifndef QXRDACQUISITIONWINDOWSETTINGS_H
#define QXRDACQUISITIONWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdAcquisitionWindowSettings : public QxrdMainWindowSettings
{
public:
  Q_INVOKABLE QxrdAcquisitionWindowSettings(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt);
};

#endif // QXRDACQUISITIONWINDOWSETTINGS_H
