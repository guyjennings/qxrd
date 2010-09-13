/******************************************************************
*
*  $Id: qxrdfitparameter.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDFITPARAMETER_H
#define QXRDFITPARAMETER_H

#include "qcepproperty.h"

class QxrdFitParameter : public QcepDoubleProperty
{
  Q_OBJECT;
public:
  QxrdFitParameter(QObject *parent, QString name, QVariant value);

  double lowerLimit() const;
  double upperLimit() const;
  bool isFitted() const;
  bool isLimited() const;

  void setLowerLimit(double ll);
  void setUpperLimit(double ul);
  void setFitted(bool fit);
  void setLimited(bool lim);

  void readSettings(QSettings &settings, QString section);
  void writeSettings(QSettings &settings, QString section);

private:
  double  m_LowerLimit;
  double  m_UpperLimit;
  int     m_IsFitted;
  int     m_IsLimited;
  HEADER_IDENT("$Id: qxrdfitparameter.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

typedef QSharedPointer<QxrdFitParameter> QxrdFitParameterPtr;

Q_DECLARE_METATYPE(QxrdFitParameterPtr);

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

/******************************************************************
*
*  $Log: qxrdfitparameter.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.4  2010/08/17 20:19:26  jennings
*  QxrdFitParameter readSettings and writeSettings - saves value, limits and flags
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
