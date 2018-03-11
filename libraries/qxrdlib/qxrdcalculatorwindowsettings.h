#ifndef QXRDCALCULATORWINDOWSETTINGS_H
#define QXRDCALCULATORWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdCalculatorWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  Q_INVOKABLE QxrdCalculatorWindowSettings(QString name);
  void initialize(QcepObjectWPtr parent);

  QcepMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdCalculatorWindowSettings*)

#endif // QXRDCALCULATORWINDOWSETTINGS_H
