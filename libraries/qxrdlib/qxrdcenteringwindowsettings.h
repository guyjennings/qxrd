#ifndef QXRDCENTERINGWINDOWSETTINGS_H
#define QXRDCENTERINGWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdCenteringWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdCenteringWindowSettings(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt);
};

Q_DECLARE_METATYPE(QxrdCenteringWindowSettings*)

#endif // QXRDCENTERINGWINDOWSETTINGS_H
