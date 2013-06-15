#include "qcepmatrix3x3property.h"
#include "qcepmutexlocker.h"
#include "qcepdebug.h"
#include "qcepsettingssaver.h"

QcepMatrix3x3Property::QcepMatrix3x3Property(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QMatrix3x3 value, QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(value),
  m_Value(value)
{
}

void QcepMatrix3x3Property::registerMetaTypes()
{
  qRegisterMetaType< QMatrix3x3 >("QMatrix3x3");

  qRegisterMetaTypeStreamOperators< QMatrix3x3 >("QMatrix3x3");
}

QMatrix3x3 QcepMatrix3x3Property::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QMatrix3x3 QcepMatrix3x3Property::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepMatrix3x3Property::setValue(QMatrix3x3 val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepMatrix3x3Property::setValue(QMatrix3x3 %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepMatrix3x3Property::incValue(QMatrix3x3 step)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepMatrix3x3Property::incValue(QMatrix3x3 %2...)")
                 .arg(name()).arg(toString(step)));
  }

  m_Value += step;

  QcepSettingsSaverPtr saver(m_Saver);

  if (saver) {
    saver->changed(this);
  }

  emit valueChanged(m_Value, incIndex(1));
}

QString QcepMatrix3x3Property::toString(const QMatrix3x3 &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QString res = "[";

  for (int row=0; row<3; row++) {
    res += " [ ";
    for (int col=0; col<3; col++) {
      res += tr("%1 ").arg(val(row, col));
    }
    res += "]";
  }

  res += "]";

  return res;
}

void QcepMatrix3x3Property::setValue(QMatrix3x3 val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 QcepMatrix3x3Property::setValue(QMatrix3x3 %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepMatrix3x3Property::setValue(QMatrix3x3 %2) [%3]")
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

void QcepMatrix3x3Property::setDefaultValue(QMatrix3x3 val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepMatrix3x3Property::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepMatrix3x3Property::resetValue").arg(name()));
  }

  setValue(defaultValue());
}


