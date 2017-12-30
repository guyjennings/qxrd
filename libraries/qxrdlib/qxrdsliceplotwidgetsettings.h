#ifndef QXRDSLICEPLOTWIDGETSETTINGS_H
#define QXRDSLICEPLOTWIDGETSETTINGS_H

#include "qxrdplotwidgetsettings.h"

class QXRD_EXPORT QxrdSlicePlotWidgetSettings : public QxrdPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdSlicePlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdSlicePlotWidgetSettings*)

#endif // QXRDSLICEPLOTWIDGETSETTINGS_H
