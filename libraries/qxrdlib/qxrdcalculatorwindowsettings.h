#ifndef QXRDCALCULATORWINDOWSETTINGS_H
#define QXRDCALCULATORWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdCalculatorWindowSettings : public QxrdMainWindowSettings
{
public:
  Q_INVOKABLE QxrdCalculatorWindowSettings(QString name);
};

#endif // QXRDCALCULATORWINDOWSETTINGS_H
