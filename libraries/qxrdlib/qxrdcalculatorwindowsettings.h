#ifndef QXRDCALCULATORWINDOWSETTINGS_H
#define QXRDCALCULATORWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdCalculatorWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  QxrdCalculatorWindowSettings(QString name, QString desc);

  QxrdMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdCalculatorWindowSettings*)

#endif // QXRDCALCULATORWINDOWSETTINGS_H
