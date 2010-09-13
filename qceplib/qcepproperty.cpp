/******************************************************************
*
*  $Id: qcepproperty.cpp,v 1.2 2010/09/13 20:00:25 jennings Exp $
*
*******************************************************************/

#include "qcepproperty.h"

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

QcepProperty::QcepProperty(QObject *parent, QString name, QVariant value)
  : QObject(),
    m_Mutex(QMutex::Recursive),
    m_NQueuedUpdates(0),
    m_Parent(parent),
    m_Name(name),
    m_Variant(value),
    SOURCE_IDENT("$Id: qcepproperty.cpp,v 1.2 2010/09/13 20:00:25 jennings Exp $")
{
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

  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepProperty::changeVariant <- %s\n", qPrintable(name()), qPrintable(val.toString()));
  );

  m_Variant = val;
}

template <typename T>
void QcepProperty::changeVariant(T val)
{
  QMutexLocker lock(&m_Mutex);

  m_Variant.setValue(val);
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

      QCEP_DEBUG(DEBUG_PREFS | DEBUG_PROPERTIES,
                 printf("Save %s/%s = %s [%s]\n", qPrintable(groupName), name, qPrintable(value.toString()), value.typeName())
      );

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
    QCEP_DEBUG(DEBUG_PREFS | DEBUG_PROPERTIES,
               printf("Load %s/%s = %s [%s]\n", qPrintable(groupName), qPrintable(key),
                      qPrintable(settings.value(key).toString()), settings.value(key).typeName());
    );

    int metaindex = meta->indexOfProperty(qPrintable(key));

    if (metaindex >= 0) {
      QMetaProperty metaproperty = meta->property(metaindex);

      if (metaproperty.isStored()) {
//        QcepProperty *property = qobject_cast<QcepProperty*> (object -> property(key));
//
//        property -> readSettings(settings, groupName);
//
        object -> setProperty(qPrintable(key), settings.value(key));
      } else {
        QCEP_DEBUG(DEBUG_PREFS | DEBUG_PROPERTIES,
                   printf("property %s of %s not stored\n",
                          qPrintable(key),
                          meta -> className());
        );
      }
    } else {
//      object -> setProperty(qPrintable(key), settings->value(key));
//      QMetaObject::invokeMethod(object, "setDynamicProperty", Qt::QueuedConnection, Q_ARG(QString, key), Q_ARG(QVariant, settings->value(key)));
//
//      printf("property %s of %s created dynamically\n",
//             qPrintable(key),
//             meta -> className());
      printf("property %s of %s does not exist\n",
             qPrintable(key),
             meta -> className());
    }
  }

  settings.endGroup();
}

void QcepProperty::dumpMetaData(const QMetaObject *meta)
{
  while (meta) {
    printf("MetaData for class %s\n", meta -> className());
    printf(" superClass = %p\n", meta -> superClass());
    printf(" methodCount = %d, methodOffset = %d\n", meta->methodCount(), meta->methodOffset());
    printf(" propertyCount = %d, propertyOffset = %d\n", meta->propertyCount(), meta->propertyOffset());

    meta = meta->superClass();
  }
}

QcepDoubleProperty::QcepDoubleProperty(QObject *parent, QString name, double value)
  : QcepProperty(parent, name, value),
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

void QcepDoubleProperty::changeValue(double val)
{
  QCEP_DEBUG(DEBUG_PROPERTIES,
             printf("QcepDoubleProperty::changeValue [%s] = %g, %d\n", qPrintable(name()), val, m_NQueuedUpdates);
  );

  if (m_NQueuedUpdates <= 1) {
    setValue(val);
  }

  m_NQueuedUpdates--;
}

void QcepDoubleProperty::changeValue(QString val)
{
  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("QcepDoubleProperty::changeValue [%s] = %s, %d\n", qPrintable(name()), qPrintable(val), m_NQueuedUpdates);
  );

  if (m_NQueuedUpdates <= 1) {
    setValue(val.toDouble());
  }

  m_NQueuedUpdates--;
}

void QcepDoubleProperty::setValue(double val)
{
  QMutexLocker lock(&m_Mutex);

  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepDoubleProperty::setValue <- %g, %d\n", qPrintable(name()), val, m_NQueuedUpdates);
  );

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

  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepDoubleProperty::incValue <- %g, %d\n", qPrintable(name()), step, m_NQueuedUpdates);
  );

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

  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepDoubleProperty::resetValue\n", qPrintable(name()));
  );

  setValue(m_Default);
}

void QcepDoubleProperty::linkTo(QComboBox *comboBox)
{
  {
    QMutexLocker lock(&m_Mutex);

    QCEP_DEBUG(DEBUG_PROPERTIES,
      printf("%s: QcepDoubleProperty::linkTo(QComboBox *%p)\n", qPrintable(name()), comboBox);
    );

    connect(this, SIGNAL(changedValue(QString)), comboBox, SLOT(setEditText(QString)));
    connect(comboBox,SIGNAL(editTextChanged(QString)), this, SLOT(changeValue(QString)));
  }
}

void QcepDoubleProperty::linkTo(QDoubleSpinBox *spinBox)
{
  {
    QMutexLocker lock(&m_Mutex);

    QCEP_DEBUG(DEBUG_PROPERTIES,
      printf("%s: QcepDoubleProperty::linkTo(QDoubleSpinBox *%p)\n", qPrintable(name()), spinBox);
    );

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

QcepIntProperty::QcepIntProperty(QObject *parent, QString name, int value)
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
  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("QcepIntProperty::changeValue [%s] = %d, %d\n", qPrintable(name()), val, m_NQueuedUpdates);
  );

  if (m_NQueuedUpdates <= 1) {
    setValue(val);
  }

  m_NQueuedUpdates--;
}

void QcepIntProperty::setValue(int val)
{
  QMutexLocker lock(&m_Mutex);

  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepIntProperty::setValue <- %d, %d\n", qPrintable(name()), val, m_NQueuedUpdates);
  );

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

  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepIntProperty::incValue <- %d, %d\n", qPrintable(name()), step, m_NQueuedUpdates);
  );

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

  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepIntProperty::resetValue\n", qPrintable(name()));
  );

  setValue(m_Default);
}

void QcepIntProperty::linkTo(QSpinBox *spinBox)
{
  {
    QMutexLocker lock(&m_Mutex);

    QCEP_DEBUG(DEBUG_PROPERTIES,
      printf("%s: QcepIntProperty::linkTo(QSpinBox *%p)\n", qPrintable(name()), spinBox);
    );

    spinBox -> setValue(value());

    connect(this, SIGNAL(changedValue(int)), spinBox, SLOT(setValue(int)));
    connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeValue(int)));
  }
}

void QcepIntProperty::linkTo(QComboBox *comboBox)
{
  {
    QMutexLocker lock(&m_Mutex);

    QCEP_DEBUG(DEBUG_PROPERTIES,
      printf("%s: QcepIntProperty::linkTo(QComboBox *%p)\n", qPrintable(name()), comboBox);
    );

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

QcepBoolProperty::QcepBoolProperty(QObject *parent, QString name, bool value)
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
  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("QcepBoolProperty::changeValue [%s] = %d, %d\n", qPrintable(name()), val, m_NQueuedUpdates);
  );

  if (m_NQueuedUpdates <= 1) {
    setValue(val);
  }

  m_NQueuedUpdates--;
}

void QcepBoolProperty::setValue(bool val)
{
  QMutexLocker lock(&m_Mutex);

  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepBoolProperty::setValue <- %d, %d\n", qPrintable(name()), val, m_NQueuedUpdates);
  );

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

  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepBoolProperty::resetValue\n", qPrintable(name()));
  );

  setValue(m_Default);
}

void QcepBoolProperty::linkTo(QAbstractButton *button)
{
  {
    QMutexLocker lock(&m_Mutex);

    QCEP_DEBUG(DEBUG_PROPERTIES,
      printf("%s: QcepBoolProperty::linkTo(QAbstractButton *%p)\n", qPrintable(name()), button);
    );

    button -> setChecked(value());

    connect(this, SIGNAL(changedValue(bool)), button, SLOT(setChecked(bool)));
    connect(button, SIGNAL(toggled(bool)), this, SLOT(changeValue(bool)));
  }
}

QcepStringProperty::QcepStringProperty(QObject *parent, QString name, QString value)
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
  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("QcepStringProperty::changeValue [%s] = %s, %d\n", qPrintable(name()), qPrintable(val), m_NQueuedUpdates);
  );

  if (m_NQueuedUpdates <= 1) {
    setValue(val);
  }

  m_NQueuedUpdates--;
}

void QcepStringProperty::setValue(QString val)
{
  QMutexLocker lock(&m_Mutex);

  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepStringProperty::setValue <- %s, %d\n", qPrintable(name()), qPrintable(val), m_NQueuedUpdates);
  );

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
  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepStringProperty::resetValue\n", qPrintable(name()));
  );

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

    QCEP_DEBUG(DEBUG_PROPERTIES,
      printf("%s: QcepStringProperty::linkTo(QLineEdit *%p)\n", qPrintable(name()), lineEdit);
    );

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

QcepDateTimeProperty::QcepDateTimeProperty(QObject *parent, QString name, QDateTime value)
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
  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("QcepDateTimeProperty::changeValue [%s] = %s, %d\n", qPrintable(name()), qPrintable(val.toString()), m_NQueuedUpdates);
  );

  if (m_NQueuedUpdates <= 1) {
    setValue(val);
  }

  m_NQueuedUpdates--;
}

void QcepDateTimeProperty::setValue(QDateTime val)
{
  QMutexLocker lock(&m_Mutex);

  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepDateTimeProperty::setValue <- %s, %d\n", qPrintable(name()), qPrintable(val.toString()), m_NQueuedUpdates);
  );

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
  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepDateTimeProperty::resetValue\n", qPrintable(name()));
  );

  setValue(m_Default);
}

QcepDoubleListProperty::QcepDoubleListProperty(QObject *parent, QString name, QcepDoubleList value)
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
  QCEP_DEBUG(DEBUG_PROPERTIES,
             printf("QcepDoubleListProperty::changeValue [%s] = %g, %d\n", qPrintable(name()), val[0], m_NQueuedUpdates);
  );

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
//  QCEP_DEBUG(DEBUG_PROPERTIES,
//    printf("QcepDoubleListProperty::changeValue [%s] = %s, %d\n", qPrintable(name()), qPrintable(val), m_NQueuedUpdates);
//  );
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

  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepDoubleListProperty::setValue <- %g, %d\n", qPrintable(name()), val[0], m_NQueuedUpdates);
  );

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
//  QCEP_DEBUG(DEBUG_PROPERTIES,
//    printf("%s: QcepDoubleListProperty::incValue <- %g, %d\n", qPrintable(name()), step, m_NQueuedUpdates);
//  );
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

  QCEP_DEBUG(DEBUG_PROPERTIES,
    printf("%s: QcepDoubleListProperty::resetValue\n", qPrintable(name()));
  );

  setValue(m_Default);
}

//void QcepDoubleListProperty::linkTo(QComboBox *comboBox)
//{
//  {
//    QMutexLocker lock(&m_Mutex);
//
//    QCEP_DEBUG(DEBUG_PROPERTIES,
//      printf("%s: QcepDoubleListProperty::linkTo(QComboBox *%p)\n", qPrintable(name()), comboBox);
//    );
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
//    QCEP_DEBUG(DEBUG_PROPERTIES,
//      printf("%s: QcepDoubleListProperty::linkTo(QDoubleSpinBox *%p)\n", qPrintable(name()), spinBox);
//    );
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

/******************************************************************
*
*  $Log: qcepproperty.cpp,v $
*  Revision 1.2  2010/09/13 20:00:25  jennings
*  Merged
*
*  Revision 1.1.2.11  2010/08/16 21:43:58  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.10  2010/07/27 21:53:03  jennings
*  Added double list property type
*  Added 'testImage' script object, interface to QxrdGenerateTestImage
*  Added QxrdDetectorGeometry class
*
*  Revision 1.1.2.9  2010/06/17 18:36:32  jennings
*  Added incValue method to QxrdIntProperty and QxrdDoubleProperty
*  Added QxrdIntProperty -> linkTo(QLabel)
*
*  Revision 1.1.2.8  2010/05/19 20:26:12  jennings
*  Added DEBUG_PREFS debugging options
*
*  Revision 1.1.2.7  2010/05/17 15:59:53  jennings
*  Changed debugging output to use QCEP_DEBUG macro
*
*  Revision 1.1.2.6  2010/05/09 13:23:05  jennings
*  Fixed image display bug caused by incomplete implementation of QxrdRasterData::copy
*
*  Revision 1.1.2.5  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.1.2.4  2010/04/19 23:45:40  jennings
*  Corrected error in printf format
*
*  Revision 1.1.2.3  2010/04/19 19:20:50  jennings
*  Added link between QcepDoubleProperty and a QComboBox
*
*  Revision 1.1.2.2  2010/04/17 05:14:06  jennings
*  Partially working
*
*  Revision 1.1.2.1  2010/04/13 19:29:13  jennings
*  Added qceplib to cvs
*
*  Revision 1.10  2009/11/09 18:00:35  jennings
*  *** empty log message ***
*
*  Revision 1.9  2009/10/02 18:51:14  jennings
*  Added option to link double property to a label - needs more work, though
*
*  Revision 1.8  2009/09/23 19:00:36  jennings
*  Removed dynamic property support - it's not thread-safe and doesn't work
*  under windows
*
*  Revision 1.7  2009/09/22 20:37:23  jennings
*  Dynamic properties attached to the acquisition object are propagated through to saved images
*
*  Revision 1.6  2009/09/09 22:32:12  jennings
*  Started to add TIFF metadata support
*
*  Revision 1.5  2009/07/03 17:25:52  jennings
*  Found a workable solution for the property widget update loops
*  by keeping track of the number of queued updates to the property
*  and not passing on the update events when the number of queued updates
*  is greater than one.
*
*  Revision 1.4  2009/07/01 20:15:53  jennings
*  *** empty log message ***
*
*  Revision 1.3  2009/07/01 03:24:33  jennings
*  Another attempt at making linked properties and widgets in different threads stable,
*  Made sure to have plenty of mutexes, making the property thread-safe
*  Then link widget signals to property slots with direct connection
*  and property signals to widget slots with queued connections
*
*  Revision 1.2  2009/06/30 21:35:31  jennings
*  Debugging meta data static init problems
*
*  Revision 1.1  2009/06/27 19:21:08  jennings
*  qavrgproperty renamed to qcepproperty
*
*  Revision 1.16  2009/06/27 05:07:25  jennings
*  Files moved back into qceplib
*
*  Revision 1.15  2009/06/27 01:19:36  jennings
*  Regsitered remaining container property types
*
*  Revision 1.14  2009/06/26 03:59:15  jennings
*  Added code for different fill pattern handling modes
*
*  Revision 1.13  2009/06/25 12:42:03  jennings
*  Implemented default values for properties - accessed by reset_xxx
*
*  Revision 1.12  2009/06/25 12:01:44  jennings
*  Implemented default values for properties - accessed by reset_xxx
*
*  Revision 1.11  2009/06/24 10:56:48  jennings
*  Modified to compile under fc11, added <stdio.h> includes
*
*  Revision 1.10  2009/06/22 00:03:34  jennings
*  Implemented saving display settings
*
*  Revision 1.9  2009/06/21 19:02:03  jennings
*  Consolidated property saving into one place
*  Implemented load/save settings routines
*
*  Revision 1.8  2009/06/21 00:30:52  jennings
*  Made graph update properly during acquisition
*  Made dark and reference data acquire directly into correct destination
*  Only perform fitting for AcquireData and AcquireOnce
*
*  Revision 1.7  2009/06/20 05:41:20  jennings
*  Added IDENT macros, moved log comments to end of files
*
*  Revision 1.6  2009/06/19 03:58:57  jennings
*  Changed QReadWriteLock to QMutex
*
*  Revision 1.5  2009/06/17 14:21:31  jennings
*  Fixed compiler warnings
*
*  Revision 1.4  2009/06/16 15:37:44  jennings
*  Added CVS log and ID entries
*
*
*******************************************************************/
