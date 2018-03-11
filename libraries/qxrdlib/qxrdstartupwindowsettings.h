#ifndef QXRDSTARTUPWINDOWSETTINGS_H
#define QXRDSTARTUPWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdStartupWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  Q_INVOKABLE QxrdStartupWindowSettings(QString name);

  void initialize(QcepObjectWPtr parent);

  QcepMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdStartupWindowSettings*)

#endif // QXRDSTARTUPWINDOWSETTINGS_H
