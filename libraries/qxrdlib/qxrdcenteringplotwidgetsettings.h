#ifndef QXRDCENTERINGPLOTWIDGETSETTINGS_H
#define QXRDCENTERINGPLOTWIDGETSETTINGS_H

#include "qxrdplotwidgetsettings.h"

class QXRD_EXPORT QxrdCenteringPlotWidgetSettings : public QxrdPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdCenteringPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdCenteringPlotWidgetSettings*)

#endif // QXRDCENTERINGPLOTWIDGETSETTINGS_H
