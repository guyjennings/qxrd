#ifndef QXRDHELPWINDOWSETTINGS_H
#define QXRDHELPWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdHelpWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  Q_INVOKABLE QxrdHelpWindowSettings(QString name);
  void initialize(QcepObjectWPtr parent);

  QcepMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdHelpWindowSettings*)

#endif // QXRDHELPWINDOWSETTINGS_H
