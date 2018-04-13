#ifndef QCEPPOWDERPOINTPROPERTY_H
#define QCEPPOWDERPOINTPROPERTY_H

#include "qceplib_global.h"
#include "qcepobject-ptr.h"
#include "qcepproperty.h"
#include "qceppowderpoint.h"

class QCEP_EXPORT QcepPowderPointProperty : public QcepProperty
{
  Q_OBJECT
public:
  explicit QcepPowderPointProperty(QcepObject *parent,
                                   const char *name,
                                   QcepPowderPoint value,
                                   QString toolTip);

  explicit QcepPowderPointProperty(QcepObject *parent,
                                   const char *name,
                                   int n1, int n2, int n3,
                                   double x, double y, double r1, double r2, double az,
                                   QString toolTip);

  QcepPowderPoint value() const;
  QcepPowderPoint defaultValue() const;
  QString toString(const QcepPowderPoint& pt);
  double subValue(int axis) const;

  void linkTo(QSpinBox *n1SpinBox, QSpinBox *n2SpinBox, QSpinBox *n3SpinBox, QDoubleSpinBox *xSpinBox, QDoubleSpinBox *ySpinBox, QDoubleSpinBox *r1SpinBox, QDoubleSpinBox *r2SpinBox, QDoubleSpinBox *azSpinBox);
  void linkTo(int axis, QSpinBox *spinBox);
  void linkTo(int axis, QDoubleSpinBox *spinBox);

public slots:
  void setValue(QcepPowderPoint val, int index);
  void setValue(QcepPowderPoint val);
  void setDefaultValue(QcepPowderPoint val);
  void resetValue();
  void setSubValue(int axis, int value, int index);
  void setSubValue(int axis, int value);
  void setSubValue(int axis, double value, int index);
  void setSubValue(int axis, double value);

signals:
  void valueChanged(QcepPowderPoint val, int index);
  void subValueChanged(int axis, int val, int index);
  void subValueChanged(int axis, double val, int index);

private:
  QcepPowderPoint m_Default;
  QcepPowderPoint m_Value;
};

class QCEP_EXPORT QcepPowderPointPropertySpinBoxHelper : public QObject {
  Q_OBJECT

public:
  QcepPowderPointPropertySpinBoxHelper(QSpinBox *spinBox, QcepPowderPointProperty *property, int axis);
  void connect();

public slots:
  void setSubValue(int axis, int value, int index);
  void setValue(int value);

signals:
  void subValueChanged(int axis, int value, int index);

private:
  QSpinBox                   *m_SpinBox;
  QcepPowderPointProperty    *m_Property;
  int                         m_Axis;
};

class QCEP_EXPORT QcepPowderPointPropertyDoubleSpinBoxHelper : public QObject {
  Q_OBJECT

public:
  QcepPowderPointPropertyDoubleSpinBoxHelper(QDoubleSpinBox *spinBox, QcepPowderPointProperty *property, int axis);
  void connect();

public slots:
  void setSubValue(int axis, double value, int index);
  void setValue(double value);

signals:
  void subValueChanged(int axis, double value, int index);

private:
  QDoubleSpinBox             *m_DoubleSpinBox;
  QcepPowderPointProperty    *m_Property;
  int                         m_Axis;
};

class QCEP_EXPORT QcepPowderPointVectorProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepPowderPointVectorProperty(QcepObject *parent,
                                const char *name,
                                QcepPowderPointVector value,
                                QString toolTip);

  QcepPowderPointVector value() const;
  QcepPowderPointVector defaultValue() const;
  QString toString(const QcepPowderPointVector &vec);

public slots:
  void setValue(QcepPowderPointVector val, int index);
  void setValue(QcepPowderPointVector val);
  void setDefaultValue(QcepPowderPointVector val);
  void resetValue();
  void clear();
  void appendValue(QcepPowderPoint val);

signals:
  void valueChanged(QcepPowderPointVector val, int index);

private:
  QcepPowderPointVector m_Default;
  QcepPowderPointVector m_Value;
};

#define QCEP_POWDERPOINT_PROPERTY(propname) \
public: \
  QcepPowderPoint get_##propname() const \
{ \
  return m_##propname.value(); \
} \
  \
  void set_##propname(QcepPowderPoint val) \
{ \
  m_##propname.setValue(val); \
} \
  \
  QcepPowderPoint def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
  \
  void setdef_##propname(QcepPowderPoint val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
  \
  void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
  \
QcepPowderPointProperty *prop_##propname() { \
  return &m_##propname; \
} \
  \
private: \
QcepPowderPointProperty m_##propname;

#define QCEP_POWDERPOINTVECTOR_PROPERTY(propname) \
public: \
  QcepPowderPointVector get_##propname() const \
{ \
  return m_##propname.value(); \
} \
  \
  void set_##propname(QcepPowderPointVector val) \
{ \
  m_##propname.setValue(val); \
} \
  \
  QcepPowderPointVector def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
  \
  void setdef_##propname(QcepPowderPointVector val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
  \
  void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
  \
QcepPowderPointVectorProperty *prop_##propname() { \
  return &m_##propname; \
} \
  \
private: \
QcepPowderPointVectorProperty m_##propname;

#endif // QCEPPOWDERPOINTPROPERTY_H
