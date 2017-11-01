#ifndef QXRDCALIBRANTWINDOWSETTINGS_H
#define QXRDCALIBRANTWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdCalibrantWindowSettings : public QxrdMainWindowSettings
{
public:
  Q_INVOKABLE QxrdCalibrantWindowSettings(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt);
};

#endif // QXRDCALIBRANTWINDOWSETTINGS_H
