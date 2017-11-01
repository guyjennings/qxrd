#ifndef QXRDHELPWINDOWSETTINGS_H
#define QXRDHELPWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdHelpWindowSettings : public QxrdMainWindowSettings
{
public:
  Q_INVOKABLE QxrdHelpWindowSettings(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt);
};

#endif // QXRDHELPWINDOWSETTINGS_H
