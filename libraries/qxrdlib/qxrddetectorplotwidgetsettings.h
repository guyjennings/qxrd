#ifndef QXRDDETECTORPLOTWIDGETSETTINGS_H
#define QXRDDETECTORPLOTWIDGETSETTINGS_H

#include "qxrdplotwidgetsettings.h"

class QXRD_EXPORT QxrdDetectorPlotWidgetSettings : public QxrdPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdDetectorPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdDetectorPlotWidgetSettings*)

#endif // QXRDDETECTORPLOTWIDGETSETTINGS_H
