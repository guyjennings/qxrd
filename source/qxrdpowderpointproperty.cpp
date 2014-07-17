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
                                                 int n1, int n2, double x, double y,
                                                 QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(QxrdPowderPoint(n1,n2,x,y)),
  m_Value(QxrdPowderPoint(n1,n2,x,y))
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
    res = m_Value.x();
    break;
  case 3:
    res = m_Value.y();
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
  }
}

void QxrdPowderPointProperty::setSubValue(int axis, double value)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  switch (axis) {
  case 2:
    if (value != m_Value.x()) {
      int newIndex = incIndex(1);

      emit subValueChanged(axis, value, newIndex);

      m_Value.x() = value;

      emit valueChanged(m_Value, newIndex);
    }
    break;

  case 3:
    if (value != m_Value.y()) {
      int newIndex = incIndex(1);

      emit subValueChanged(axis, value, newIndex);

      m_Value.y() = value;

      emit valueChanged(m_Value, newIndex);
    }
    break;
  }
}

QString QxrdPowderPointProperty::toString(const QxrdPowderPoint &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QString res = tr("[ %1 %2 %3 %4 ]").arg(val.n1()).arg(val.n2()).arg(val.x()).arg(val.y());

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

    if (val.x() != m_Value.x()) {
      emit subValueChanged(2, val.x(), newIndex);
    }

    if (val.y() != m_Value.y()) {
      emit subValueChanged(3, val.y(), newIndex);
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
  QScriptValue obj = engine->newArray(4);

  obj.setProperty(0, pt.n1());
  obj.setProperty(1, pt.n2());
  obj.setProperty(2, pt.x());
  obj.setProperty(3, pt.y());

  return obj;
}

void QxrdPowderPointProperty::fromScriptValue(const QScriptValue &obj, QxrdPowderPoint &pt)
{
  pt.n1() = (int) obj.property(0).toInteger();
  pt.n2() = (int) obj.property(1).toInteger();
  pt.x() = obj.property(2).toNumber();
  pt.y() = obj.property(3).toNumber();
}

void QxrdPowderPointProperty::linkTo(QSpinBox *n1SpinBox,
                                     QSpinBox *n2SpinBox,
                                     QDoubleSpinBox *xSpinBox,
                                     QDoubleSpinBox *ySpinBox)
{
  if (n1SpinBox) {
    linkTo(0, n1SpinBox);
  }

  if (n2SpinBox) {
    linkTo(1, n2SpinBox);
  }

  if (xSpinBox) {
    linkTo(2, xSpinBox);
  }

  if (ySpinBox) {
    linkTo(3, ySpinBox);
  }
}

void QxrdPowderPointProperty::linkTo(int axis, QSpinBox *spinBox)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (axis==0 || axis==1) {
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

  if (axis==2 || axis==3) {
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

#ifndef QT_NO_DATASTREAM

QDataStream &operator<<(QDataStream &stream, const QxrdPowderPoint &pt)
{
  stream << pt.n1() << pt.n2() << pt.x() << pt.y();

  return stream;
}

QDataStream &operator>>(QDataStream &stream, QxrdPowderPoint &pt)
{
  stream >> pt.n1() >> pt.n2() >> pt.x() >> pt.y();

  return stream;
}

#endif
