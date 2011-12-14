#include "qcepproperty.h"
#include "qxrdapplication.h"
#include "qxrdsettingssaver.h"

#include <QMutexLocker>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QAbstractButton>
#include <QMetaProperty>
#include <QSettings>
#include <stdio.h>

QcepProperty::QcepProperty(QxrdSettingsSaver *saver, QObject *parent, const char *name)
  : QObject(),
    m_Mutex(QMutex::Recursive),
    m_Saver(saver),
    m_Debug(false),
    m_IsStored(false),
    m_WasLoaded(false),
    m_Name(name)
{
  setObjectName(name);

  if (parent) {
    const QMetaObject   *metaObj = parent->metaObject();

    if (metaObj) {
      int i =  metaObj->indexOfProperty(name);

      if (i>=0) {
        QMetaProperty metaProperty = metaObj->property(i);

        if (metaProperty.isStored()) {
          m_IsStored = true;
        } else {
          m_IsStored = false;
        }
      }
    }
  }

  if (m_Saver && !m_IsStored) {
    printf("Warning: property %s has saver but is not stored\n", name);
  }
}

int QcepProperty::wasLoaded() const
{
  return m_WasLoaded;
}

void QcepProperty::setWasLoaded(int loaded)
{
  m_WasLoaded = loaded;
}

QString QcepProperty::name() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Name;
}

int QcepProperty::index()
{
  return m_Index.fetchAndAddOrdered(0);
}

int QcepProperty::incIndex(int step)
{
  return m_Index.fetchAndAddOrdered(step) + step;
}

void QcepProperty::setDebug(int dbg)
{
  m_Debug = dbg;
}

int QcepProperty::debug() const
{
  return m_Debug;
}

void QcepProperty::registerMetaTypes()
{
  qRegisterMetaType< QcepDoubleVector >("QcepDoubleVector");
  qRegisterMetaType< QcepBoolVector >("QcepBoolVector");
  qRegisterMetaType< QcepIntVector >("QcepIntVector");
  qRegisterMetaType< QcepStringVector >("QcepStringVector");
  qRegisterMetaType< QcepDoubleList >("QcepDoubleList");
  qRegisterMetaType< QcepBoolList >("QcepBoolList");
  qRegisterMetaType< QcepIntList >("QcepIntList");
  qRegisterMetaType< QcepStringList >("QcepStringList");
  qRegisterMetaType< QVariant >("QVariant");

  qRegisterMetaTypeStreamOperators< QcepDoubleVector >("QcepDoubleVector");
  qRegisterMetaTypeStreamOperators< QcepBoolVector >("QcepBoolVector");
  qRegisterMetaTypeStreamOperators< QcepIntVector >("QcepIntVector");
  qRegisterMetaTypeStreamOperators< QcepStringVector >("QcepStringVector");
  qRegisterMetaTypeStreamOperators< QcepDoubleList >("QcepDoubleList");
  qRegisterMetaTypeStreamOperators< QcepBoolList >("QcepBoolList");
  qRegisterMetaTypeStreamOperators< QcepIntList >("QcepIntList");
  qRegisterMetaTypeStreamOperators< QcepStringList >("QcepStringList");
}

void QcepProperty::setSaver(QxrdSettingsSaver *saver)
{
  m_Saver = saver;
}

void QcepProperty::writeSettings(QObject *object, const QMetaObject *meta, QString groupName, QSettings *settings)
{
  if (settings) {
    int count = meta->propertyCount();
    int offset = meta->propertyOffset();

    settings->beginGroup(groupName);

    for (int i=offset; i<count; i++) {
      QMetaProperty metaproperty = meta->property(i);

      if (metaproperty.isStored()) {
        const char *name = metaproperty.name();
        QVariant value = object -> property(name);

        if (qcepDebug(DEBUG_PREFS | DEBUG_PROPERTIES)) {
          g_Application->printMessage(
                tr("Save %1/%2 = %3 [%4]")
                .arg(groupName).arg(name)
                .arg(value.toString()).arg(value.typeName()));
        }

        settings->setValue(name, value);
      }
    }

    QByteArray name;

    foreach (name, object->dynamicPropertyNames()) {
      settings->setValue(name.data(), object->property(name.data()));
    }

    settings->endGroup();
  }
}

void QcepProperty::readSettings(QObject *object, const QMetaObject *meta, QString groupName, QSettings *settings)
{
  if (settings) {
    settings->beginGroup(groupName);

    QStringList keys = settings->childKeys();

    foreach (QString key, keys) {
      if (qcepDebug(DEBUG_PREFS | DEBUG_PROPERTIES)) {
        g_Application->printMessage(
              tr("Load %1/%2 = %3 [%4]")
              .arg(groupName).arg(key)
              .arg(settings->value(key).toString())
              .arg(settings->value(key).typeName()));
      }

      int metaindex = meta->indexOfProperty(qPrintable(key));

      if (metaindex >= 0) {
        QMetaProperty metaproperty = meta->property(metaindex);

        if (metaproperty.isStored()) {
          QcepProperty *property = object->findChild<QcepProperty *>(key);

          if (property) {
            property->setWasLoaded(true);
          }

          object -> setProperty(qPrintable(key), settings->value(key));
        } else {
          if (qcepDebug(DEBUG_PREFS | DEBUG_PROPERTIES)) {
            g_Application->printMessage(
                  tr("property %1 of %2 not stored").arg(key)
                  .arg(meta -> className()));
          }
        }
      } else {
        if (qcepDebug(DEBUG_PREFS | DEBUG_PROPERTIES)) {
          g_Application->printMessage(
                tr("property %1 of %2 does not exist")
                .arg(key).arg(meta -> className()));
        }
      }
    }

    settings->endGroup();
  }
}

void QcepProperty::dumpMetaData(const QMetaObject *meta)
{
  while (meta) {
    g_Application->printMessage(
          tr("MetaData for class %1").arg(meta -> className()));
    g_Application->printMessage(
          tr(" superClass = %1").HEXARG((void*) meta -> superClass()));
    g_Application->printMessage(
          tr(" methodCount = %1, methodOffset = %2")
          .arg(meta->methodCount()).arg(meta->methodOffset()));
    g_Application->printMessage(
          tr(" propertyCount = %1, propertyOffset = %2")
          .arg(meta->propertyCount()).arg(meta->propertyOffset()));

    meta = meta->superClass();
  }
}

QcepDoubleProperty::QcepDoubleProperty(QxrdSettingsSaver *saver, QObject *parent, const char *name, double value)
  : QcepProperty(saver, parent, name),
    m_Default(value),
    m_Value(value)
{
}

double QcepDoubleProperty::value() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Value;
}

double QcepDoubleProperty::defaultValue() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Default;
}

void QcepDoubleProperty::setValue(double val, int index)
{
  if (debug()) {
    g_Application->printMessage(
          tr("%1 QcepDoubleProperty::setValue(double %2, int %3) [%4]")
          .arg(name())
          .arg(val)
          .arg(index)
          .arg(this->index()));
  }

//  if (index == this->index()) {
    setValue(val);
//  }
}

void QcepDoubleProperty::setValue(double val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
          tr("%1: QcepDoubleProperty::setValue(double %2)")
          .arg(name())
          .arg(val));
  }

  if (val != m_Value) {
    if (g_Application && debug()) {
      g_Application->printMessage(
            tr("%1: QcepDoubleProperty::setValue(double %2) [%3]")
            .arg(name())
            .arg(val)
            .arg(index()));
    }

    QMutexLocker lock(&m_Mutex);

    m_Value = val;

    if (m_Saver) {
      m_Saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
    emit valueChanged(tr("%1").arg(m_Value));
  }
}

void QcepDoubleProperty::incValue(double step)
{

  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    g_Application->printMessage(
          tr("%1: QcepDoubleProperty::incValue(double %2)")
        .arg(name()).arg(step));
  }

  if (step) {
    QMutexLocker lock(&m_Mutex);

    m_Value += step;

    if (m_Saver) {
      m_Saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
    emit valueChanged(tr("%1").arg(m_Value));
  }
}

void QcepDoubleProperty::setDefaultValue(double val)
{
  QMutexLocker lock(&m_Mutex);

  m_Default = val;
}

void QcepDoubleProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepDoubleProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

void QcepDoubleProperty::linkTo(QDoubleSpinBox *spinBox)
{
  if (qcepDebug(DEBUG_PROPERTIES || debug())) {
    g_Application->printMessage(
        tr("%1: QcepDoubleProperty::linkTo(QDoubleSpinBox *%2)")
           .arg(name()).HEXARG(spinBox));
  }

  QcepDoublePropertyDoubleSpinBoxHelper *helper
      = new QcepDoublePropertyDoubleSpinBoxHelper(spinBox, this);

  helper->moveToThread(spinBox->thread());
  helper->connect();

  spinBox -> setValue(value());
  spinBox -> setKeyboardTracking(false);

  connect(this,   SIGNAL(valueChanged(double, int)), helper, SLOT(setValue(double, int)));
  connect(helper, SIGNAL(valueChanged(double, int)), this,   SLOT(setValue(double, int)));
}

void QcepDoubleProperty::linkTo(QLabel *label)
{
  label -> setText(tr("%1").arg(value()));

  connect(this, SIGNAL(valueChanged(QString)), label, SLOT(setText(QString)));
}

QcepDoublePropertyDoubleSpinBoxHelper::QcepDoublePropertyDoubleSpinBoxHelper(QDoubleSpinBox *spinBox, QcepDoubleProperty *property)
  : QObject(spinBox),
    m_DoubleSpinBox(spinBox),
    m_Property(property)
{
}

void QcepDoublePropertyDoubleSpinBoxHelper::connect()
{
  CONNECT_CHECK(QObject::connect(m_DoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)), Qt::DirectConnection));
}

void QcepDoublePropertyDoubleSpinBoxHelper::setValue(double value, int index)
{
  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    g_Application->printMessage(
          tr("%1: QcepDoublePropertyDoubleSpinBoxHelper::setValue(double %2, int %3) [%4,%5]")
          .arg(m_Property->name()).arg(value).arg(index).arg(m_Property->index()).arg(m_DoubleSpinBox->value()));
  }

  if (m_Property->index() == index) {
    if (m_DoubleSpinBox->value() != value) {
      if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
        g_Application->printMessage(
              tr("%1: QcepDoublePropertyDoubleSpinBoxHelper spinBox %2 set to %3")
              .arg(m_Property->name())
              .arg(m_DoubleSpinBox->objectName())
              .arg(value));
      }

      bool block = m_DoubleSpinBox->blockSignals(true);
      m_DoubleSpinBox->setValue(value);
      m_DoubleSpinBox->blockSignals(block);
    }
  }
}

void QcepDoublePropertyDoubleSpinBoxHelper::setValue(double value)
{
  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    g_Application->printMessage(
          tr("%1: QcepDoublePropertyDoubleSpinBoxHelper::setValue(double %2)")
          .arg(m_Property->name()).arg(value));
  }

  emit valueChanged(value, m_Property->incIndex(1));
}

QcepIntProperty::QcepIntProperty(QxrdSettingsSaver *saver, QObject *parent, const char *name, int value)
  : QcepProperty(saver, parent, name),
    m_Default(value),
    m_Value(value)
{
}

int QcepIntProperty::value() const
{
  return m_Value;
}

int QcepIntProperty::defaultValue() const
{
  return m_Default;
}

void QcepIntProperty::setValue(int val, int index)
{
  if (debug()) {
    g_Application->printMessage(
          tr("%1 QcepIntProperty::setValue(int %2, int %3) [%4]")
          .arg(name())
          .arg(val)
          .arg(index)
          .arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepIntProperty::setValue(int val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    if (g_Application) {
      g_Application->printMessage(
            tr("%1: QcepIntProperty::setValue(int %2)")
            .arg(name()).arg(val));
    }
  }

  if (val != m_Value) {
    if (g_Application && debug()) {
      g_Application->printMessage(
            tr("%1: QcepIntProperty::setValue(int %2) [%3]")
            .arg(name())
            .arg(val)
            .arg(index()));
    }

    m_Value.fetchAndStoreOrdered(val);

    if (m_Saver) {
      m_Saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
    emit valueChanged(tr("%1").arg(m_Value));
  }
}

void QcepIntProperty::incValue(int step)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    g_Application->printMessage(
          tr("%1: QcepIntProperty::incValue(int %2)")
        .arg(name()).arg(step));
  }

  if (step) {
    m_Value.fetchAndAddOrdered(step);

    if (m_Saver) {
      m_Saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
    emit valueChanged(tr("%1").arg(m_Value));
  }
}

void QcepIntProperty::setDefaultValue(int val)
{
  m_Default.fetchAndStoreOrdered(val);
}

void QcepIntProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    g_Application->printMessage(
        tr("%1: QcepIntProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

void QcepIntProperty::linkTo(QSpinBox *spinBox)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    g_Application->printMessage(
          tr("%1: QcepIntProperty::linkTo(QSpinBox *%2)")
          .arg(name()).HEXARG(spinBox));
  }

  QcepIntPropertySpinBoxHelper *helper
      = new QcepIntPropertySpinBoxHelper(spinBox, this);

  helper->moveToThread(spinBox->thread());
  helper->connect();

  spinBox -> setValue(value());
  spinBox -> setKeyboardTracking(false);

  connect(this,   SIGNAL(valueChanged(int, int)), helper, SLOT(setValue(int, int)));
  connect(helper, SIGNAL(valueChanged(int, int)), this,   SLOT(setValue(int, int)));
}

void QcepIntProperty::linkTo(QComboBox *comboBox)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    g_Application->printMessage(
          tr("%1: QcepIntProperty::linkTo(QComboBox *%2)")
          .arg(name()).HEXARG(comboBox));
  }

  QcepIntPropertyComboBoxHelper *helper
      = new QcepIntPropertyComboBoxHelper(comboBox, this);

  helper->moveToThread(comboBox->thread());
  helper->connect();

  comboBox -> setCurrentIndex(value());

  connect(this,   SIGNAL(valueChanged(int, int)),        helper, SLOT(setCurrentIndex(int, int)));
  connect(helper, SIGNAL(currentIndexChanged(int, int)), this,   SLOT(setValue(int, int)));
}

void QcepIntProperty::linkTo(QLabel *label)
{
  {
    label -> setText(tr("%1").arg(value()));

    connect(this, SIGNAL(valueChanged(QString)), label, SLOT(setText(QString)));
  }
}

QcepIntPropertySpinBoxHelper::QcepIntPropertySpinBoxHelper(QSpinBox *spinBox, QcepIntProperty *property)
  : QObject(spinBox),
    m_SpinBox(spinBox),
    m_Property(property)
{
}

void QcepIntPropertySpinBoxHelper::connect()
{
  CONNECT_CHECK(QObject::connect(m_SpinBox, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)), Qt::DirectConnection));
}

void QcepIntPropertySpinBoxHelper::setValue(int value, int index)
{
  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    g_Application->printMessage(
          tr("%1: QcepIntPropertySpinBoxHelper::setValue(int %2, int %3) [%4,%5]")
          .arg(m_Property->name()).arg(value).arg(index).arg(m_Property->index()).arg(m_SpinBox->value()));
  }

  if (m_Property->index() == index) {
    if (m_SpinBox->value() != value) {
      if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
        g_Application->printMessage(
              tr("%1: QcepIntPropertySpinBoxHelper spinBox %2 set to %3")
              .arg(m_Property->name())
              .arg(m_SpinBox->objectName())
            .arg(value));
      }

      bool block = m_SpinBox->blockSignals(true);
      m_SpinBox->setValue(value);
      m_SpinBox->blockSignals(block);
    }
  }
}

void QcepIntPropertySpinBoxHelper::setValue(int value)
{
  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    g_Application->printMessage(
          tr("%1: QcepIntPropertySpinBoxHelper::setValue(int %2)")
          .arg(m_Property->name()).arg(value));
  }

  emit valueChanged(value, m_Property->incIndex(1));
}

QcepIntPropertyComboBoxHelper::QcepIntPropertyComboBoxHelper(QComboBox *comboBox, QcepIntProperty *property)
  : QObject(comboBox),
    m_ComboBox(comboBox),
    m_Property(property)
{
}

void QcepIntPropertyComboBoxHelper::connect()
{
  CONNECT_CHECK(QObject::connect(m_ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentIndex(int)), Qt::DirectConnection));
}

void QcepIntPropertyComboBoxHelper::setCurrentIndex(int value, int index)
{
  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    g_Application->printMessage(
          tr("%1: QcepIntPropertyComboBoxHelper::setValue(int %2, int %3) [%4,%5]")
          .arg(m_Property->name())
          .arg(value)
          .arg(index)
          .arg(m_Property->index()).arg(m_ComboBox->currentIndex()));
  }

  if (m_Property->index() == index) {
    if (m_ComboBox->currentIndex() != value) {
      if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
        g_Application->printMessage(
              tr("%1: QcepIntPropertyComboBoxHelper comboBox %2 set to %3")
              .arg(m_Property->name())
              .arg(m_ComboBox->objectName())
              .arg(value));
      }

      bool block = m_ComboBox->blockSignals(true);
      m_ComboBox->setCurrentIndex(value);
      m_ComboBox->blockSignals(block);
    }
  }
}

void QcepIntPropertyComboBoxHelper::setCurrentIndex(int value)
{
  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    g_Application->printMessage(
          tr("%1: QcepIntPropertyComboBoxHelper::setValue(int %2)")
          .arg(m_Property->name()).arg(value));
  }

  emit currentIndexChanged(value, m_Property->incIndex(1));
}

QcepBoolProperty::QcepBoolProperty(QxrdSettingsSaver *saver, QObject *parent, const char *name, bool value)
  : QcepProperty(saver, parent, name),
    m_Default(value),
    m_Value(value)
{
}

bool QcepBoolProperty::value() const
{
  return m_Value;
}

bool QcepBoolProperty::defaultValue() const
{
  return m_Default;
}

void QcepBoolProperty::setValue(bool val, int index)
{
  if (debug()) {
    g_Application->printMessage(
          tr("%1 QcepBoolProperty::setValue(bool %2, int %3) [%4]")
          .arg(name())
          .arg(val)
          .arg(index)
          .arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepBoolProperty::setValue(bool val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    if (g_Application) {
      g_Application->printMessage(
            tr("%1: QcepBoolProperty::setValue(bool %2)")
            .arg(name())
            .arg(val));
    }
  }

  if (val != m_Value) {
    if (g_Application && debug()) {
      g_Application->printMessage(
            tr("%1: QcepBoolProperty::setValue(bool %2) [%3]")
            .arg(name())
            .arg(val)
            .arg(index()));
    }

    m_Value.fetchAndStoreOrdered(val);

    if (m_Saver) {
      m_Saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepBoolProperty::setDefaultValue(bool val)
{
  m_Default.fetchAndStoreOrdered(val);
}

void QcepBoolProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES || debug())) {
    g_Application->printMessage(
        tr("%1: QcepBoolProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

void QcepBoolProperty::linkTo(QAbstractButton *button)
{
  if (qcepDebug(DEBUG_PROPERTIES || debug())) {
    g_Application->printMessage(
          tr("%1: QcepBoolProperty::linkTo(QAbstractButton *%2)")
          .arg(name()).HEXARG(button));
  }

  QcepBoolPropertyButtonHelper *helper
      = new QcepBoolPropertyButtonHelper(button, this);

  helper->moveToThread(button->thread());
  helper->connect();

  button -> setChecked(value());

  connect(this,   SIGNAL(valueChanged(bool, int)), helper, SLOT(setChecked(bool, int)));
  connect(helper, SIGNAL(toggled(bool, int)),      this,   SLOT(setValue(bool, int)));
}

QcepBoolPropertyButtonHelper::QcepBoolPropertyButtonHelper(QAbstractButton *button, QcepBoolProperty *property)
  : QObject(button),
    m_Button(button),
    m_Property(property)
{
}

void QcepBoolPropertyButtonHelper::connect()
{
  CONNECT_CHECK(QObject::connect(m_Button, SIGNAL(toggled(bool)), this, SLOT(setChecked(bool)), Qt::DirectConnection));
}

void QcepBoolPropertyButtonHelper::setChecked(bool value, int index)
{
  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    g_Application->printMessage(
          tr("%1: QcepBoolPropertyButtonHelper::setChecked(bool %2, int %3) [%4,%5]")
          .arg(m_Property->name())
          .arg(value)
          .arg(index)
          .arg(m_Property->index())
          .arg(m_Button->isChecked()));
  }

  if (m_Property->index() == index) {
    if (m_Button->isChecked() != value) {
      if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
        g_Application->printMessage(
              tr("%1: QcepBoolPropertyButtonHelper button %2 set to %3")
              .arg(m_Property->name())
              .arg(m_Button->objectName())
              .arg(value));
      }

      bool block = m_Button->blockSignals(true);
      m_Button->setChecked(value);
      m_Button->blockSignals(block);
    }
  }
}

void QcepBoolPropertyButtonHelper::setChecked(bool value)
{
  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    g_Application->printMessage(
          tr("%1: QcepIntPropertySpinBoxHelper::setValue(int %2)")
          .arg(m_Property->name()).arg(value));
  }

  emit toggled(value, m_Property->incIndex(1));
}

QcepStringProperty::QcepStringProperty(QxrdSettingsSaver *saver, QObject *parent, const char *name, QString value)
  : QcepProperty(saver, parent, name),
    m_Default(value),
    m_Value(value)
{
}

QString QcepStringProperty::value() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Value;
}

QString QcepStringProperty::defaultValue() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Default;
}

void QcepStringProperty::setValue(QString val, int index)
{
  if (debug()) {
    g_Application->printMessage(
          tr("%1 QcepStringProperty::setValue(QString \"%2\", int %3) [%4]")
          .arg(name())
          .arg(val)
          .arg(index)
          .arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepStringProperty::setValue(QString val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
          tr("%1: QcepStringProperty::setValue(QString \"%2\")")
          .arg(name())
          .arg(val));
  }

  if (val != m_Value) {
    if (g_Application && debug()) {
      g_Application->printMessage(
            tr("%1: QcepStringProperty::setValue(QString \"%2\") [%3]")
            .arg(name())
            .arg(val)
            .arg(index()));
    }

    QMutexLocker lock(&m_Mutex);

    m_Value = val;

    if (m_Saver) {
      m_Saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepStringProperty::setDefaultValue(QString val)
{
  QMutexLocker lock(&m_Mutex);

  m_Default = val;
}

void QcepStringProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepStringProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

void QcepStringProperty::linkTo(QComboBox * /*comboBox*/)
{
  printf("QcepStringProperty::linkTo(QComboBox * not implemented yet\n");
}

void QcepStringProperty::linkTo(QLineEdit *lineEdit)
{
  if (qcepDebug(DEBUG_PROPERTIES || debug())) {
    g_Application->printMessage(
        tr("%1: QcepStringProperty::linkTo(QLineEdit *%2)")
        .arg(name()).HEXARG(lineEdit));
  }

  QcepStringPropertyLineEditHelper *helper
      = new QcepStringPropertyLineEditHelper(lineEdit, this);

  helper -> moveToThread(lineEdit->thread());
  helper -> connect();

  lineEdit -> setText(value());

  connect(this,   SIGNAL(valueChanged(QString, int)), helper, SLOT(setText(QString, int)));
  connect(helper, SIGNAL(textEdited(QString, int)),   this,   SLOT(setValue(QString, int)));
}

void QcepStringProperty::linkTo(QLabel *label)
{
  label -> setText(value());

  connect(this, SIGNAL(valueChanged(QString,int)), label, SLOT(setText(QString)));
}

QcepStringPropertyLineEditHelper::QcepStringPropertyLineEditHelper(QLineEdit *lineEdit, QcepStringProperty *property)
  : QObject(lineEdit),
    m_LineEdit(lineEdit),
    m_Property(property)
{
}

void QcepStringPropertyLineEditHelper::connect()
{
  CONNECT_CHECK(QObject::connect(m_LineEdit, SIGNAL(textEdited(QString)), this, SLOT(setText(QString)), Qt::DirectConnection));
}

void QcepStringPropertyLineEditHelper::setText(QString value, int index)
{
  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    g_Application->printMessage(
          tr("%1: QcepStringPropertyLineEditHelper::setText(QString \"%2\", int %3) [%4,%5]")
          .arg(m_Property->name())
          .arg(value)
          .arg(index)
          .arg(m_Property->index()).arg(m_LineEdit->text()));
  }

  if (m_Property->index() == index) {
    if (m_LineEdit->text() != value) {
      if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
        g_Application->printMessage(
              tr("%1: QcepStringPropertyLineEditHelper lineEdit %2 set to %3")
              .arg(m_Property->name())
              .arg(m_LineEdit->objectName())
              .arg(value));
      }

      bool block = m_LineEdit->blockSignals(true);
      m_LineEdit->setText(value);
      m_LineEdit->blockSignals(block);
    }
  }
}

void QcepStringPropertyLineEditHelper::setText(QString value)
{
  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    g_Application->printMessage(
          tr("%1: QcepStringPropertyLineEditHelper::setText(QString \"%2\")")
          .arg(m_Property->name()).arg(value));
  }

  emit textEdited(value, m_Property->incIndex(1));
}

QcepDateTimeProperty::QcepDateTimeProperty(QxrdSettingsSaver *saver, QObject *parent, const char *name, QDateTime value)
  : QcepProperty(saver, parent, name),
    m_Default(value),
    m_Value(value)
{
}

QDateTime QcepDateTimeProperty::value() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Value;
}

QDateTime QcepDateTimeProperty::defaultValue() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Default;
}

void QcepDateTimeProperty::setValue(QDateTime val, int index)
{
  if (debug()) {
    g_Application->printMessage(
          tr("%1 QcepDateTimeProperty::setValue(QDateTime \"%2\", int %3) [%4]")
          .arg(name())
          .arg(val.toString())
          .arg(index)
          .arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepDateTimeProperty::setValue(QDateTime val)
{

  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
          tr("%1: QcepDateTimeProperty::setValue(QDateTime \"%2\")")
          .arg(name())
          .arg(val.toString()));
  }

  if (val != m_Value) {
    if (g_Application && debug()) {
      g_Application->printMessage(
            tr("%1: QcepDateTimeProperty::setValue(QDateTime \"%2\") [%3]")
            .arg(name())
            .arg(val.toString())
            .arg(index()));
    }

    QMutexLocker lock(&m_Mutex);

    m_Value = val;

    if (m_Saver) {
      m_Saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepDateTimeProperty::setDefaultValue(QDateTime val)
{
  QMutexLocker lock(&m_Mutex);

  if (val != m_Default) {
    m_Default = val;
  }
}

void QcepDateTimeProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepDateTimeProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

QcepDoubleListProperty::QcepDoubleListProperty(QxrdSettingsSaver *saver, QObject *parent, const char *name, QcepDoubleList value)
  : QcepProperty(saver, parent, name),
    m_Default(value),
    m_Value(value)
{
}

QcepDoubleList QcepDoubleListProperty::value() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Value;
}

QcepDoubleList QcepDoubleListProperty::defaultValue() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Default;
}

void QcepDoubleListProperty::setValue(QcepDoubleList val, int index)
{
  if (debug()) {
    g_Application->printMessage(
          tr("%1 QcepDoubleListProperty::setValue(QcepDoubleList %2, int %3) [%4]")
          .arg(name())
          .arg(toString(val))
          .arg(index)
          .arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepDoubleListProperty::clear()
{
  QMutexLocker lock(&m_Mutex);

  setValue(QcepDoubleList());
}

void QcepDoubleListProperty::appendValue(double val)
{
  QMutexLocker lock(&m_Mutex);

  QcepDoubleList list = value();
  list.append(val);

  setValue(list);
}

QString QcepDoubleListProperty::toString(const QcepDoubleList &val)
{
  QString res = "[";
  int ct = val.count();

  for (int i=0; i<ct; i++) {
    if (i<(ct-1)) {
      res += tr("%1, ").arg(val[i]);
    } else {
      res += tr("%1").arg(val[i]);
    }
  }

  res += "]";

  return res;
}

void QcepDoubleListProperty::setValue(QcepDoubleList val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
          tr("%1 QcepDoubleListProperty::setValue(QcepDoubleList %2)")
          .arg(name())
          .arg(toString(val)));
  }

  if (val != m_Value) {
    if (g_Application && debug()) {
      g_Application->printMessage(
            tr("%1: QcepDateTimeProperty::setValue(QcepDoubleList %2) [%3]")
            .arg(name())
            .arg(toString(val))
            .arg(index()));
    }

    QMutexLocker lock(&m_Mutex);

    m_Value = val;

    if (m_Saver) {
      m_Saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepDoubleListProperty::setDefaultValue(QcepDoubleList val)
{
  QMutexLocker lock(&m_Mutex);

  m_Default = val;
}

void QcepDoubleListProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepDoubleListProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

QcepStringListProperty::QcepStringListProperty(QxrdSettingsSaver *saver, QObject *parent, const char *name, QStringList value)
  : QcepProperty(saver, parent, name),
    m_Default(value),
    m_Value(value)
{
}

QStringList QcepStringListProperty::value() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Value;
}

QStringList QcepStringListProperty::defaultValue() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Default;
}

void QcepStringListProperty::setValue(QStringList val, int index)
{
  if (debug()) {
    g_Application->printMessage(
          tr("%1 QcepStringListProperty::setValue(QcepStringList %2, int %3) [%4]")
          .arg(name())
          .arg(toString(val))
          .arg(index)
          .arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepStringListProperty::clear()
{
  QMutexLocker lock(&m_Mutex);

  setValue(QcepStringList());
}

void QcepStringListProperty::appendValue(QString val)
{
  QMutexLocker lock(&m_Mutex);

  QStringList list = value();
  list.append(val);

  setValue(list);
}

QString QcepStringListProperty::toString(const QStringList &val)
{
  QString res = "[";
  int ct = val.count();

  for (int i=0; i<ct; i++) {
    if (i<(ct-1)) {
      res += tr("%1, ").arg(val[i]);
    } else {
      res += tr("%1").arg(val[i]);
    }
  }

  res += "]";

  return res;
}

void QcepStringListProperty::setValue(QStringList val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
          tr("%1 QcepStringListProperty::setValue(QcepStringList %2)")
          .arg(name())
          .arg(toString(val)));
  }

  if (val != m_Value) {
    if (g_Application && debug()) {
      g_Application->printMessage(
            tr("%1: QcepDateTimeProperty::setValue(QcepStringList %2) [%3]")
            .arg(name())
            .arg(toString(val))
            .arg(index()));
    }

    QMutexLocker lock(&m_Mutex);

    m_Value = val;

    if (m_Saver) {
      m_Saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepStringListProperty::setDefaultValue(QStringList val)
{
  QMutexLocker lock(&m_Mutex);

  m_Default = val;
}

void QcepStringListProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepStringListProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

QcepByteArrayProperty::QcepByteArrayProperty(QxrdSettingsSaver *saver, QObject *parent, const char *name, QByteArray value)
  : QcepProperty(saver, parent, name),
    m_Default(value),
    m_Value(value)
{
}

QByteArray QcepByteArrayProperty::value() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Value;
}

QByteArray QcepByteArrayProperty::defaultValue() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Default;
}

void QcepByteArrayProperty::setValue(QByteArray val, int index)
{
  if (debug()) {
    g_Application->printMessage(
          tr("%1 QcepByteArrayProperty::setValue(QByteArray %2, int %3) [%4]")
          .arg(name())
          .arg(toString(val))
          .arg(index)
          .arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepByteArrayProperty::clear()
{
  QMutexLocker lock(&m_Mutex);

  setValue(QByteArray());
}

QString QcepByteArrayProperty::toString(const QByteArray &val)
{
  QString res = "[";
  int ct = val.count();

  for (int i=0; i<ct; i++) {
    if (i<(ct-1)) {
      res += tr("%1, ").arg(val[i]);
    } else {
      res += tr("%1").arg(val[i]);
    }
  }

  res += "]";

  return res;
}

void QcepByteArrayProperty::setValue(QByteArray val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
          tr("%1 QcepByteArrayProperty::setValue(QByteArray %2)")
          .arg(name())
          .arg(toString(val)));
  }

  if (val != m_Value) {
    if (g_Application && debug()) {
      g_Application->printMessage(
            tr("%1: QcepByteArrayProperty::setValue(QByteArray %2) [%3]")
            .arg(name())
            .arg(toString(val))
            .arg(index()));
    }

    QMutexLocker lock(&m_Mutex);

    m_Value = val;

    if (m_Saver) {
      m_Saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepByteArrayProperty::setDefaultValue(QByteArray val)
{
  QMutexLocker lock(&m_Mutex);

  m_Default = val;
}

void QcepByteArrayProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepByteArrayProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}
