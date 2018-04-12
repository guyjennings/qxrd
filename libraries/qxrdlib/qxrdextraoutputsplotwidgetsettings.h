#ifndef QXRDEXTRAOUTPUTSPLOTWIDGETSETTINGS_H
#define QXRDEXTRAOUTPUTSPLOTWIDGETSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepplotwidgetsettings.h"

class QXRD_EXPORT QxrdExtraOutputsPlotWidgetSettings : public QcepPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdExtraOutputsPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdExtraOutputsPlotWidgetSettings*)

#endif // QXRDEXTRAOUTPUTSPLOTWIDGETSETTINGS_H
