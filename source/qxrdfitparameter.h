#ifndef QXRDFITPARAMETER_H
#define QXRDFITPARAMETER_H

#include "qcepproperty.h"

class QxrdFitParameter : public QcepDoubleProperty
{
  Q_OBJECT
public:
  QxrdFitParameter(QxrdSettingsSaverPtr saver, QObject *parent, const char *name, QVariant value);

  double lowerLimit() const;
  double upperLimit() const;
  bool isFitted() const;
  bool isLimited() const;

  void setLowerLimit(double ll);
  void setUpperLimit(double ul);
  void setFitted(bool fit);
  void setLimited(bool lim);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

private:
  double  m_LowerLimit;
  double  m_UpperLimit;
  int     m_IsFitted;
  int     m_IsLimited;
};

typedef QSharedPointer<QxrdFitParameter> QxrdFitParameterPtr;

Q_DECLARE_METATYPE(QxrdFitParameterPtr)

#define QXRD_FIT_PARAMETER(propname) \
public: \
      double get_##propname() const \
  { \
    return m_##propname.value(); \
  } \
\
void set_##propname(double val) \
{ \
  m_##propname.setValue(val); \
} \
\
double def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(double val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QxrdFitParameter *prop_##propname() { \
                                    return &m_##propname; \
                                  } \
\
private: \
  QxrdFitParameter m_##propname;

#endif // QXRDFITPARAMETER_H
