#ifndef QCEPPLOTWIDGETSETTINGS_H
#define QCEPPLOTWIDGETSETTINGS_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"

//TODO: implement mouse tracking
class QCEP_EXPORT QcepPlotWidgetSettings : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QcepPlotWidgetSettings(QString name);

public:
  Q_PROPERTY(int  legendPosition READ get_LegendPosition WRITE set_LegendPosition)
  QCEP_INTEGER_PROPERTY(LegendPosition)

  Q_PROPERTY(bool xAxisVis READ get_XAxisVis WRITE set_XAxisVis)
  QCEP_BOOLEAN_PROPERTY(XAxisVis)

  Q_PROPERTY(bool yAxisVis READ get_YAxisVis WRITE set_YAxisVis)
  QCEP_BOOLEAN_PROPERTY(YAxisVis)

  Q_PROPERTY(bool x2AxisVis READ get_X2AxisVis WRITE set_X2AxisVis)
  QCEP_BOOLEAN_PROPERTY(X2AxisVis)

  Q_PROPERTY(bool y2AxisVis READ get_Y2AxisVis WRITE set_Y2AxisVis)
  QCEP_BOOLEAN_PROPERTY(Y2AxisVis)

  Q_PROPERTY(bool xAxisLog READ get_XAxisLog WRITE set_XAxisLog)
  QCEP_BOOLEAN_PROPERTY(XAxisLog)

  Q_PROPERTY(bool yAxisLog READ get_YAxisLog WRITE set_YAxisLog)
  QCEP_BOOLEAN_PROPERTY(YAxisLog)

  Q_PROPERTY(bool x2AxisLog READ get_X2AxisLog WRITE set_X2AxisLog)
  QCEP_BOOLEAN_PROPERTY(X2AxisLog)

  Q_PROPERTY(bool y2AxisLog READ get_Y2AxisLog WRITE set_Y2AxisLog)
  QCEP_BOOLEAN_PROPERTY(Y2AxisLog)

  Q_PROPERTY(double xMouse READ get_XMouse WRITE set_XMouse STORED false)
  QCEP_DOUBLE_PROPERTY(XMouse)

  Q_PROPERTY(double yMouse READ get_YMouse WRITE set_YMouse STORED false)
  QCEP_DOUBLE_PROPERTY(YMouse)
};

Q_DECLARE_METATYPE(QcepPlotWidgetSettings*)

#endif // QCEPPLOTWIDGETSETTINGS_H
