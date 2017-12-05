#ifndef QCEPMATRIX3X3PROPERTY_H
#define QCEPMATRIX3X3PROPERTY_H

#include "qceplib_global.h"
#include "qcepproperty.h"
#include "qcepmatrix3x3.h"

class QCEP_EXPORT QcepMatrix3x3Property : public QcepProperty
{
  Q_OBJECT
public:
  explicit QcepMatrix3x3Property(QcepObject *parent,
                                 const char *name,
                                 QcepMatrix3x3 value,
                                 QString toolTip);
  explicit QcepMatrix3x3Property(QcepObject *parent,
                                 const char *name,
                                 double r0c0, double r0c1, double r0c2,
                                 double r1c0, double r1c1, double r1c2,
                                 double r2c0, double r2c1, double r2c2,
                                 QString toolTip);

  QcepMatrix3x3 value() const;
  QcepMatrix3x3 defaultValue() const;
  QString toString(const QcepMatrix3x3& mat);
  
  static void registerMetaTypes();

public slots:
  void setValue(QcepMatrix3x3 val, int index);
  void setValue(QcepMatrix3x3 val);
  void incValue(QcepMatrix3x3 step);
  void setDefaultValue(QcepMatrix3x3 val);
  void resetValue();

signals:
  void valueChanged(QcepMatrix3x3 val, int index);

private:
  QcepMatrix3x3 m_Default;
  QcepMatrix3x3 m_Value;
};

#define QCEP_MATRIX3X3_PROPERTY(propname) \
public: \
QcepMatrix3x3 get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QcepMatrix3x3 val) \
{ \
  m_##propname.setValue(val); \
} \
\
QcepMatrix3x3 def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QcepMatrix3x3 val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepMatrix3x3Property *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepMatrix3x3Property m_##propname;

#endif // QCEPMATRIX3X3PROPERTY_H
