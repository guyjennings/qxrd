#ifndef QXRDMASKINGWINDOWSETTINGS_H
#define QXRDMASKINGWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdMaskingWindowSettings : public QxrdMainWindowSettings
{
public:
  Q_INVOKABLE QxrdMaskingWindowSettings(QString name);
};

#endif // QXRDMASKINGWINDOWSETTINGS_H
