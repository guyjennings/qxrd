#ifndef QXRDSTARTUPWINDOWSETTINGS_H
#define QXRDSTARTUPWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdStartupWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  QxrdStartupWindowSettings(QString name, QString desc);

  void initialize(QcepObjectWPtr parent);

  QxrdMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdStartupWindowSettings*)

#endif // QXRDSTARTUPWINDOWSETTINGS_H
