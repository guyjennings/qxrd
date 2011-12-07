#include "qxrdfitparameter.h"
#include <QSettings>

QxrdFitParameter::QxrdFitParameter(QxrdSettingsSaver *saver, QObject *parent, const char *name, QVariant value) :
    QcepDoubleProperty(saver, parent, name, value.toDouble()),
    m_LowerLimit(0),
    m_UpperLimit(0),
    m_IsFitted(true),
    m_IsLimited(false)
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

void QxrdFitParameter::writeSettings(QSettings *settings, QString section)
{
  if (settings) {
    settings->beginGroup(section+"/"+name());

    settings->setValue("value",value());
    settings->setValue("lower",m_LowerLimit);
    settings->setValue("upper",m_UpperLimit);
    settings->setValue("fitted",m_IsFitted);
    settings->setValue("limited",m_IsLimited);

    settings->endGroup();
  }
}

void QxrdFitParameter::readSettings(QSettings *settings, QString section)
{
  if (settings) {
    settings->beginGroup(section+"/"+name());

    if (settings->contains("value")) setValue(settings->value("value").toDouble());
    if (settings->contains("lower")) m_LowerLimit = settings->value("lower").toDouble();
    if (settings->contains("upper")) m_UpperLimit = settings->value("upper").toDouble();
    if (settings->contains("fitted")) m_IsFitted   = settings->value("fitted").toInt();
    if (settings->contains("limited")) m_IsLimited  = settings->value("limited").toInt();

    settings->endGroup();
  }
}
