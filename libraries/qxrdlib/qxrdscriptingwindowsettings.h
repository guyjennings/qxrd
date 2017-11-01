#ifndef QXRDSCRIPTINGWINDOWSETTINGS_H
#define QXRDSCRIPTINGWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdScriptingWindowSettings : public QxrdMainWindowSettings
{
public:
  Q_INVOKABLE QxrdScriptingWindowSettings(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt);
};

#endif // QXRDSCRIPTINGWINDOWSETTINGS_H
