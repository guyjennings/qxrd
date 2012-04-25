#include "qcepdebug.h"
#include "qcepproperty.h"
#include "qxrdapplication.h"
#include "qxrdsettingssaver.h"
#include "qxrddocumentationdictionary.h"
#include "qxrdmutexlocker.h"

#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QProgressBar>
#include <QAbstractButton>
#include <QMetaProperty>
#include <QSettings>
#include <stdio.h>

QcepProperty::QcepProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QString toolTip)
  : QObject(),
    m_Mutex(QMutex::Recursive),
    m_Saver(saver),
    m_Debug(false),
    m_IsStored(false),
    m_Name(name),
    m_Parent(parent),
    m_ToolTip(toolTip)
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

    QXRD_DOC_OBJECT(tr("%1.%2").arg(parent->objectName()).arg(name), toolTip)
  }

  if (m_Saver && !m_IsStored) {
    printMessage(tr("Warning: property %1 of parent %2 has saver but is not stored")
                 .arg(name).arg(parent?parent->objectName():"NULL"));
//    printf("Warning: property %s has saver but is not stored\n", qPrintable(name));
  }

//  if (m_IsStored && !m_Saver) {
//    printMessage(tr("Warning: property %1 of parent %2 is stored but has no saver")
//                 .arg(name).arg(parent?parent->objectName():"NULL"));
////    printf("Warning: property %s is stored but has no saver\n", qPrintable(name));
//  }
}

QString QcepProperty::name() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Name;
}

QString QcepProperty::parentName() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_Parent) {
    return m_Parent->objectName();
  } else {
    return QString();
  }
}

QString QcepProperty::toolTip() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_ToolTip;
}

void QcepProperty::setToolTip(QString tip)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_ToolTip = tip;
}

QString QcepProperty::expandedToolTip() const
{
  QString pn = parentName();
  QString nm = name();
  QString tt = toolTip();

  if (pn.count() && nm.count() && tt.count()) {
    return pn+"."+nm+"\n"+tt;
  } else if (pn.count() || nm.count()) {
    return pn+"."+nm+"\n"+tt;
  } else {
    return tt;
  }
}

void QcepProperty::setWidgetToolTip(QWidget *widget)
{
  if (widget) {
    QString tt = widget->toolTip();

    if (tt == "") {
      widget->setToolTip(expandedToolTip());
    } else {
      widget->setToolTip(tt + "\n=============\n" + expandedToolTip());
    }
  }
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
  qRegisterMetaType< QwtDoublePoint >("QwtDoublePoint");
  qRegisterMetaType< QwtDoubleRect >("QwtDoubleRect");
  qRegisterMetaType< QcepPolygon >("QcepPolygon");
  qRegisterMetaType< QVariant >("QVariant");

  qRegisterMetaTypeStreamOperators< QcepDoubleVector >("QcepDoubleVector");
  qRegisterMetaTypeStreamOperators< QcepBoolVector >("QcepBoolVector");
  qRegisterMetaTypeStreamOperators< QcepIntVector >("QcepIntVector");
  qRegisterMetaTypeStreamOperators< QcepStringVector >("QcepStringVector");
  qRegisterMetaTypeStreamOperators< QcepDoubleList >("QcepDoubleList");
  qRegisterMetaTypeStreamOperators< QcepBoolList >("QcepBoolList");
  qRegisterMetaTypeStreamOperators< QcepIntList >("QcepIntList");
  qRegisterMetaTypeStreamOperators< QcepStringList >("QcepStringList");
  qRegisterMetaTypeStreamOperators< QwtDoublePoint >("QwtDoublePoint");
  qRegisterMetaTypeStreamOperators< QwtDoubleRect >("QwtDoubleRect");
  qRegisterMetaTypeStreamOperators< QcepPolygon >("QcepPolygon");
}

void QcepProperty::setSaver(QcepSettingsSaverWPtr saver)
{
  m_Saver = saver;
}

void QcepProperty::setSettingsValue(QSettings *settings, QString name, QVariant v)
{
  settings->setValue(name, v);

  if (v.canConvert<QcepDoubleVector>()){
    QcepDoubleVector dv = v.value<QcepDoubleVector>();

    settings->beginWriteArray(name, dv.count());

    for (int i=0; i<dv.count(); i++) {
      settings->setArrayIndex(i);
      settings->setValue(name, dv.value(i));
    }

    settings->endArray();
  } else if (v.canConvert<QcepBoolVector>()) {
    QcepBoolVector dv = v.value<QcepBoolVector>();

    settings->beginWriteArray(name, dv.count());

    for (int i=0; i<dv.count(); i++) {
      settings->setArrayIndex(i);
      settings->setValue(name, dv.value(i));
    }

    settings->endArray();
  } else if (v.canConvert<QcepIntVector>()) {
    QcepIntVector dv = v.value<QcepIntVector>();

    settings->beginWriteArray(name, dv.count());

    for (int i=0; i<dv.count(); i++) {
      settings->setArrayIndex(i);
      settings->setValue(name, dv.value(i));
    }

    settings->endArray();
  } else if (v.canConvert<QcepStringVector>()) {
    QcepStringVector dv = v.value<QcepStringVector>();

    settings->beginWriteArray(name, dv.count());

    for (int i=0; i<dv.count(); i++) {
      settings->setArrayIndex(i);
      settings->setValue(name, dv.value(i));
    }

    settings->endArray();
  } else if (v.canConvert<QcepDoubleList>()){
    QcepDoubleList dv = v.value<QcepDoubleList>();

    settings->beginWriteArray(name, dv.count());

    for (int i=0; i<dv.count(); i++) {
      settings->setArrayIndex(i);
      settings->setValue(name, dv.value(i));
    }

    settings->endArray();
  } else if (v.canConvert<QcepBoolList>()) {
    QcepBoolList dv = v.value<QcepBoolList>();

    settings->beginWriteArray(name, dv.count());

    for (int i=0; i<dv.count(); i++) {
      settings->setArrayIndex(i);
      settings->setValue(name, dv.value(i));
    }

    settings->endArray();
  } else if (v.canConvert<QcepIntList>()) {
    QcepIntList dv = v.value<QcepIntList>();

    settings->beginWriteArray(name, dv.count());

    for (int i=0; i<dv.count(); i++) {
      settings->setArrayIndex(i);
      settings->setValue(name, dv.value(i));
    }

    settings->endArray();
  } else if (v.canConvert<QcepStringList>()) {
    QcepStringList dv = v.value<QcepStringList>();

    settings->beginWriteArray(name, dv.count());

    for (int i=0; i<dv.count(); i++) {
      settings->setArrayIndex(i);
      settings->setValue(name, dv.value(i));
    }

    settings->endArray();
  } else if (v.canConvert<QcepPolygon>()) {
    QcepPolygon dv = v.value<QcepPolygon>();

    settings->beginWriteArray(name, dv.count());

    for (int i=0; i<dv.count(); i++) {
      settings->setArrayIndex(i);
//      settings->beginGroup(name);
      settings->setValue("x", dv.value(i).x());
      settings->setValue("y", dv.value(i).y());
//      settings->endGroup();
    }

    settings->endArray();
  } else if (v.canConvert<QwtDoublePoint>()) {
    QwtDoublePoint dv = v.value<QwtDoublePoint>();

    settings->beginGroup(name);

    settings->setValue("x", dv.x());
    settings->setValue("y", dv.y());

    settings->endGroup();
  } else if (v.canConvert<QwtDoubleRect>()) {
    QwtDoubleRect dv = v.value<QwtDoubleRect>();

    settings->beginGroup(name);

    settings->setValue("left",   dv.left());
    settings->setValue("top",    dv.top());
    settings->setValue("right",  dv.right());
    settings->setValue("bottom", dv.bottom());

    settings->endGroup();
  } else if (v.type() == QVariant::StringList) {
    QStringList dv = v.toStringList();

    settings->beginWriteArray(name, dv.count());

    for (int i=0; i<dv.count(); i++) {
      settings->setArrayIndex(i);
      settings->setValue(name, dv.value(i));
    }

    settings->endArray();
  }
}

void QcepProperty::writeSettings(QObject *object, const QMetaObject *meta, QString groupName, QSettings *settings)
{
  if (settings && g_Application) {
    int count = meta->propertyCount();
    int offset = meta->propertyOffset();

    settings->beginGroup(groupName);

    for (int i=offset; i<count; i++) {
      QMetaProperty metaproperty = meta->property(i);

      if (metaproperty.isStored()) {
        const char *name = metaproperty.name();
        QVariant value = object -> property(name);

        if (qcepDebug(DEBUG_PREFS | DEBUG_PROPERTIES)) {
          if (g_Application) {
            g_Application -> printMessage(
                  tr("Save %1/%2 = %3 [%4]")
                  .arg(groupName).arg(name)
                  .arg(value.toString()).arg(value.typeName()));
          }
        }

        setSettingsValue(settings, name, value);
      }
    }

    QByteArray name;

    foreach (name, object->dynamicPropertyNames()) {
      setSettingsValue(settings, name.data(), object->property(name.data()));
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
      if (g_Application && qcepDebug(DEBUG_PREFS | DEBUG_PROPERTIES)) {
        g_Application -> printMessage(
              tr("Load %1/%2 = %3 [%4]")
              .arg(groupName).arg(key)
              .arg(settings->value(key).toString())
              .arg(settings->value(key).typeName()));
      }

      int metaindex = meta->indexOfProperty(qPrintable(key));

      if (metaindex >= 0) {
        QMetaProperty metaproperty = meta->property(metaindex);

        if (metaproperty.isStored()) {
          object -> setProperty(qPrintable(key), settings->value(key));
        } else {
          if (g_Application && qcepDebug(DEBUG_PREFS | DEBUG_PROPERTIES)) {
            g_Application -> printMessage(
                  tr("property %1 of %2 not stored").arg(key)
                  .arg(meta -> className()));
          }
        }
      } else {
        if (g_Application && qcepDebug(DEBUG_PREFS | DEBUG_PROPERTIES)) {
          g_Application -> printMessage(
                tr("property %1 of %2 does not exist")
                .arg(key).arg(meta -> className()));
        }
      }
    }

    settings->endGroup();
  }
}

void QcepProperty::printMessage(QString msg, QDateTime ts)
{
  QcepSettingsSaverPtr saver(m_Saver);

  if (saver) {
    saver->printMessage(msg, ts);
  } else {
    printf("%s\n", qPrintable(msg));
  }
}

void QcepProperty::dumpMetaData(const QMetaObject *meta)
{
  while (meta && g_Application) {
    g_Application -> printMessage(
          tr("MetaData for class %1").arg(meta -> className()));
    g_Application -> printMessage(
          tr(" superClass = %1").HEXARG((void*) meta -> superClass()));
    g_Application -> printMessage(
          tr(" methodCount = %1, methodOffset = %2")
          .arg(meta->methodCount()).arg(meta->methodOffset()));
    g_Application -> printMessage(
          tr(" propertyCount = %1, propertyOffset = %2")
          .arg(meta->propertyCount()).arg(meta->propertyOffset()));

    meta = meta->superClass();
  }
}

QcepDoubleProperty::QcepDoubleProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, double value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

double QcepDoubleProperty::value() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

double QcepDoubleProperty::defaultValue() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepDoubleProperty::setValue(double val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepDoubleProperty::setValue(double %2, int %3) [%4]")
                 .arg(name()).arg(val).arg(index).arg(this->index()));
  }

  //  if (index == this->index()) {
  setValue(val);
  //  }
}

void QcepDoubleProperty::setValue(double val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoubleProperty::setValue(double %2)")
                 .arg(name()).arg(val));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepDoubleProperty::setValue(double %2) [%3]")
                   .arg(name()).arg(val).arg(index()));
    }

    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
    emit valueChanged(tr("%1").arg(m_Value));
  }
}

void QcepDoubleProperty::incValue(double step)
{

  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepDoubleProperty::incValue(double %2)")
                 .arg(name()).arg(step));
  }

  if (step) {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Value += step;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
    emit valueChanged(tr("%1").arg(m_Value));
  }
}

void QcepDoubleProperty::setDefaultValue(double val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepDoubleProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoubleProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

void QcepDoubleProperty::linkTo(QDoubleSpinBox *spinBox)
{
  if (qcepDebug(DEBUG_PROPERTIES || debug())) {
    printMessage(tr("%1: QcepDoubleProperty::linkTo(QDoubleSpinBox *%2)")
                 .arg(name()).HEXARG(spinBox));
  }

  QcepDoublePropertyDoubleSpinBoxHelper *helper
      = new QcepDoublePropertyDoubleSpinBoxHelper(spinBox, this);

  helper->moveToThread(spinBox->thread());
  helper->connect();

  spinBox -> setValue(value());
  spinBox -> setKeyboardTracking(false);

  setWidgetToolTip(spinBox);

  connect(this,   SIGNAL(valueChanged(double, int)), helper, SLOT(setValue(double, int)));
  connect(helper, SIGNAL(valueChanged(double, int)), this,   SLOT(setValue(double, int)));
}

void QcepDoubleProperty::linkTo(QLabel *label)
{
  label -> setText(tr("%1").arg(value()));

  setWidgetToolTip(label);

  connect(this, SIGNAL(valueChanged(QString)), label, SLOT(setText(QString)));
}

void QcepDoubleProperty::linkTo(QProgressBar *progress)
{
  setWidgetToolTip(progress);

  connect(this, SIGNAL(valueChanged(double,int)), progress, SLOT(setValue(int)));
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
    m_Property->printMessage(tr("%1: QcepDoublePropertyDoubleSpinBoxHelper::setValue(double %2, int %3) [%4,%5]")
                             .arg(m_Property->name()).arg(value).arg(index).arg(m_Property->index()).arg(m_DoubleSpinBox->value()));
  }

  if (m_Property->index() == index) {
    if (m_DoubleSpinBox->value() != value) {
      if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
        m_Property->printMessage(tr("%1: QcepDoublePropertyDoubleSpinBoxHelper spinBox %2 set to %3")
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
    m_Property->printMessage(tr("%1: QcepDoublePropertyDoubleSpinBoxHelper::setValue(double %2)")
                             .arg(m_Property->name()).arg(value));
  }

  emit valueChanged(value, m_Property->incIndex(1));
}

QcepIntProperty::QcepIntProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, int value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
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
    printMessage(tr("%1 QcepIntProperty::setValue(int %2, int %3) [%4]")
                 .arg(name()).arg(val).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepIntProperty::setValue(int val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepIntProperty::setValue(int %2)")
                 .arg(name()).arg(val));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepIntProperty::setValue(int %2) [%3]")
                   .arg(name()).arg(val).arg(index()));
    }

    m_Value.fetchAndStoreOrdered(val);

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
    emit valueChanged(tr("%1").arg(m_Value));
  }
}

void QcepIntProperty::incValue(int step)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepIntProperty::incValue(int %2)")
                 .arg(name()).arg(step));
  }

  if (step) {
    m_Value.fetchAndAddOrdered(step);

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
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
    printMessage(tr("%1: QcepIntProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

void QcepIntProperty::linkTo(QSpinBox *spinBox)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepIntProperty::linkTo(QSpinBox *%2)")
                 .arg(name()).HEXARG(spinBox));
  }

  QcepIntPropertySpinBoxHelper *helper
      = new QcepIntPropertySpinBoxHelper(spinBox, this);

  helper->moveToThread(spinBox->thread());
  helper->connect();

  spinBox -> setValue(value());
  spinBox -> setKeyboardTracking(false);

  setWidgetToolTip(spinBox);

  connect(this,   SIGNAL(valueChanged(int, int)), helper, SLOT(setValue(int, int)));
  connect(helper, SIGNAL(valueChanged(int, int)), this,   SLOT(setValue(int, int)));
}

void QcepIntProperty::linkTo(QComboBox *comboBox)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepIntProperty::linkTo(QComboBox *%2)")
                 .arg(name()).HEXARG(comboBox));
  }

  QcepIntPropertyComboBoxHelper *helper
      = new QcepIntPropertyComboBoxHelper(comboBox, this);

  helper->moveToThread(comboBox->thread());
  helper->connect();

  comboBox -> setCurrentIndex(value());

  setWidgetToolTip(comboBox);

  connect(this,   SIGNAL(valueChanged(int, int)),        helper, SLOT(setCurrentIndex(int, int)));
  connect(helper, SIGNAL(currentIndexChanged(int, int)), this,   SLOT(setValue(int, int)));
}

void QcepIntProperty::linkTo(QLabel *label)
{
  {
    label -> setText(tr("%1").arg(value()));

    setWidgetToolTip(label);

    connect(this, SIGNAL(valueChanged(QString)), label, SLOT(setText(QString)));
  }
}

void QcepIntProperty::linkTo(QProgressBar *progress)
{
  setWidgetToolTip(progress);

  connect(this, SIGNAL(valueChanged(int,int)), progress, SLOT(setValue(int)));
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
    m_Property->printMessage(tr("%1: QcepIntPropertySpinBoxHelper::setValue(int %2, int %3) [%4,%5]")
                 .arg(m_Property->name()).arg(value).arg(index).arg(m_Property->index()).arg(m_SpinBox->value()));
  }

  if (m_Property->index() == index) {
    if (m_SpinBox->value() != value) {
      if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
        m_Property->printMessage(tr("%1: QcepIntPropertySpinBoxHelper spinBox %2 set to %3")
                     .arg(m_Property->name()).arg(m_SpinBox->objectName()).arg(value));
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
    m_Property->printMessage(tr("%1: QcepIntPropertySpinBoxHelper::setValue(int %2)")
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
    m_Property->printMessage(tr("%1: QcepIntPropertyComboBoxHelper::setValue(int %2, int %3) [%4,%5]")
                 .arg(m_Property->name()).arg(value).arg(index)
                 .arg(m_Property->index()).arg(m_ComboBox->currentIndex()));
  }

  if (m_Property->index() == index) {
    if (m_ComboBox->currentIndex() != value) {
      if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
        m_Property->printMessage(tr("%1: QcepIntPropertyComboBoxHelper comboBox %2 set to %3")
                     .arg(m_Property->name()).arg(m_ComboBox->objectName()).arg(value));
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
    m_Property->printMessage(tr("%1: QcepIntPropertyComboBoxHelper::setValue(int %2)")
                 .arg(m_Property->name()).arg(value));
  }

  emit currentIndexChanged(value, m_Property->incIndex(1));
}

QcepBoolProperty::QcepBoolProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, bool value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
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
    printMessage(tr("%1 QcepBoolProperty::setValue(bool %2, int %3) [%4]")
                 .arg(name()).arg(val).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepBoolProperty::setValue(bool val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepBoolProperty::setValue(bool %2)")
                 .arg(name()).arg(val));
  }

  if ((int) val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepBoolProperty::setValue(bool %2) [%3]")
                   .arg(name()).arg(val).arg(index()));
    }

    m_Value.fetchAndStoreOrdered(val);

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
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
    printMessage(tr("%1: QcepBoolProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

void QcepBoolProperty::toggle()
{
  setValue(!m_Value);
}

void QcepBoolProperty::linkTo(QAbstractButton *button)
{
  if (qcepDebug(DEBUG_PROPERTIES || debug())) {
    printMessage(tr("%1: QcepBoolProperty::linkTo(QAbstractButton *%2)")
                 .arg(name()).HEXARG(button));
  }

  QcepBoolPropertyButtonHelper *helper
      = new QcepBoolPropertyButtonHelper(button, this);

  helper->moveToThread(button->thread());
  helper->connect();

  button -> setChecked(value());

  setWidgetToolTip(button);

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
    m_Property->printMessage(tr("%1: QcepBoolPropertyButtonHelper::setChecked(bool %2, int %3) [%4,%5]")
                 .arg(m_Property->name()).arg(value).arg(index)
                 .arg(m_Property->index()).arg(m_Button->isChecked()));
  }

  if (m_Property->index() == index) {
    if (m_Button->isChecked() != value) {
      if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
        m_Property->printMessage(tr("%1: QcepBoolPropertyButtonHelper button %2 set to %3")
                     .arg(m_Property->name()).arg(m_Button->objectName()).arg(value));
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
    m_Property->printMessage(tr("%1: QcepBoolPropertySpinBoxHelper::setValue(int %2)")
                 .arg(m_Property->name()).arg(value));
  }

  emit toggled(value, m_Property->incIndex(1));
}

QcepStringProperty::QcepStringProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QString value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QString QcepStringProperty::value() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QString QcepStringProperty::defaultValue() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepStringProperty::setValue(QString val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepStringProperty::setValue(QString \"%2\", int %3) [%4]")
                 .arg(name()).arg(val).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepStringProperty::setValue(QString val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepStringProperty::setValue(QString \"%2\")")
                 .arg(name()).arg(val));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepStringProperty::setValue(QString \"%2\") [%3]")
                   .arg(name()).arg(val).arg(index()));
    }

    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepStringProperty::setDefaultValue(QString val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepStringProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepStringProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

void QcepStringProperty::linkTo(QComboBox *comboBox)
{
  setWidgetToolTip(comboBox);

  printf("QcepStringProperty::linkTo(QComboBox * not implemented yet\n");
}

void QcepStringProperty::linkTo(QLineEdit *lineEdit)
{
  if (qcepDebug(DEBUG_PROPERTIES || debug())) {
    printMessage(tr("%1: QcepStringProperty::linkTo(QLineEdit *%2)")
                 .arg(name()).HEXARG(lineEdit));
  }

  QcepStringPropertyLineEditHelper *helper
      = new QcepStringPropertyLineEditHelper(lineEdit, this);

  helper -> moveToThread(lineEdit->thread());
  helper -> connect();

  lineEdit -> setText(value());

  setWidgetToolTip(lineEdit);

  connect(this,   SIGNAL(valueChanged(QString, int)), helper, SLOT(setText(QString, int)));
  connect(helper, SIGNAL(textEdited(QString, int)),   this,   SLOT(setValue(QString, int)));
}

void QcepStringProperty::linkTo(QLabel *label)
{
  label -> setText(value());

  setWidgetToolTip(label);

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
    m_Property->printMessage(tr("%1: QcepStringPropertyLineEditHelper::setText(QString \"%2\", int %3) [%4,%5]")
                 .arg(m_Property->name()).arg(value).arg(index)
                 .arg(m_Property->index()).arg(m_LineEdit->text()));
  }

  if (m_Property->index() == index) {
    if (m_LineEdit->text() != value) {
      if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
        m_Property->printMessage(tr("%1: QcepStringPropertyLineEditHelper lineEdit %2 set to %3")
                     .arg(m_Property->name()).arg(m_LineEdit->objectName()).arg(value));
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
    m_Property->printMessage(tr("%1: QcepStringPropertyLineEditHelper::setText(QString \"%2\")")
                 .arg(m_Property->name()).arg(value));
  }

  emit textEdited(value, m_Property->incIndex(1));
}

QcepDateTimeProperty::QcepDateTimeProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QDateTime value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QDateTime QcepDateTimeProperty::value() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QDateTime QcepDateTimeProperty::defaultValue() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepDateTimeProperty::setValue(QDateTime val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepDateTimeProperty::setValue(QDateTime \"%2\", int %3) [%4]")
                 .arg(name()).arg(val.toString()).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepDateTimeProperty::setValue(QDateTime val)
{

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDateTimeProperty::setValue(QDateTime \"%2\")")
                 .arg(name()).arg(val.toString()));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepDateTimeProperty::setValue(QDateTime \"%2\") [%3]")
                   .arg(name()).arg(val.toString()).arg(index()));
    }

    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepDateTimeProperty::setDefaultValue(QDateTime val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (val != m_Default) {
    m_Default = val;
  }
}

void QcepDateTimeProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDateTimeProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

QcepDoubleListProperty::QcepDoubleListProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QcepDoubleList value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QcepDoubleList QcepDoubleListProperty::value() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QcepDoubleList QcepDoubleListProperty::defaultValue() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepDoubleListProperty::setValue(QcepDoubleList val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepDoubleListProperty::setValue(QcepDoubleList %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepDoubleListProperty::incValue(QcepDoubleList step)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepDoubleListProperty::incValue(QcepDoubleList %2...)")
                 .arg(name()).arg(step.value(0)));
  }

  {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    for (int i=0; i<m_Value.count(); i++) {
      m_Value[i] += step.value(i);
    }

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepDoubleListProperty::clear()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setValue(QcepDoubleList());
}

void QcepDoubleListProperty::appendValue(double val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
    printMessage(tr("%1 QcepDoubleListProperty::setValue(QcepDoubleList %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepDoubleListProperty::setValue(QcepDoubleList %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepDoubleListProperty::setDefaultValue(QcepDoubleList val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepDoubleListProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoubleListProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

QcepDoubleVectorProperty::QcepDoubleVectorProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QcepDoubleVector value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QcepDoubleVector QcepDoubleVectorProperty::value() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QcepDoubleVector QcepDoubleVectorProperty::defaultValue() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepDoubleVectorProperty::setValue(QcepDoubleVector val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepDoubleVectorProperty::setValue(QcepDoubleVector %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepDoubleVectorProperty::incValue(QcepDoubleVector step)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepDoubleVectorProperty::incValue(QcepDoubleList %2...)")
                 .arg(name()).arg(step.value(0)));
  }

  {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    for (int i=0; i<m_Value.count(); i++) {
      m_Value[i] += step.value(i);
    }

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepDoubleVectorProperty::clear()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setValue(QcepDoubleVector());
}

void QcepDoubleVectorProperty::appendValue(double val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepDoubleVector list = value();
  list.append(val);

  setValue(list);
}

QString QcepDoubleVectorProperty::toString(const QcepDoubleVector &val)
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

void QcepDoubleVectorProperty::setValue(QcepDoubleVector val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 QcepDoubleVectorProperty::setValue(QcepDoubleVector %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepDoubleVectorProperty::setValue(QcepDoubleVector %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepDoubleVectorProperty::setDefaultValue(QcepDoubleVector val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepDoubleVectorProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoubleVectorProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

QcepIntListProperty::QcepIntListProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QcepIntList value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QcepIntList QcepIntListProperty::value() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QcepIntList QcepIntListProperty::defaultValue() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepIntListProperty::setValue(QcepIntList val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepIntListProperty::setValue(QcepIntList %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepIntListProperty::incValue(QcepIntList step)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepIntListProperty::incValue(QcepDoubleList %2...)")
                 .arg(name()).arg(step.value(0)));
  }

  {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    for (int i=0; i<m_Value.count(); i++) {
      m_Value[i] += step.value(i);
    }

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepIntListProperty::clear()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setValue(QcepIntList());
}

void QcepIntListProperty::appendValue(int val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepIntList list = value();
  list.append(val);

  setValue(list);
}

QString QcepIntListProperty::toString(const QcepIntList &val)
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

void QcepIntListProperty::setValue(QcepIntList val)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 QcepIntListProperty::setValue(QcepIntList %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepIntListProperty::setValue(QcepIntList %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepIntListProperty::setDefaultValue(QcepIntList val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepIntListProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepIntListProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

QcepStringListProperty::QcepStringListProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QStringList value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QStringList QcepStringListProperty::value() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QStringList QcepStringListProperty::defaultValue() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepStringListProperty::setValue(QStringList val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepStringListProperty::setValue(QcepStringList %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepStringListProperty::clear()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setValue(QcepStringList());
}

void QcepStringListProperty::appendValue(QString val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
    printMessage(tr("%1 QcepStringListProperty::setValue(QcepStringList %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepStringListProperty::setValue(QcepStringList %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepStringListProperty::setDefaultValue(QStringList val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepStringListProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepStringListProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

QcepByteArrayProperty::QcepByteArrayProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QByteArray value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QByteArray QcepByteArrayProperty::value() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QByteArray QcepByteArrayProperty::defaultValue() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepByteArrayProperty::setValue(QByteArray val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepByteArrayProperty::setValue(QByteArray %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepByteArrayProperty::clear()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
    printMessage(tr("%1 QcepByteArrayProperty::setValue(QByteArray %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepByteArrayProperty::setValue(QByteArray %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepByteArrayProperty::setDefaultValue(QByteArray val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepByteArrayProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepByteArrayProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

QcepDoublePointProperty::QcepDoublePointProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QwtDoublePoint value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QwtDoublePoint QcepDoublePointProperty::value() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QwtDoublePoint QcepDoublePointProperty::defaultValue() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepDoublePointProperty::setValue(QwtDoublePoint val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepDoublePointProperty::setValue(QwtDoublePoint(%2,%3), int %4) [%5]")
                 .arg(name()).arg(val.x()).arg(val.y()).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepDoublePointProperty::setValue(QwtDoublePoint val)
{

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoublePointProperty::setValue(QwtDoublePoint(%2,%3)")
                 .arg(name()).arg(val.x()).arg(val.y()));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepDoublePointProperty::setValue(QwtDoublePoint(%2,%3)) [%4]")
                   .arg(name()).arg(val.x()).arg(val.y()).arg(index()));
    }

    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepDoublePointProperty::setDefaultValue(QwtDoublePoint val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (val != m_Default) {
    m_Default = val;
  }
}

void QcepDoublePointProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoublePointProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

QcepDoubleRectProperty::QcepDoubleRectProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QwtDoubleRect value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QwtDoubleRect QcepDoubleRectProperty::value() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QwtDoubleRect QcepDoubleRectProperty::defaultValue() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepDoubleRectProperty::setValue(QwtDoubleRect val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepDoubleRectProperty::setValue(QwtDoubleRect([%2,%3],[%4,%5]), int %6) [%7]")
                 .arg(name()).arg(val.left()).arg(val.top()).arg(val.right()).arg(val.bottom()).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepDoubleRectProperty::setValue(QwtDoubleRect val)
{

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoubleRectProperty::setValue(QwtDoubleRect([%2,%3],[%4,%5])")
                 .arg(name()).arg(val.top()).arg(val.right()).arg(val.bottom()));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepDoubleRectProperty::setValue(QwtDoubleRect([%2,%3],[%4,%5])) [%6]")
                   .arg(name()).arg(val.top()).arg(val.right()).arg(val.bottom()).arg(index()));
    }

    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepDoubleRectProperty::setDefaultValue(QwtDoubleRect val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (val != m_Default) {
    m_Default = val;
  }
}

void QcepDoubleRectProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoubleRectProperty::resetValue").arg(name()));
  }

  setValue(m_Default);
}

QcepPolygonProperty::QcepPolygonProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QcepPolygon value, QString toolTip)
  : QcepProperty(saver, parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QcepPolygon QcepPolygonProperty::value() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QcepPolygon QcepPolygonProperty::defaultValue() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepPolygonProperty::setValue(QcepPolygon val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepPolygonProperty::setValue(QcepPolygon(...), int %2) [%3]")
                 .arg(name()).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepPolygonProperty::setValue(QcepPolygon val)
{

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepPolygonProperty::setValue(QcepPolygon(...)")
                 .arg(name()));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepPolygonProperty::setValue(QcepPolygon(...)) [%2]")
                   .arg(name()).arg(index()));
    }

    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepPolygonProperty::appendValue(QwtDoublePoint pt)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepPolygon poly = value();
  poly.append(pt);

  setValue(poly);
}

void QcepPolygonProperty::clear()
{
  setValue(QcepPolygon());
}

void QcepPolygonProperty::setDefaultValue(QcepPolygon val)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (val != m_Default) {
    m_Default = val;
  }
}

void QcepPolygonProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepPolygon::resetValue").arg(name()));
  }

  setValue(m_Default);
}

