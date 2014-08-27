#include "qxrdpowderpointproperty.h"
#include "qcepmutexlocker.h"
#include "qcepdebug.h"
#include "qcepsettingssaver.h"
#include <QScriptEngine>
#include <stdio.h>

QxrdPowderPointProperty::QxrdPowderPointProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QxrdPowderPoint value, QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(value),
  m_Value(value)
{
}

QxrdPowderPointProperty::QxrdPowderPointProperty(QcepSettingsSaverWPtr saver,
                                                 QObject *parent,
                                                 const char *name,
                                                 int n1, int n2, int n3, double x, double y, double r1, double r2, double az,
                                                 QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(QxrdPowderPoint(n1,n2,n3,x,y,r1,r2,az)),
  m_Value(QxrdPowderPoint(n1,n2,n3,x,y,r1,r2,az))
{
}

void QxrdPowderPointProperty::registerMetaTypes()
{
  qRegisterMetaType< QxrdPowderPoint >("QxrdPowderPoint");

  qRegisterMetaTypeStreamOperators< QxrdPowderPoint >("QxrdPowderPoint");

  registerCustomSaver("QxrdPowderPoint", QxrdPowderPoint::customSaver);
}

QxrdPowderPoint QxrdPowderPointProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QxrdPowderPoint QxrdPowderPointProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

double QxrdPowderPointProperty::subValue(int axis) const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  double res = 0;

  switch (axis) {
  case 0:
    res = m_Value.n1();
    break;
  case 1:
    res = m_Value.n2();
    break;
  case 2:
    res = m_Value.n3();
    break;
  case 3:
    res = m_Value.x();
    break;
  case 4:
    res = m_Value.y();
    break;
  case 5:
    res = m_Value.r1();
    break;
  case 6:
    res = m_Value.r2();
    break;
  case 7:
    res = m_Value.az();
    break;
  }

  return res;
}

void QxrdPowderPointProperty::setValue(QxrdPowderPoint val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QxrdPowderPointProperty::setValue(CctwDoubleVector3D %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QxrdPowderPointProperty::setSubValue(int axis, double value, int index)
{
  if (index == this->index()) {
    setSubValue(axis, value);
  }
}

void QxrdPowderPointProperty::setSubValue(int axis, int value, int index)
{
  if (index == this->index()) {
    setSubValue(axis, value);
  }
}

void QxrdPowderPointProperty::setSubValue(int axis, int value)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  switch (axis) {
  case 0:
    if (value != m_Value.n1()) {
      int newIndex = incIndex(1);

      emit subValueChanged(axis, value, newIndex);

      m_Value.n1() = value;

      emit valueChanged(m_Value, newIndex);
    }
    break;

  case 1:
    if (value != m_Value.n2()) {
      int newIndex = incIndex(1);

      emit subValueChanged(axis, value, newIndex);

      m_Value.n2() = value;

      emit valueChanged(m_Value, newIndex);
    }
    break;

  case 2:
    if (value != m_Value.n3()) {
      int newIndex = incIndex(1);

      emit subValueChanged(axis, value, newIndex);

      m_Value.n3() = value;

      emit valueChanged(m_Value, newIndex);
    }
    break;
  }
}

void QxrdPowderPointProperty::setSubValue(int axis, double value)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  switch (axis) {
  case 3:
    if (value != m_Value.x()) {
      int newIndex = incIndex(1);

      emit subValueChanged(axis, value, newIndex);

      m_Value.x() = value;

      emit valueChanged(m_Value, newIndex);
    }
    break;

  case 4:
    if (value != m_Value.y()) {
      int newIndex = incIndex(1);

      emit subValueChanged(axis, value, newIndex);

      m_Value.y() = value;

      emit valueChanged(m_Value, newIndex);
    }
    break;

  case 5:
    if (value != m_Value.r1()) {
      int newIndex = incIndex(1);

      emit subValueChanged(axis, value, newIndex);

      m_Value.r1() = value;

      emit valueChanged(m_Value, newIndex);
    }
    break;

  case 6:
    if (value != m_Value.r2()) {
      int newIndex = incIndex(1);

      emit subValueChanged(axis, value, newIndex);

      m_Value.r2() = value;

      emit valueChanged(m_Value, newIndex);
    }
    break;

  case 7:
    if (value != m_Value.az()) {
      int newIndex = incIndex(1);

      emit subValueChanged(axis, value, newIndex);

      m_Value.az() = value;

      emit valueChanged(m_Value, newIndex);
    }
    break;
  }
}

QString QxrdPowderPointProperty::toString(const QxrdPowderPoint &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QString res = tr("[ %1 %2 %3 %4 %5 %6 %7 %8 ]").arg(val.n1()).arg(val.n2()).arg(val.n3()).arg(val.x()).arg(val.y()).arg(val.r1()).arg(val.r2()).arg(val.az());

  return res;
}

void QxrdPowderPointProperty::setValue(QxrdPowderPoint val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 QxrdPowderPointProperty::setValue(QxrdPowderPoint %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    int newIndex = incIndex(1);

    if (debug()) {
      printMessage(tr("%1: QxrdPowderPointProperty::setValue(QxrdPowderPoint %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    if (val.n1() != m_Value.n1()) {
      emit subValueChanged(0, val.n1(), newIndex);
    }

    if (val.n2() != m_Value.n2()) {
      emit subValueChanged(1, val.n2(), newIndex);
    }

    if (val.n3() != m_Value.n3()) {
      emit subValueChanged(2, val.n3(), newIndex);
    }

    if (val.x() != m_Value.x()) {
      emit subValueChanged(3, val.x(), newIndex);
    }

    if (val.y() != m_Value.y()) {
      emit subValueChanged(4, val.y(), newIndex);
    }

    if (val.r1() != m_Value.r1()) {
      emit subValueChanged(5, val.r1(), newIndex);
    }

    if (val.r2() != m_Value.r2()) {
      emit subValueChanged(6, val.r2(), newIndex);
    }

    if (val.az() != m_Value.az()) {
      emit subValueChanged(7, val.az(), newIndex);
    }

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, newIndex);
  }
}

void QxrdPowderPointProperty::setDefaultValue(QxrdPowderPoint val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QxrdPowderPointProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QxrdPowderPointProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QScriptValue QxrdPowderPointProperty::toScriptValue(QScriptEngine *engine, const QxrdPowderPoint &pt)
{
  QScriptValue obj = engine->newObject();

  obj.setProperty("n1", pt.n1());
  obj.setProperty("n2", pt.n2());
  obj.setProperty("n3", pt.n3());
  obj.setProperty("x",  pt.x());
  obj.setProperty("y",  pt.y());
  obj.setProperty("r1", pt.r1());
  obj.setProperty("r2", pt.r2());
  obj.setProperty("az", pt.az());

  return obj;
}

void QxrdPowderPointProperty::fromScriptValue(const QScriptValue &obj, QxrdPowderPoint &pt)
{
  pt.n1() = obj.property("n1").toInteger();
  pt.n2() = obj.property("n2").toInteger();
  pt.n3() = obj.property("n3").toInteger();
  pt.x()  = obj.property("x").toNumber();
  pt.y()  = obj.property("y").toNumber();
  pt.r1() = obj.property("r1").toNumber();
  pt.r2() = obj.property("r2").toNumber();
  pt.az() = obj.property("az").toNumber();
}

void QxrdPowderPointProperty::linkTo(QSpinBox *n1SpinBox,
                                     QSpinBox *n2SpinBox,
                                     QSpinBox *n3SpinBox,
                                     QDoubleSpinBox *xSpinBox,
                                     QDoubleSpinBox *ySpinBox,
                                     QDoubleSpinBox *r1SpinBox,
                                     QDoubleSpinBox *r2SpinBox,
                                     QDoubleSpinBox *azSpinBox)
{
  if (n1SpinBox) {
    linkTo(0, n1SpinBox);
  }

  if (n2SpinBox) {
    linkTo(1, n2SpinBox);
  }

  if (n3SpinBox) {
    linkTo(2, n3SpinBox);
  }

  if (xSpinBox) {
    linkTo(3, xSpinBox);
  }

  if (ySpinBox) {
    linkTo(4, ySpinBox);
  }

  if (r1SpinBox) {
    linkTo(5, r1SpinBox);
  }

  if (r2SpinBox) {
    linkTo(6, r2SpinBox);
  }

  if (azSpinBox) {
    linkTo(7, azSpinBox);
  }
}

void QxrdPowderPointProperty::linkTo(int axis, QSpinBox *spinBox)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (axis>=0 && axis<=2) {
    QxrdPowderPointPropertySpinBoxHelper *helper
        = new QxrdPowderPointPropertySpinBoxHelper(spinBox, this, axis);

    helper->moveToThread(spinBox->thread());
    helper->connect();

    spinBox -> setValue((int) subValue(axis));
    spinBox -> setKeyboardTracking(false);

    setWidgetToolTip(spinBox);

    connect(this, SIGNAL(subValueChanged(int,int,int)), helper, SLOT(setSubValue(int,int,int)));
    connect(helper, SIGNAL(subValueChanged(int,int,int)), this, SLOT(setSubValue(int,int,int)));
  }
}

void QxrdPowderPointProperty::linkTo(int axis, QDoubleSpinBox *spinBox)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (axis>=3 && axis<=7) {
    QxrdPowderPointPropertyDoubleSpinBoxHelper *helper
        = new QxrdPowderPointPropertyDoubleSpinBoxHelper(spinBox, this, axis);

    helper->moveToThread(spinBox->thread());
    helper->connect();

    spinBox -> setValue(subValue(axis));
    spinBox -> setKeyboardTracking(false);

    setWidgetToolTip(spinBox);

    connect(this, SIGNAL(subValueChanged(int,double,int)), helper, SLOT(setSubValue(int,double,int)));
    connect(helper, SIGNAL(subValueChanged(int,double,int)), this, SLOT(setSubValue(int,double,int)));
  }
}

QxrdPowderPointPropertySpinBoxHelper::QxrdPowderPointPropertySpinBoxHelper
  (QSpinBox *spinBox, QxrdPowderPointProperty *property, int axis)
  : QObject(spinBox),
    m_SpinBox(spinBox),
    m_Property(property),
    m_Axis(axis)
{
}

void QxrdPowderPointPropertySpinBoxHelper::connect()
{
  CONNECT_CHECK(QObject::connect(m_SpinBox, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)), Qt::DirectConnection));
}

void QxrdPowderPointPropertySpinBoxHelper::setSubValue(int axis, int value, int index)
{
  if (m_Property->index() == index) {
    if (m_Axis == axis) {
      if (m_SpinBox->value() != value) {
        bool block = m_SpinBox->blockSignals(true);
        m_SpinBox->setValue(value);
        m_SpinBox->blockSignals(block);
      }
    }
  }
}

void QxrdPowderPointPropertySpinBoxHelper::setValue(int value)
{
  emit subValueChanged(m_Axis, value, m_Property->incIndex(1));
}

QxrdPowderPointPropertyDoubleSpinBoxHelper::QxrdPowderPointPropertyDoubleSpinBoxHelper
  (QDoubleSpinBox *spinBox, QxrdPowderPointProperty *property, int axis)
  : QObject(spinBox),
    m_DoubleSpinBox(spinBox),
    m_Property(property),
    m_Axis(axis)
{
}

void QxrdPowderPointPropertyDoubleSpinBoxHelper::connect()
{
  CONNECT_CHECK(QObject::connect(m_DoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)), Qt::DirectConnection));
}

void QxrdPowderPointPropertyDoubleSpinBoxHelper::setSubValue(int axis, double value, int index)
{
  if (m_Property->index() == index) {
    if (m_Axis == axis) {
      if (m_DoubleSpinBox->value() != value) {
        bool block = m_DoubleSpinBox->blockSignals(true);
        m_DoubleSpinBox->setValue(value);
        m_DoubleSpinBox->blockSignals(block);
      }
    }
  }
}

void QxrdPowderPointPropertyDoubleSpinBoxHelper::setValue(double value)
{
  emit subValueChanged(m_Axis, value, m_Property->incIndex(1));
}

QxrdPowderPointVectorProperty::QxrdPowderPointVectorProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QxrdPowderPointVector value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QxrdPowderPointVector QxrdPowderPointVectorProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QxrdPowderPointVector QxrdPowderPointVectorProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QxrdPowderPointVectorProperty::setValue(QxrdPowderPointVector val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QxrdPowderPointVectorProperty::setValue(QxrdPowderPointVector %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QxrdPowderPointVectorProperty::clear()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setValue(QxrdPowderPointVector());
}

void QxrdPowderPointVectorProperty::appendValue(QxrdPowderPoint val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QxrdPowderPointVector list = value();
  list.append(val);

  setValue(list);
}

QString QxrdPowderPointVectorProperty::toString(const QxrdPowderPointVector &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QString res = "[";
  int ct = val.count();

  for (int i=0; i<ct; i++) {
    if (i<(ct-1)) {
      res += tr("%1,\n").arg(val[i].toString());
    } else {
      res += tr("%1").arg(val[i].toString());
    }
  }

  res += "]";

  return res;
}

void QxrdPowderPointVectorProperty::setValue(QxrdPowderPointVector val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 QxrdPowderPointVectorProperty::setValue(QxrdPowderPointVector %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QxrdPowderPointVectorProperty::setValue(QxrdPowderPointVector %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QxrdPowderPointVectorProperty::setDefaultValue(QxrdPowderPointVector val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QxrdPowderPointVectorProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QxrdPowderPointVectorProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

void QxrdPowderPointVectorProperty::registerMetaTypes()
{
  qRegisterMetaType< QxrdPowderPointVector >("QxrdPowderPointVector");

  qRegisterMetaTypeStreamOperators< QxrdPowderPointVector >("QxrdPowderPointVector");

  registerCustomSaver("QxrdPowderPointVector", QxrdPowderPointVector::customSaver);
}

QScriptValue QxrdPowderPointVectorProperty::toScriptValue(QScriptEngine *engine,
                                                          const QxrdPowderPointVector &vec)
{
  int n = vec.count();

  QScriptValue res = engine->newArray(n);

  for (int i=0; i<n; i++) {
    const QxrdPowderPoint &pt = vec.at(i);
    QScriptValue obj = engine->newObject();

    obj.setProperty("n1", pt.n1());
    obj.setProperty("n2", pt.n2());
    obj.setProperty("n3", pt.n3());
    obj.setProperty("x", pt.x());
    obj.setProperty("y", pt.y());
    obj.setProperty("r1", pt.r1());
    obj.setProperty("r2", pt.r2());
    obj.setProperty("az", pt.az());

    res.setProperty(i, obj);
  }

  return res;
}

void QxrdPowderPointVectorProperty::fromScriptValue(const QScriptValue &obj,
                                                    QxrdPowderPointVector &vec)
{
  int n=obj.property("length").toInteger();

  vec.resize(n);

  for (int i=0; i<n; i++) {
    QScriptValue pt = obj.property(i);

    if (pt.isValid()) {
      vec[i].n1() = pt.property("n1").toInteger();
      vec[i].n2() = pt.property("n2").toInteger();
      vec[i].n3() = pt.property("n3").toInteger();
      vec[i].x()  = pt.property("x").toNumber();
      vec[i].y()  = pt.property("y").toNumber();
      vec[i].r1() = pt.property("r1").toNumber();
      vec[i].r2() = pt.property("r2").toNumber();
      vec[i].az() = pt.property("az").toNumber();
    }
  }
}

#ifndef QT_NO_DATASTREAM

QDataStream &operator<<(QDataStream &stream, const QxrdPowderPoint &pt)
{
  stream << pt.n1() << pt.n2() << pt.n3() << pt.x() << pt.y() << pt.r1() << pt.r2() << pt.az();

  return stream;
}

QDataStream &operator>>(QDataStream &stream, QxrdPowderPoint &pt)
{
  stream >> pt.n1() >> pt.n2() >> pt.n3()>> pt.x() >> pt.y() >> pt.r1() >> pt.r2() >> pt.az();

  return stream;
}

#endif
