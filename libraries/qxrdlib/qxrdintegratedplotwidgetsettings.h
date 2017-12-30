#ifndef QXRDINTEGRATEDPLOTWIDGETSETTINGS_H
#define QXRDINTEGRATEDPLOTWIDGETSETTINGS_H

#include "qxrdplotwidgetsettings.h"

class QXRD_EXPORT QxrdIntegratedPlotWidgetSettings : public QxrdPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdIntegratedPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdIntegratedPlotWidgetSettings*)

#endif // QXRDINTEGRATEDPLOTWIDGETSETTINGS_H
