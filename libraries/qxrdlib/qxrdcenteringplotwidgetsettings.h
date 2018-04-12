#ifndef QXRDCENTERINGPLOTWIDGETSETTINGS_H
#define QXRDCENTERINGPLOTWIDGETSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepplotwidgetsettings.h"

class QXRD_EXPORT QxrdCenteringPlotWidgetSettings : public QcepPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdCenteringPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdCenteringPlotWidgetSettings*)

#endif // QXRDCENTERINGPLOTWIDGETSETTINGS_H
