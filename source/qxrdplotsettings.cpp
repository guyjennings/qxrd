#include "qxrdplotsettings.h"

QxrdPlotSettings::QxrdPlotSettings(QxrdSettingsSaverWPtr saver, QObject *parent) :
  QObject(parent),
  m_XMouse(QxrdSettingsSaverPtr(), this,"xMouse",0, "X Position of Mouse"),
  m_YMouse(QxrdSettingsSaverPtr(), this,"yMouse",0, "Y Position of Mouse"),
  m_XAxisLog(saver, this,"xAxisLog",0, "Logarithmic X Axis?"),
  m_YAxisLog(saver, this,"yAxisLog",0, "Logarithmic Y Axis?"),
  m_X2AxisLog(saver, this,"x2AxisLog",0, "Logarithmic 2nd X Axis?"),
  m_Y2AxisLog(saver, this,"y2AxisLog",0, "Logarithmic 2nd Y Axis?")
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

