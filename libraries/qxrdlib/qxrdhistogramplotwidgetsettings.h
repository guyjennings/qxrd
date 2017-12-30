#ifndef QXRDHISTOGRAMPLOTWIDGETSETTINGS_H
#define QXRDHISTOGRAMPLOTWIDGETSETTINGS_H

#include "qxrdplotwidgetsettings.h"

class QXRD_EXPORT QxrdHistogramPlotWidgetSettings : public QxrdPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdHistogramPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdHistogramPlotWidgetSettings*)

#endif // QXRDHISTOGRAMPLOTWIDGETSETTINGS_H
