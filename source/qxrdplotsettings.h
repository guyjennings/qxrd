#ifndef QXRDPLOTSETTINGS_H
#define QXRDPLOTSETTINGS_H

#include <QObject>
#include "qcepproperty.h"

class QxrdPlotSettings : public QObject
{
  Q_OBJECT
public:
  explicit QxrdPlotSettings(QxrdSettingsSaverWPtr saver, QObject *parent);
  
  Q_PROPERTY(double xMouse READ get_XMouse WRITE set_XMouse STORED false)
  QCEP_DOUBLE_PROPERTY(XMouse)

  Q_PROPERTY(double yMouse READ get_YMouse WRITE set_YMouse STORED false)
  QCEP_DOUBLE_PROPERTY(YMouse)

  Q_PROPERTY(bool xAxisLog READ get_XAxisLog WRITE set_XAxisLog)
  QCEP_BOOLEAN_PROPERTY(XAxisLog)

  Q_PROPERTY(bool yAxisLog READ get_YAxisLog WRITE set_YAxisLog)
  QCEP_BOOLEAN_PROPERTY(YAxisLog)

  Q_PROPERTY(bool x2AxisLog READ get_X2AxisLog WRITE set_X2AxisLog)
  QCEP_BOOLEAN_PROPERTY(X2AxisLog)

  Q_PROPERTY(bool y2AxisLog READ get_Y2AxisLog WRITE set_Y2AxisLog)
  QCEP_BOOLEAN_PROPERTY(Y2AxisLog)

public:
  virtual void readSettings(QSettings *settings, QString section);
  virtual void writeSettings(QSettings *settings, QString section);

signals:
  
public slots:
  
};

typedef QSharedPointer<QxrdPlotSettings> QxrdPlotSettingsPtr;
typedef QWeakPointer<QxrdPlotSettings>   QxrdPlotSettingsWPtr;

#endif // QXRDPLOTSETTINGS_H
