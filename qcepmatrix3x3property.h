#ifndef QCEPMATRIX3X3PROPERTY_H
#define QCEPMATRIX3X3PROPERTY_H

#include "qcepproperty.h"
#include <QMatrix3x3>

class QcepMatrix3x3Property : public QcepProperty
{
  Q_OBJECT
public:
  explicit QcepMatrix3x3Property(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QMatrix3x3 value, QString toolTip);
  
  QMatrix3x3 value() const;
  QMatrix3x3 defaultValue() const;
  QString toString(const QMatrix3x3& mat);
  
  static void registerMetaTypes();

public slots:
  void setValue(QMatrix3x3 val, int index);
  void setValue(QMatrix3x3 val);
  void incValue(QMatrix3x3 step);
  void setDefaultValue(QMatrix3x3 val);
  void resetValue();

signals:
  void valueChanged(QMatrix3x3 val, int index);

private:
  QMatrix3x3 m_Default;
  QMatrix3x3 m_Value;
};

#define QCEP_MATRIX3X3_PROPERTY(propname) \
public: \
QMatrix3x3 get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QMatrix3x3 val) \
{ \
  m_##propname.setValue(val); \
} \
\
QMatrix3x3 def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QMatrix3x3 val) \
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
