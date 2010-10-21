/******************************************************************
*
*  $Id: qxrdfitparameter.cpp,v 1.3 2010/10/21 16:31:24 jennings Exp $
*
*******************************************************************/

#include "qxrdfitparameter.h"
#include "qxrdsettings.h"

QxrdFitParameter::QxrdFitParameter(QObject *parent, const char *name, QVariant value) :
    QcepDoubleProperty(parent, name, value.toDouble()),
    m_LowerLimit(0),
    m_UpperLimit(0),
    m_IsFitted(true),
    m_IsLimited(false),
    SOURCE_IDENT("$Id: qxrdfitparameter.cpp,v 1.3 2010/10/21 16:31:24 jennings Exp $")
{
}

double QxrdFitParameter::lowerLimit() const
{
  return m_LowerLimit;
}

double QxrdFitParameter::upperLimit() const
{
  return m_UpperLimit;
}

bool QxrdFitParameter::isFitted() const
{
  return m_IsFitted;
}

bool QxrdFitParameter::isLimited() const
{
  return m_IsLimited;
}

void QxrdFitParameter::setLowerLimit(double ll)
{
  m_LowerLimit = ll;
}

void QxrdFitParameter::setUpperLimit(double ul)
{
  m_UpperLimit = ul;
}

void QxrdFitParameter::setFitted(bool fit)
{
  m_IsFitted = fit;
}

void QxrdFitParameter::setLimited(bool lim)
{
  m_IsLimited = lim;
}

void QxrdFitParameter::writeSettings(QSettings &settings, QString section)
{
  settings.beginGroup(section+"/"+name());

  settings.setValue("value",value());
  settings.setValue("lower",m_LowerLimit);
  settings.setValue("upper",m_UpperLimit);
  settings.setValue("fitted",m_IsFitted);
  settings.setValue("limited",m_IsLimited);

  settings.endGroup();
}

void QxrdFitParameter::readSettings(QSettings &settings, QString section)
{
  settings.beginGroup(section+"/"+name());

  if (settings.contains("value")) setValue(settings.value("value").toDouble());
  if (settings.contains("lower")) m_LowerLimit = settings.value("lower").toDouble();
  if (settings.contains("upper")) m_UpperLimit = settings.value("upper").toDouble();
  if (settings.contains("fitted")) m_IsFitted   = settings.value("fitted").toInt();
  if (settings.contains("limited")) m_IsLimited  = settings.value("limited").toInt();

  settings.endGroup();
}

/******************************************************************
*
*  $Log: qxrdfitparameter.cpp,v $
*  Revision 1.3  2010/10/21 16:31:24  jennings
*  Implemented saving of settings soon after they change, rather than at program exit
*
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.6  2010/08/17 20:19:26  jennings
*  QxrdFitParameter readSettings and writeSettings - saves value, limits and flags
*
*  Revision 1.1.2.5  2010/08/17 19:20:50  jennings
*  Added INVOKE_CHECK macro to check returned result QMetaObject::invokeMethod calls
*
*  Revision 1.1.2.4  2010/08/16 21:45:27  jennings
*  Distinguish between initial and refined fit parameters and data in powder ring fitting
*  Add read/write settings routines for powder ring fitting data
*
*  Revision 1.1.2.3  2010/08/09 02:45:38  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.2  2010/08/06 21:09:34  jennings
*  Initial partial implementation of powder ring fitting widget
*
*  Revision 1.1.2.1  2010/08/02 21:11:58  jennings
*  Added classes defining fitting parameters for a set of powder rings
*
*
*******************************************************************/
