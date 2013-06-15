#include "qcepvector3dproperty.h"
#include "qcepmutexlocker.h"
#include "qcepdebug.h"
#include "qcepsettingssaver.h"

QcepVector3DProperty::QcepVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QVector3D value, QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(value),
  m_Value(value)
{
}

void QcepVector3DProperty::registerMetaTypes()
{
  qRegisterMetaType< QVector3D >("QVector3D");

  qRegisterMetaTypeStreamOperators< QVector3D >("QVector3D");
}

QVector3D QcepVector3DProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QVector3D QcepVector3DProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepVector3DProperty::setValue(QVector3D val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepVector3DProperty::setValue(QVector3D %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepVector3DProperty::incValue(QVector3D step)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepVector3DProperty::incValue(QVector3D %2...)")
                 .arg(name()).arg(toString(step)));
  }

  m_Value += step;

  QcepSettingsSaverPtr saver(m_Saver);

  if (saver) {
    saver->changed(this);
  }

  emit valueChanged(m_Value, incIndex(1));
}

QString QcepVector3DProperty::toString(const QVector3D &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QString res = tr("[ %1 %2 %3 ]").arg(val.x()).arg(val.y()).arg(val.z());

  return res;
}

void QcepVector3DProperty::setValue(QVector3D val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 QcepVector3DProperty::setValue(QVector3D %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepVector3DProperty::setValue(QVector3D %2) [%3]")
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

void QcepVector3DProperty::setDefaultValue(QVector3D val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepVector3DProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepVector3DProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}


