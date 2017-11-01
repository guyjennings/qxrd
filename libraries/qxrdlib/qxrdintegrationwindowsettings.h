#ifndef QXRDINTEGRATIONWINDOWSETTINGS_H
#define QXRDINTEGRATIONWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdIntegrationWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdIntegrationWindowSettings(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt);
};

Q_DECLARE_METATYPE(QxrdIntegrationWindowSettings*)

#endif // QXRDINTEGRATIONWINDOWSETTINGS_H
