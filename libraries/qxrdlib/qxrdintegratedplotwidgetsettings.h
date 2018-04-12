#ifndef QXRDINTEGRATEDPLOTWIDGETSETTINGS_H
#define QXRDINTEGRATEDPLOTWIDGETSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepplotwidgetsettings.h"

class QXRD_EXPORT QxrdIntegratedPlotWidgetSettings : public QcepPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdIntegratedPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdIntegratedPlotWidgetSettings*)

#endif // QXRDINTEGRATEDPLOTWIDGETSETTINGS_H
