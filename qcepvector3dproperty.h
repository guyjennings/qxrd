#ifndef QCEPVECTOR3DPROPERTY_H
#define QCEPVECTOR3DPROPERTY_H

#include "qcepproperty.h"
#include <QVector3D>

class QcepVector3DProperty : public QcepProperty
{
  Q_OBJECT
public:
  explicit QcepVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QVector3D value, QString toolTip);
  explicit QcepVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, double x, double y, double z, QString toolTip);

  QVector3D value() const;
  QVector3D defaultValue() const;
  QString toString(const QVector3D& mat);

  static void registerMetaTypes();

public slots:
  void setValue(QVector3D val, int index);
  void setValue(QVector3D val);
  void incValue(QVector3D step);
  void setDefaultValue(QVector3D val);
  void resetValue();

signals:
  void valueChanged(QVector3D val, int index);

private:
  QVector3D m_Default;
  QVector3D m_Value;
};

#define QCEP_VECTOR3D_PROPERTY(propname) \
public: \
QVector3D get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QVector3D val) \
{ \
  m_##propname.setValue(val); \
} \
\
QVector3D def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QVector3D val) \
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
