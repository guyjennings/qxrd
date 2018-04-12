#ifndef QXRDHISTOGRAMPLOTWIDGETSETTINGS_H
#define QXRDHISTOGRAMPLOTWIDGETSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepplotwidgetsettings.h"

class QXRD_EXPORT QxrdHistogramPlotWidgetSettings : public QcepPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdHistogramPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdHistogramPlotWidgetSettings*)

#endif // QXRDHISTOGRAMPLOTWIDGETSETTINGS_H
