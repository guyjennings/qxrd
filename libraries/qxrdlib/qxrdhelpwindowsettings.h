#ifndef QXRDHELPWINDOWSETTINGS_H
#define QXRDHELPWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdHelpWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdHelpWindowSettings(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt);
};

Q_DECLARE_METATYPE(QxrdHelpWindowSettings*)

#endif // QXRDHELPWINDOWSETTINGS_H
