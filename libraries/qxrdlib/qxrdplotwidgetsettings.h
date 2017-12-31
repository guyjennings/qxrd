#ifndef QXRDPLOTWIDGETSETTINGS_H
#define QXRDPLOTWIDGETSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"

//TODO: implement mouse tracking
class QXRD_EXPORT QxrdPlotWidgetSettings : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdPlotWidgetSettings(QString name);

public:
  Q_PROPERTY(int  legendPosition READ get_LegendPosition WRITE set_LegendPosition)
  QCEP_INTEGER_PROPERTY(LegendPosition)

  Q_PROPERTY(bool xAxisLog READ get_XAxisLog WRITE set_XAxisLog)
  QCEP_BOOLEAN_PROPERTY(XAxisLog)

  Q_PROPERTY(bool yAxisLog READ get_YAxisLog WRITE set_YAxisLog)
  QCEP_BOOLEAN_PROPERTY(YAxisLog)

  Q_PROPERTY(bool x2AxisLog READ get_X2AxisLog WRITE set_X2AxisLog)
  QCEP_BOOLEAN_PROPERTY(X2AxisLog)

  Q_PROPERTY(bool y2AxisLog READ get_Y2AxisLog WRITE set_Y2AxisLog)
  QCEP_BOOLEAN_PROPERTY(Y2AxisLog)
};

Q_DECLARE_METATYPE(QxrdPlotWidgetSettings*)

#endif // QXRDPLOTWIDGETSETTINGS_H
