#ifndef QXRDHELPWINDOWSETTINGS_H
#define QXRDHELPWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdHelpWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  QxrdHelpWindowSettings(QString name, QString desc);

  QxrdMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdHelpWindowSettings*)

#endif // QXRDHELPWINDOWSETTINGS_H
