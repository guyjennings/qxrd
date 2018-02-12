#ifndef QXRDDISTORTIONPLOTWIDGETSETTINGS_H
#define QXRDDISTORTIONPLOTWIDGETSETTINGS_H

#include "qxrdplotwidgetsettings.h"

class QXRD_EXPORT QxrdDistortionPlotWidgetSettings : public QxrdPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdDistortionPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdDistortionPlotWidgetSettings*)

#endif // QXRDDISTORTIONPLOTWIDGETSETTINGS_H