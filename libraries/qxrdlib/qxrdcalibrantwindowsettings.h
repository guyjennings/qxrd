#ifndef QXRDCALIBRANTWINDOWSETTINGS_H
#define QXRDCALIBRANTWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdCalibrantWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  Q_INVOKABLE QxrdCalibrantWindowSettings(QString name);
  void initialize(QcepObjectWPtr parent);

  QcepMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdCalibrantWindowSettings*)

#endif // QXRDCALIBRANTWINDOWSETTINGS_H
