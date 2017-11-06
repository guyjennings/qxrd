#ifndef QXRDEXTRAIOWINDOWSETTINGS_H
#define QXRDEXTRAIOWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdExtraIOWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdExtraIOWindowSettings(QString name);

  QxrdMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdExtraIOWindowSettings*)

#endif // QXRDEXTRAIOWINDOWSETTINGS_H
