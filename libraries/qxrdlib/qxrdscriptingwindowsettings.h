#ifndef QXRDSCRIPTINGWINDOWSETTINGS_H
#define QXRDSCRIPTINGWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdScriptingWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdScriptingWindowSettings(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt);
};

Q_DECLARE_METATYPE(QxrdScriptingWindowSettings*)

#endif // QXRDSCRIPTINGWINDOWSETTINGS_H
