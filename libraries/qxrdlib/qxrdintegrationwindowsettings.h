#ifndef QXRDINTEGRATIONWINDOWSETTINGS_H
#define QXRDINTEGRATIONWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdIntegrationWindowSettings : public QxrdMainWindowSettings
{
public:
  Q_INVOKABLE QxrdIntegrationWindowSettings(QString name);
};

#endif // QXRDINTEGRATIONWINDOWSETTINGS_H
