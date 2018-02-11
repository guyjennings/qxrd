#ifndef QXRDSCRIPTINGWINDOWSETTINGS_H
#define QXRDSCRIPTINGWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdScriptingWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  QxrdScriptingWindowSettings(QString name, QString desc);

  QxrdMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdScriptingWindowSettings*)

#endif // QXRDSCRIPTINGWINDOWSETTINGS_H
