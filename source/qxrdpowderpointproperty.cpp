#include "qxrdpowderpointproperty.h"

QxrdPowderPointProperty::QxrdPowderPointProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QxrdPowderPoint value, QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(value),
  m_Value(value)
{
}

QxrdPowderPointProperty::QxrdPowderPointProperty(QcepSettingsSaverWPtr saver,
                                                 QObject *parent,
                                                 const char *name,
                                                 int n1, int n2, double x, double y,
                                                 QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(QxrdPowderPoint(n1,n2,x,y)),
  m_Value(QxrdPowderPoint(n1,n2,x,y))
{
}
