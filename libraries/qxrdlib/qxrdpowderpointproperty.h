#ifndef QXRDPOWDERPOINTPROPERTY_H
#define QXRDPOWDERPOINTPROPERTY_H

#include "qxrdlib_global.h"
#include "qcepobject-ptr.h"
#include "qcepproperty.h"
#include "qxrdpowderpoint.h"

class QXRD_EXPORT QxrdPowderPointProperty : public QcepProperty
{
  Q_OBJECT
public:
  explicit QxrdPowderPointProperty(QcepObject *parent,
                                   const char *name,
                                   QxrdPowderPoint value,
                                   QString toolTip);

  explicit QxrdPowderPointProperty(QcepObject *parent,
                                   const char *name,
                                   int n1, int n2, int n3,
                                   double x, double y, double r1, double r2, double az,
                                   QString toolTip);

  QxrdPowderPoint value() const;
  QxrdPowderPoint defaultValue() const;
  QString toString(const QxrdPowderPoint& pt);
  double subValue(int axis) const;

  void linkTo(QSpinBox *n1SpinBox, QSpinBox *n2SpinBox, QSpinBox *n3SpinBox, QDoubleSpinBox *xSpinBox, QDoubleSpinBox *ySpinBox, QDoubleSpinBox *r1SpinBox, QDoubleSpinBox *r2SpinBox, QDoubleSpinBox *azSpinBox);
  void linkTo(int axis, QSpinBox *spinBox);
  void linkTo(int axis, QDoubleSpinBox *spinBox);

public slots:
  void setValue(QxrdPowderPoint val, int index);
  void setValue(QxrdPowderPoint val);
  void setDefaultValue(QxrdPowderPoint val);
  void resetValue();
  void setSubValue(int axis, int value, int index);
  void setSubValue(int axis, int value);
  void setSubValue(int axis, double value, int index);
  void setSubValue(int axis, double value);

signals:
  void valueChanged(QxrdPowderPoint val, int index);
  void subValueChanged(int axis, int val, int index);
  void subValueChanged(int axis, double val, int index);

private:
  QxrdPowderPoint m_Default;
  QxrdPowderPoint m_Value;
};

class QXRD_EXPORT QxrdPowderPointPropertySpinBoxHelper : public QObject {
  Q_OBJECT

public:
  QxrdPowderPointPropertySpinBoxHelper(QSpinBox *spinBox, QxrdPowderPointProperty *property, int axis);
  void connect();

public slots:
  void setSubValue(int axis, int value, int index);
  void setValue(int value);

signals:
  void subValueChanged(int axis, int value, int index);

private:
  QSpinBox                   *m_SpinBox;
  QxrdPowderPointProperty    *m_Property;
  int                         m_Axis;
};

class QXRD_EXPORT QxrdPowderPointPropertyDoubleSpinBoxHelper : public QObject {
  Q_OBJECT

public:
  QxrdPowderPointPropertyDoubleSpinBoxHelper(QDoubleSpinBox *spinBox, QxrdPowderPointProperty *property, int axis);
  void connect();

public slots:
  void setSubValue(int axis, double value, int index);
  void setValue(double value);

signals:
  void subValueChanged(int axis, double value, int index);

private:
  QDoubleSpinBox             *m_DoubleSpinBox;
  QxrdPowderPointProperty    *m_Property;
  int                         m_Axis;
};

class QXRD_EXPORT QxrdPowderPointVectorProperty : public QcepProperty {
  Q_OBJECT
public:
  QxrdPowderPointVectorProperty(QcepObject *parent,
                                const char *name,
                                QxrdPowderPointVector value,
                                QString toolTip);

  QxrdPowderPointVector value() const;
  QxrdPowderPointVector defaultValue() const;
  QString toString(const QxrdPowderPointVector &vec);

public slots:
  void setValue(QxrdPowderPointVector val, int index);
  void setValue(QxrdPowderPointVector val);
  void setDefaultValue(QxrdPowderPointVector val);
  void resetValue();
  void clear();
  void appendValue(QxrdPowderPoint val);

signals:
  void valueChanged(QxrdPowderPointVector val, int index);

private:
  QxrdPowderPointVector m_Default;
  QxrdPowderPointVector m_Value;
};

#define QXRD_POWDERPOINT_PROPERTY(propname) \
public: \
  QxrdPowderPoint get_##propname() const \
{ \
  return m_##propname.value(); \
} \
  \
  void set_##propname(QxrdPowderPoint val) \
{ \
  m_##propname.setValue(val); \
} \
  \
  QxrdPowderPoint def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
  \
  void setdef_##propname(QxrdPowderPoint val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
  \
  void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
  \
QxrdPowderPointProperty *prop_##propname() { \
  return &m_##propname; \
} \
  \
private: \
QxrdPowderPointProperty m_##propname;

#define QXRD_POWDERPOINTVECTOR_PROPERTY(propname) \
public: \
  QxrdPowderPointVector get_##propname() const \
{ \
  return m_##propname.value(); \
} \
  \
  void set_##propname(QxrdPowderPointVector val) \
{ \
  m_##propname.setValue(val); \
} \
  \
  QxrdPowderPointVector def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
  \
  void setdef_##propname(QxrdPowderPointVector val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
  \
  void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
  \
QxrdPowderPointVectorProperty *prop_##propname() { \
  return &m_##propname; \
} \
  \
private: \
QxrdPowderPointVectorProperty m_##propname;

#endif // QXRDPOWDERPOINTPROPERTY_H
