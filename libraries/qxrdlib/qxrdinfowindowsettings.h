#ifndef QXRDINFOWINDOWSETTINGS_H
#define QXRDINFOWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdInfoWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdInfoWindowSettings(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt);
};

Q_DECLARE_METATYPE(QxrdInfoWindowSettings*)

#endif // QXRDINFOWINDOWSETTINGS_H
