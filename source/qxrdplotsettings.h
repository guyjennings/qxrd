#ifndef QXRDPLOTSETTINGS_H
#define QXRDPLOTSETTINGS_H

#include <QObject>
#include "qcepproperty.h"

class QxrdPlotSettings : public QObject
{
  Q_OBJECT
public:
  explicit QxrdPlotSettings(QObject *parent = 0);
  
  Q_PROPERTY(double xMouse READ get_XMouse WRITE set_XMouse STORED false)
  QCEP_DOUBLE_PROPERTY(XMouse)

  Q_PROPERTY(double yMouse READ get_YMouse WRITE set_YMouse STORED false)
  QCEP_DOUBLE_PROPERTY(YMouse)

  Q_PROPERTY(int xAxisLog READ get_XAxisLog WRITE set_XAxisLog)
  QCEP_INTEGER_PROPERTY(XAxisLog)

  Q_PROPERTY(int yAxisLog READ get_YAxisLog WRITE set_YAxisLog)
  QCEP_INTEGER_PROPERTY(YAxisLog)

  Q_PROPERTY(int x2AxisLog READ get_X2AxisLog WRITE set_X2AxisLog)
  QCEP_INTEGER_PROPERTY(X2AxisLog)

  Q_PROPERTY(int y2AxisLog READ get_Y2AxisLog WRITE set_Y2AxisLog)
  QCEP_INTEGER_PROPERTY(Y2AxisLog)

public:
  virtual void readSettings(QSettings *settings, QString section);
  virtual void writeSettings(QSettings *settings, QString section);

signals:
  
public slots:
  
};

#endif // QXRDPLOTSETTINGS_H
