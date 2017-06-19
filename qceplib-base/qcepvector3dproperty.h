#ifndef QCEPVECTOR3DPROPERTY_H
#define QCEPVECTOR3DPROPERTY_H

#include "qcepproperty.h"
#include "qcepvector3d.h"

class QcepVector3DProperty : public QcepProperty
{
  Q_OBJECT
public:
  explicit QcepVector3DProperty(QcepObject *parent, const char *name, QcepVector3D value, QString toolTip);
  explicit QcepVector3DProperty(QcepObject *parent, const char *name, double x, double y, double z, QString toolTip);

  QcepVector3D value() const;
  QcepVector3D defaultValue() const;
  QString toString(const QcepVector3D& mat);

  static void registerMetaTypes();

public slots:
  void setValue(QcepVector3D val, int index);
  void setValue(QcepVector3D val);
  void incValue(QcepVector3D step);
  void setDefaultValue(QcepVector3D val);
  void resetValue();

signals:
  void valueChanged(QcepVector3D val, int index);

private:
  QcepVector3D m_Default;
  QcepVector3D m_Value;
};

#define QCEP_VECTOR3D_PROPERTY(propname) \
public: \
QcepVector3D get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QcepVector3D val) \
{ \
  m_##propname.setValue(val); \
} \
\
QcepVector3D def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QcepVector3D val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepVector3DProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepVector3DProperty m_##propname;

#endif // QCEPVECTOR3DPROPERTY_H
