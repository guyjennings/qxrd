#include "qxrdpowderpointproperty.h"
#include "qcepmutexlocker.h"
#include "qcepdebug.h"
#include <QScriptEngine>
#include <stdio.h>
#include "qcepobject.h"

QxrdPowderPointProperty::QxrdPowderPointProperty(QcepObject *parent,
                                                 const char *name,
                                                 QxrdPowderPoint value,
                                                 QString toolTip) :
  QcepProperty(parent, name, toolTip),
  m_Default(value),
  m_Value(value)
{
}

QxrdPowderPointProperty::QxrdPowderPointProperty(QcepObject *parent,
                                                 const char *name,
                                                 int n1, int n2, int n3,
                                                 double x, double y, double r1, double r2, double az,
                                                 QString toolTip) :
  QcepProperty(parent, name, toolTip),
  m_Default(QxrdPowderPoint(n1,n2,n3,x,y,r1,r2,az)),
  m_Value(QxrdPowderPoint(n1,n2,n3,x,y,r1,r2,az))
{
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

    if (m_Parent) {
      m_Parent->propertyChanged(this);
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

    connect(this, (void (QxrdPowderPointProperty::*)(int, int, int)) &QxrdPowderPointProperty::subValueChanged,
            helper, &QxrdPowderPointPropertySpinBoxHelper::setSubValue);
    connect(helper, &QxrdPowderPointPropertySpinBoxHelper::subValueChanged,
            this, (void (QxrdPowderPointProperty::*)(int, int, int)) &QxrdPowderPointProperty::setSubValue);
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

    connect(this, (void (QxrdPowderPointProperty::*)(int, double, int)) &QxrdPowderPointProperty::subValueChanged,
            helper, &QxrdPowderPointPropertyDoubleSpinBoxHelper::setSubValue);
    connect(helper, &QxrdPowderPointPropertyDoubleSpinBoxHelper::subValueChanged,
            this, (void (QxrdPowderPointProperty::*)(int, double, int)) &QxrdPowderPointProperty::setSubValue);
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
  CONNECT_CHECK(QObject::connect(m_SpinBox, (void (QSpinBox::*)(int)) &QSpinBox::valueChanged,
                                 this, &QxrdPowderPointPropertySpinBoxHelper::setValue, Qt::DirectConnection));
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
  CONNECT_CHECK(QObject::connect(m_DoubleSpinBox, (void (QDoubleSpinBox::*)(double)) &QDoubleSpinBox::valueChanged,
                                 this, &QxrdPowderPointPropertyDoubleSpinBoxHelper::setValue, Qt::DirectConnection));
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

QxrdPowderPointVectorProperty::QxrdPowderPointVectorProperty(QcepObject *parent,
                                                             const char *name,
                                                             QxrdPowderPointVector value,
                                                             QString toolTip)
  : QcepProperty(parent, name, toolTip),
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

    if (m_Parent) {
      m_Parent->propertyChanged(this);
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