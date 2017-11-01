#ifndef QXRDCALIBRANTWINDOWSETTINGS_H
#define QXRDCALIBRANTWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdCalibrantWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdCalibrantWindowSettings(QString name);

  QxrdMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdCalibrantWindowSettings*)

#endif // QXRDCALIBRANTWINDOWSETTINGS_H
