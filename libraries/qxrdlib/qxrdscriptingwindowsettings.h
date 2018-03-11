#ifndef QXRDSCRIPTINGWINDOWSETTINGS_H
#define QXRDSCRIPTINGWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdScriptingWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  Q_INVOKABLE QxrdScriptingWindowSettings(QString name);
  void initialize(QcepObjectWPtr parent);

  QcepMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdScriptingWindowSettings*)

#endif // QXRDSCRIPTINGWINDOWSETTINGS_H
