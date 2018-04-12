#ifndef QXRDDISTORTIONPLOTWIDGETSETTINGS_H
#define QXRDDISTORTIONPLOTWIDGETSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepplotwidgetsettings.h"

class QXRD_EXPORT QxrdDistortionPlotWidgetSettings : public QcepPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdDistortionPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdDistortionPlotWidgetSettings*)

#endif // QXRDDISTORTIONPLOTWIDGETSETTINGS_H
