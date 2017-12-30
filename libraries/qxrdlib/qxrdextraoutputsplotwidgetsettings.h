#ifndef QXRDEXTRAOUTPUTSPLOTWIDGETSETTINGS_H
#define QXRDEXTRAOUTPUTSPLOTWIDGETSETTINGS_H

#include "qxrdplotwidgetsettings.h"

class QXRD_EXPORT QxrdExtraOutputsPlotWidgetSettings : public QxrdPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdExtraOutputsPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdExtraOutputsPlotWidgetSettings*)

#endif // QXRDEXTRAOUTPUTSPLOTWIDGETSETTINGS_H
