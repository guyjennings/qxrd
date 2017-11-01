#ifndef QXRDACQUISITIONWINDOWSETTINGS_H
#define QXRDACQUISITIONWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdAcquisitionWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdAcquisitionWindowSettings(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt);

};

Q_DECLARE_METATYPE(QxrdAcquisitionWindowSettings*)

#endif // QXRDACQUISITIONWINDOWSETTINGS_H
