#include "qxrdplotsettings.h"

QxrdPlotSettings::QxrdPlotSettings(QObject *parent) :
  QObject(parent),
  m_XMouse(QxrdSettingsSaverPtr(), this,"xMouse",0, "X Position of Mouse"),
  m_YMouse(QxrdSettingsSaverPtr(), this,"yMouse",0, "Y Position of Mouse"),
  m_XAxisLog(QxrdSettingsSaverPtr(), this,"xAxisLog",0, "Logarithmic X Axis?"),
  m_YAxisLog(QxrdSettingsSaverPtr(), this,"yAxisLog",0, "Logarithmic Y Axis?"),
  m_X2AxisLog(QxrdSettingsSaverPtr(), this,"x2AxisLog",0, "Logarithmic 2nd X Axis?"),
  m_Y2AxisLog(QxrdSettingsSaverPtr(), this,"y2AxisLog",0, "Logarithmic 2nd Y Axis?"),
{
}

void QxrdPlotSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdPlotSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

