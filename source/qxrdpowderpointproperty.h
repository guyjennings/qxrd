#ifndef QXRDPOWDERPOINTPROPERTY_H
#define QXRDPOWDERPOINTPROPERTY_H

#include "qcepproperty.h"
#include "qxrdpowderpoint.h"

class QxrdPowderPointProperty : public QcepProperty
{
  Q_OBJECT
public:
  explicit QxrdPowderPointProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QxrdPowderPoint value, QString toolTip);
  explicit QxrdPowderPointProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, int n1, int n2, double x, double y, QString toolTip);

  QxrdPowderPoint value() const;
  QxrdPowderPoint defaultValue() const;
  QString toString(const QxrdPowderPoint& pt);
  int subValue(int axis) const;

  static void registerMetaTypes();
  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdPowderPoint &pt);
  static void fromScriptValue(const QScriptValue &obj, QxrdPowderPoint &pt);

  void linkTo(QSpinBox *n1SpinBox, QSpinBox *n2SpinBox, QDoubleSpinBox *xSpinBox, QDoubleSpinBox *ySpinBox);
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

class QxrdPowderPointSpinBoxHelper : public QObject {
  Q_OBJECT

public:
  QxrdPowderPointSpinBoxHelper(QSpinBox *spinBox, QxrdPowderPointProperty *property, int axis);
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

class QxrdPowderPointDoubleSpinBoxHelper : public QObject {
  Q_OBJECT

public:
  QxrdPowderPointDoubleSpinBoxHelper(QDoubleSpinBox *spinBox, QxrdPowderPointProperty *property, int axis);
  void connect();

public slots:
  void setSubValue(int axis, double value, int index);
  void setValue(int value);

signals:
  void subValueChanged(int axis, double value, int index);

private:
  QDoubleSpinBox             *m_SpinBox;
  QxrdPowderPointProperty    *m_Property;
  int                         m_Axis;
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

#ifndef QT_NO_DATASTREAM

extern QDataStream &operator<<(QDataStream &stream, const QxrdPowderPoint &pt);
extern QDataStream &operator>>(QDataStream &stream, QxrdPowderPoint &pt);

#endif

#endif // QXRDPOWDERPOINTPROPERTY_H
