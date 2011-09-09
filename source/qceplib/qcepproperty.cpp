#include "qcepproperty.h"
#include "qxrdapplication.h"

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

QcepProperty::QcepProperty(QObject *parent, const char *name, QVariant value)
  : QObject(),
    m_Mutex(QMutex::Recursive),
    m_IsStored(false),
    m_WasLoaded(false),
    m_Name(name)/*,
    m_Variant(value)*/
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
}

QcepProperty::~QcepProperty()
{
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

QAtomicInt QcepProperty::m_ChangeCount = 0;

int QcepProperty::getChangeCount()
{
  return m_ChangeCount.fetchAndStoreOrdered(0);
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

void QcepProperty::writeSettings(QObject *object, const QMetaObject *meta, QString groupName, QSettings &settings)
{
  int count = meta->propertyCount();
  int offset = meta->propertyOffset();

  settings.beginGroup(groupName);

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

      settings.setValue(name, value);
    }
  }

  QByteArray name;

  foreach (name, object->dynamicPropertyNames()) {
    settings.setValue(name.data(), object->property(name.data()));
  }

  settings.endGroup();
}

void QcepProperty::readSettings(QObject *object, const QMetaObject *meta, QString groupName, QSettings &settings)
{
  settings.beginGroup(groupName);

  QStringList keys = settings.childKeys();

  foreach (QString key, keys) {
    if (qcepDebug(DEBUG_PREFS | DEBUG_PROPERTIES)) {
      g_Application->printMessage(
          tr("Load %1/%2 = %3 [%4]")
             .arg(groupName).arg(key)
             .arg(settings.value(key).toString())
             .arg(settings.value(key).typeName()));
    }

    int metaindex = meta->indexOfProperty(qPrintable(key));

    if (metaindex >= 0) {
      QMetaProperty metaproperty = meta->property(metaindex);

      if (metaproperty.isStored()) {
        QcepProperty *property = object->findChild<QcepProperty *>(key);

        if (property) {
          property->setWasLoaded(true);
        }

        object -> setProperty(qPrintable(key), settings.value(key));
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

  settings.endGroup();
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

QcepDoubleProperty::QcepDoubleProperty(QObject *parent, const char *name, double value)
  : QcepProperty(parent, name, value),
    m_Default(value),
    m_Value(value)
{
}

QcepDoubleProperty::~QcepDoubleProperty()
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

void QcepDoubleProperty::changeValue(double val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("QcepDoubleProperty::changeValue [%1] = %2, %3")
        .arg(name()).arg(val).arg(m_NQueuedUpdates));
  }

  if (m_NQueuedUpdates <= 1) {
    setValue(val);
  }

  m_NQueuedUpdates--;
}

void QcepDoubleProperty::changeValue(QString val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("QcepDoubleProperty::changeValue [%1] = %2, %3")
        .arg(name()).arg(val).arg(m_NQueuedUpdates));
  }

  if (m_NQueuedUpdates <= 1) {
    setValue(val.toDouble());
  }

  m_NQueuedUpdates--;
}

void QcepDoubleProperty::setValue(double val)
{
  QMutexLocker lock(&m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepDoubleProperty::setValue <- %2, %3")
           .arg(name()).arg(val).arg(m_NQueuedUpdates));
  }

  if (val != m_Value) {
    m_Value = val;

    m_NQueuedUpdates++;
    m_ChangeCount.fetchAndAddOrdered(1);

    emit changedValue(m_Value);
    emit changedValue(tr("%1").arg(m_Value));
  }
}

void QcepDoubleProperty::incValue(double step)
{
  QMutexLocker lock(&m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepDoubleProperty::incValue <- %2, %3")
        .arg(name()).arg(step).arg(m_NQueuedUpdates));
  }

  if (step) {
    m_Value += step;

    m_NQueuedUpdates++;
    m_ChangeCount.fetchAndAddOrdered(1);

    emit changedValue(m_Value);
    emit changedValue(tr("%1").arg(m_Value));
  }
}

void QcepDoubleProperty::setDefaultValue(double val)
{
  QMutexLocker lock(&m_Mutex);

  if (val != m_Default) {
    m_Default = val;

    emit changedDefault(val);
  }
}

void QcepDoubleProperty::resetValue()
{
  QMutexLocker lock(&m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepDoubleProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

void QcepDoubleProperty::linkTo(QComboBox *comboBox)
{
  {
    QMutexLocker lock(&m_Mutex);

    if (qcepDebug(DEBUG_PROPERTIES)) {
      g_Application->printMessage(
          tr("%1: QcepDoubleProperty::linkTo(QComboBox *%2)")
          .arg(name()).HEXARG(comboBox));
    }

    connect(this, SIGNAL(changedValue(QString)), comboBox, SLOT(setEditText(QString)));
    connect(comboBox,SIGNAL(editTextChanged(QString)), this, SLOT(changeValue(QString)));
  }
}

void QcepDoubleProperty::linkTo(QDoubleSpinBox *spinBox)
{
  {
    QMutexLocker lock(&m_Mutex);

    if (qcepDebug(DEBUG_PROPERTIES)) {
      g_Application->printMessage(
          tr("%1: QcepDoubleProperty::linkTo(QDoubleSpinBox *%2)")
             .arg(name()).HEXARG(spinBox));
    }

    spinBox -> setValue(value());

    connect(this, SIGNAL(changedValue(double)), spinBox, SLOT(setValue(double)));
    connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(changeValue(double)));
  }
}

void QcepDoubleProperty::linkTo(QLabel *label)
{
  {
    QMutexLocker lock(&m_Mutex);

    label -> setText(tr("%1").arg(value()));

    connect(this, SIGNAL(changedValue(QString)), label, SLOT(setText(QString)));
  }
}

QcepIntProperty::QcepIntProperty(QObject *parent, const char *name, int value)
  : QcepProperty(parent, name, value),
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
            tr("%1: QcepIntProperty::setValue <- %2, %3")
            .arg(name()).arg(val).arg(m_NQueuedUpdates));
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
    m_ChangeCount.fetchAndAddOrdered(1);

    emit changedValue(m_Value, incIndex(1));
    emit changedValue(tr("%1").arg(m_Value));
  }
}

void QcepIntProperty::incValue(int step)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    g_Application->printMessage(
        tr("%1: QcepIntProperty::incValue <- %2, %3")
        .arg(name()).arg(step).arg(m_NQueuedUpdates));
  }

  if (step) {
    m_Value.fetchAndAddOrdered(step);
    m_ChangeCount.fetchAndAddOrdered(1);

    emit changedValue(m_Value, incIndex(1));
    emit changedValue(tr("%1").arg(m_Value));
  }
}

void QcepIntProperty::setDefaultValue(int val)
{
  if (val != m_Default) {
    m_Default.fetchAndStoreOrdered(val);

    emit changedDefault(val);
  }
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

    connect(this, SIGNAL(changedValue(int, int)), helper, SLOT(setValue(int, int)));
    connect(helper, SIGNAL(valueChanged(int, int)), this, SLOT(setValue(int, int)));
  }
}

void QcepIntProperty::linkTo(QComboBox *comboBox)
{
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

    connect(this, SIGNAL(changedValue(int, int)), helper, SLOT(setCurrentIndex(int, int)));
    connect(helper, SIGNAL(currentIndexChanged(int, int)), this, SLOT(setValue(int, int)));
  }
}

void QcepIntProperty::linkTo(QLabel *label)
{
  {
    label -> setText(tr("%1").arg(value()));

    connect(this, SIGNAL(changedValue(QString)), label, SLOT(setText(QString)));
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

      m_SpinBox->setValue(value);
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

  emit valueChanged(value, m_Property->incIndex());
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
          .arg(m_Property->name()).arg(value).arg(index).arg(m_Property->index()).arg(m_ComboBox->currentIndex()));
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

      m_ComboBox->setCurrentIndex(value);
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

  emit currentIndexChanged(value, m_Property->incIndex());
}

QcepBoolProperty::QcepBoolProperty(QObject *parent, const char *name, bool value)
  : QcepProperty(parent, name, value),
    m_Default(value),
    m_Value(value)
{
}

bool QcepBoolProperty::value() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Value;
}

bool QcepBoolProperty::defaultValue() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Default;
}

void QcepBoolProperty::changeValue(bool val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("QcepBoolProperty::changeValue [%1] = %2, %3")
        .arg(name()).arg(val).arg(m_NQueuedUpdates));
  }

  if (m_NQueuedUpdates <= 1) {
    setValue(val);
  }

  m_NQueuedUpdates--;
}

void QcepBoolProperty::setValue(bool val)
{
  QMutexLocker lock(&m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepBoolProperty::setValue <- %2, %3")
        .arg(name()).arg(val).arg(m_NQueuedUpdates));
  }

  if (val != m_Value) {
    m_Value = val;

    m_NQueuedUpdates++;
    m_ChangeCount.fetchAndAddOrdered(1);

    emit changedValue(m_Value);
  }
}

void QcepBoolProperty::setDefaultValue(bool val)
{
  QMutexLocker lock(&m_Mutex);

  if (val != m_Default) {
    m_Default = val;

    emit changedDefault(val);
  }
}

void QcepBoolProperty::resetValue()
{
  QMutexLocker lock(&m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepBoolProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

void QcepBoolProperty::linkTo(QAbstractButton *button)
{
  {
    QMutexLocker lock(&m_Mutex);

    if (qcepDebug(DEBUG_PROPERTIES)) {
      g_Application->printMessage(
          tr("%1: QcepBoolProperty::linkTo(QAbstractButton *%2)")
          .arg(name()).HEXARG(button));
    }

    button -> setChecked(value());

    connect(this, SIGNAL(changedValue(bool)), button, SLOT(setChecked(bool)));
    connect(button, SIGNAL(toggled(bool)), this, SLOT(changeValue(bool)));
  }
}

QcepStringProperty::QcepStringProperty(QObject *parent, const char *name, QString value)
  : QcepProperty(parent, name, value),
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

void QcepStringProperty::changeValue(QString val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("QcepStringProperty::changeValue [%1] = %2, %3")
        .arg(name()).arg(val).arg(m_NQueuedUpdates));
  }

  if (m_NQueuedUpdates <= 1) {
    setValue(val);
  }

  m_NQueuedUpdates--;
}

void QcepStringProperty::setValue(QString val)
{
  QMutexLocker lock(&m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepStringProperty::setValue <- %2, %3")
        .arg(name()).arg(val).arg(m_NQueuedUpdates));
  }

  if (val != m_Value) {
    m_Value = val;

    m_NQueuedUpdates++;
    m_ChangeCount.fetchAndAddOrdered(1);

    emit changedValue(m_Value);
  }
}

void QcepStringProperty::setDefaultValue(QString val)
{
  QMutexLocker lock(&m_Mutex);

  if (val != m_Default) {
    m_Default = val;

    emit changedDefault(val);
  }
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
  {
    QMutexLocker lock(&m_Mutex);

    if (qcepDebug(DEBUG_PROPERTIES)) {
      g_Application->printMessage(
          tr("%1: QcepStringProperty::linkTo(QLineEdit *%2)")
          .arg(name()).HEXARG(lineEdit));
    }

    lineEdit -> setText(value());

    connect(this, SIGNAL(changedValue(QString)), lineEdit, SLOT(setText(QString)));
    connect(lineEdit, SIGNAL(textEdited(QString)), this, SLOT(changeValue(QString)));
  }
}

void QcepStringProperty::linkTo(QLabel *label)
{
  {
    QMutexLocker lock(&m_Mutex);

    label -> setText(value());

    connect(this, SIGNAL(changedValue(QString)), label, SLOT(setText(QString)));
  }
}

QcepDateTimeProperty::QcepDateTimeProperty(QObject *parent, const char *name, QDateTime value)
  : QcepProperty(parent, name, value),
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

void QcepDateTimeProperty::changeValue(QDateTime val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("QcepDateTimeProperty::changeValue [%1] = %2, %3")
        .arg(name()).arg(val.toString()).arg(m_NQueuedUpdates));
  }

  if (m_NQueuedUpdates <= 1) {
    setValue(val);
  }

  m_NQueuedUpdates--;
}

void QcepDateTimeProperty::setValue(QDateTime val)
{
  QMutexLocker lock(&m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepDateTimeProperty::setValue <- %2, %3")
        .arg(name()).arg(val.toString()).arg(m_NQueuedUpdates));
  }

  if (val != m_Value) {
    m_Value = val;

    m_NQueuedUpdates++;
    m_ChangeCount.fetchAndAddOrdered(1);

    emit changedValue(m_Value);
  }
}

void QcepDateTimeProperty::setDefaultValue(QDateTime val)
{
  QMutexLocker lock(&m_Mutex);

  if (val != m_Default) {
    m_Default = val;

    emit changedDefault(val);
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

QcepDoubleListProperty::QcepDoubleListProperty(QObject *parent, const char *name, QcepDoubleList value)
  : QcepProperty(parent, name, 0),
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

void QcepDoubleListProperty::changeValue(QcepDoubleList val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("QcepDoubleListProperty::changeValue [%1] = %2, %3")
        .arg(name()).arg(val[0]).arg(m_NQueuedUpdates));
  }

  if (m_NQueuedUpdates <= 1) {
    setValue(val);
  }

  m_NQueuedUpdates--;
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

void QcepDoubleListProperty::setValue(QcepDoubleList val)
{
  QMutexLocker lock(&m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    int ct = val.count();
    QString msg;

    msg = tr("%1: QcepDoubleListProperty::setValue <- [").arg(name());

    for (int i=0; i<ct; i++) {
      if (i<(ct-1)) {
        msg += tr("%1, ").arg(val[i]);
      } else {
        msg += tr("%1").arg(val[i]);
      }
    }

    msg += tr("], %1").arg(m_NQueuedUpdates);

    g_Application->printMessage(msg);
  }

  if (val != m_Value) {
    m_Value = val;

    m_NQueuedUpdates++;
    m_ChangeCount.fetchAndAddOrdered(1);

    emit changedValue(m_Value);
//    emit changedValue(tr("%1").arg(m_Value));
  }
}

void QcepDoubleListProperty::setDefaultValue(QcepDoubleList val)
{
  QMutexLocker lock(&m_Mutex);

  if (val != m_Default) {
    m_Default = val;

    emit changedDefault(val);
  }
}

void QcepDoubleListProperty::resetValue()
{
  QMutexLocker lock(&m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepDoubleListProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}
