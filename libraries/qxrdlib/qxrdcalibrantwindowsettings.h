#ifndef QXRDCALIBRANTWINDOWSETTINGS_H
#define QXRDCALIBRANTWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdCalibrantWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdCalibrantWindowSettings(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt);
};

Q_DECLARE_METATYPE(QxrdCalibrantWindowSettings*)

#endif // QXRDCALIBRANTWINDOWSETTINGS_H
