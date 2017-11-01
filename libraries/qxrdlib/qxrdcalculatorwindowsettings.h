#ifndef QXRDCALCULATORWINDOWSETTINGS_H
#define QXRDCALCULATORWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdCalculatorWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdCalculatorWindowSettings(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt);
};

Q_DECLARE_METATYPE(QxrdCalculatorWindowSettings*)

#endif // QXRDCALCULATORWINDOWSETTINGS_H
