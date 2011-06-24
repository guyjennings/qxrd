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
    m_NQueuedUpdates(0),
    m_IsStored(false),
    m_WasLoaded(false),
    m_Parent(parent),
    m_Name(name),
    m_Variant(value)
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

QVariant QcepProperty::variant() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Variant;
}

QString QcepProperty::name() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Name;
}

void QcepProperty::changeVariant(QVariant val)
{
  QMutexLocker lock(&m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepProperty::changeVariant <- %2)").arg(name()).arg(val.toString()));
  }

  m_Variant = val;

  if (m_IsStored && m_WasLoaded) {
    if (qcepDebug(DEBUG_PREFS)) {
      g_Application->printMessage(
          tr("%1: QcepProperty::changeVariant is stored\n").arg(name()));
    }

    m_ChangeCount.fetchAndAddOrdered(1);
  }
}

template <typename T>
void QcepProperty::changeVariant(T val)
{
  QMutexLocker lock(&m_Mutex);

  m_Variant.setValue(val);

  if (m_IsStored && m_WasLoaded) {
    if (qcepDebug(DEBUG_PREFS)) {
      g_Application->printMessage(
          tr("%1: QcepProperty::changeVariant is stored\n").arg(name()));
    }

    m_ChangeCount.fetchAndAddOrdered(1);
  }
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

//int QcepProperty::debug() const
//{
//  QMutexLocker lock(&m_Mutex);
//
//  return m_Debug;
//}
//
//void QcepProperty::setDebug(int dbg)
//{
//  QMutexLocker lock(&m_Mutex);
//
//  m_Debug = dbg;
//}
//
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
//
//        property -> readSettings(settings, groupName);
//
        object -> setProperty(qPrintable(key), settings.value(key));
      } else {
        if (qcepDebug(DEBUG_PREFS | DEBUG_PROPERTIES)) {
          g_Application->printMessage(
              tr("property %1 of %2 not stored").arg(key)
                 .arg(meta -> className()));
        }
      }
    } else {
//      object -> setProperty(qPrintable(key), settings->value(key));
//      QMetaObject::invokeMethod(object, "setDynamicProperty", Qt::QueuedConnection, Q_ARG(QString, key), Q_ARG(QVariant, settings->value(key)));
//
//      printf("property %s of %s created dynamically\n",
//             qPrintable(key),
//             meta -> className());
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
    changeVariant(val);

    m_Value = val;

    m_NQueuedUpdates++;

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
    changeVariant(m_Value+step);

    m_Value += step;

    m_NQueuedUpdates++;

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
//  setDebug(true);
}

int QcepIntProperty::value() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Value;
}

int QcepIntProperty::defaultValue() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Default;
}

void QcepIntProperty::changeValue(int val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("QcepIntProperty::changeValue [%1] = %2, %3")
        .arg(name()).arg(val).arg(m_NQueuedUpdates));
  }

  if (m_NQueuedUpdates <= 1) {
    setValue(val);
  }

  m_NQueuedUpdates--;
}

void QcepIntProperty::setValue(int val)
{
  QMutexLocker lock(&m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepIntProperty::setValue <- %2, %3")
        .arg(name()).arg(val).arg(m_NQueuedUpdates));
  }

  if (val != m_Value) {
    changeVariant(val);

    m_Value = val;

    m_NQueuedUpdates++;

    emit changedValue(m_Value);
    emit changedValue(tr("%1").arg(m_Value));
  }
}

void QcepIntProperty::incValue(int step)
{
  QMutexLocker lock(&m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepIntProperty::incValue <- %2, %3")
        .arg(name()).arg(step).arg(m_NQueuedUpdates));
  }

  if (step) {
    changeVariant(m_Value + step);

    m_Value += step;

    m_NQueuedUpdates++;

    emit changedValue(m_Value);
    emit changedValue(tr("%1").arg(m_Value));
  }
}

void QcepIntProperty::setDefaultValue(int val)
{
  QMutexLocker lock(&m_Mutex);

  if (val != m_Default) {
    m_Default = val;

    emit changedDefault(val);
  }
}

void QcepIntProperty::resetValue()
{
  QMutexLocker lock(&m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    g_Application->printMessage(
        tr("%1: QcepIntProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

void QcepIntProperty::linkTo(QSpinBox *spinBox)
{
  {
    QMutexLocker lock(&m_Mutex);

    if (qcepDebug(DEBUG_PROPERTIES)) {
      g_Application->printMessage(
          tr("%1: QcepIntProperty::linkTo(QSpinBox *%2)")
          .arg(name()).HEXARG(spinBox));
    }

    spinBox -> setValue(value());

    connect(this, SIGNAL(changedValue(int)), spinBox, SLOT(setValue(int)));
    connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeValue(int)));
  }
}

void QcepIntProperty::linkTo(QComboBox *comboBox)
{
  {
    QMutexLocker lock(&m_Mutex);

    if (qcepDebug(DEBUG_PROPERTIES)) {
      g_Application->printMessage(
          tr("%1: QcepIntProperty::linkTo(QComboBox *%2)")
          .arg(name()).HEXARG(comboBox));
    }

    comboBox -> setCurrentIndex(value());

    connect(this, SIGNAL(changedValue(int)), comboBox, SLOT(setCurrentIndex(int)));
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeValue(int)));
  }
}

void QcepIntProperty::linkTo(QLabel *label)
{
  {
    QMutexLocker lock(&m_Mutex);

    label -> setText(tr("%1").arg(value()));

    connect(this, SIGNAL(changedValue(QString)), label, SLOT(setText(QString)));
  }
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
    changeVariant(val);

    m_Value = val;

    m_NQueuedUpdates++;

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
    changeVariant(val);

    m_Value = val;

    m_NQueuedUpdates++;

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
    changeVariant(val);

    m_Value = val;

    m_NQueuedUpdates++;

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
  changeVariant(value);
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

//void QcepDoubleListProperty::changeValue(QString val)
//{
//  if (qcepDebug(DEBUG_PROPERTIES)) {
//    printf("QcepDoubleListProperty::changeValue [%s] = %s, %d\n", qPrintable(name()), qPrintable(val), m_NQueuedUpdates);
//  }
//
//  if (m_NQueuedUpdates <= 1) {
//    setValue(val.toDouble());
//  }
//
//  m_NQueuedUpdates--;
//}

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
    changeVariant(val);

    m_Value = val;

    m_NQueuedUpdates++;

    emit changedValue(m_Value);
//    emit changedValue(tr("%1").arg(m_Value));
  }
}

//void QcepDoubleListProperty::incValue(QcepDoubleList step)
//{
//  QMutexLocker lock(&m_Mutex);
//
//  if (qcepDebug(DEBUG_PROPERTIES)) {
//    printf("%s: QcepDoubleListProperty::incValue <- %g, %d\n", qPrintable(name()), step, m_NQueuedUpdates);
//  }
//
//  if (step) {
//    changeVariant(m_Value+step);
//
//    m_Value += step;
//
//    m_NQueuedUpdates++;
//
//    emit changedValue(m_Value);
//    emit changedValue(tr("%1").arg(m_Value));
//  }
//}

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

//void QcepDoubleListProperty::linkTo(QComboBox *comboBox)
//{
//  {
//    QMutexLocker lock(&m_Mutex);
//
//    if (qcepDebug(DEBUG_PROPERTIES)) {
//      printf("%s: QcepDoubleListProperty::linkTo(QComboBox *%p)\n", qPrintable(name()), comboBox);
//    }
//
//    connect(this, SIGNAL(changedValue(QString)), comboBox, SLOT(setEditText(QString)));
//    connect(comboBox,SIGNAL(editTextChanged(QString)), this, SLOT(changeValue(QString)));
//  }
//}

//void QcepDoubleListProperty::linkTo(QDoubleSpinBox *spinBox)
//{
//  {
//    QMutexLocker lock(&m_Mutex);
//
//    if (qcepDebug(DEBUG_PROPERTIES)) {
//      printf("%s: QcepDoubleListProperty::linkTo(QDoubleSpinBox *%p)\n", qPrintable(name()), spinBox);
//    }
//
//    spinBox -> setValue(value());
//
//    connect(this, SIGNAL(changedValue(QcepDoubleList)), spinBox, SLOT(setValue(QcepDoubleList)));
//    connect(spinBox, SIGNAL(valueChanged(QcepDoubleList)), this, SLOT(changeValue(QcepDoubleList)));
//  }
//}

//void QcepDoubleListProperty::linkTo(QLabel *label)
//{
//  {
//    QMutexLocker lock(&m_Mutex);
//
//    label -> setText(tr("%1").arg(value()));
//
//    connect(this, SIGNAL(changedValue(QString)), label, SLOT(setText(QString)));
//  }
//}