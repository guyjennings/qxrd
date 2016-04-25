#ifndef QCEPPLOTSETTINGS_H
#define QCEPPLOTSETTINGS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"

class QcepPlotSettings : public QcepObject
{
  Q_OBJECT

public:
  explicit QcepPlotSettings(QString name, QcepSettingsSaverWPtr saver, QcepObjectWPtr parent);

  Q_PROPERTY(double xMouse READ get_XMouse WRITE set_XMouse STORED false)
  QCEP_DOUBLE_PROPERTY(XMouse)

  Q_PROPERTY(double yMouse READ get_YMouse WRITE set_YMouse STORED false)
  QCEP_DOUBLE_PROPERTY(YMouse)

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

signals:

public slots:

};

#endif // QCEPPLOTSETTINGS_H
