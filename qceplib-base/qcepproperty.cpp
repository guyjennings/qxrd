#include "qcepdebug.h"
#include "qcepproperty.h"
#include "qcepobject.h"
#include "qcepdocumentationdictionary.h"
#include "qcepmutexlocker.h"
#include "qceppropertyvalue.h"
#include "qcepdataobject.h"

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
#include "qcepvector3d.h"
#include "qcepmatrix3x3.h"

QMap<QString, CustomSettingsSaver*> QcepProperty::m_CustomSavers;

QcepProperty::QcepProperty(QcepObject *parent, const char *name, QString toolTip)
  : QObject(),
    m_Mutex(QMutex::Recursive),
    m_Parent(parent),
    m_Debug(false),
    m_IsStored(false),
    m_Name(name),
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

    QCEP_DOC_OBJECT(tr("%1.%2").arg(parent->objectName()).arg(name), toolTip)
  }
}

QString QcepProperty::name() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Name;
}

void QcepProperty::setName(QString name)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Name = name;
}

QString QcepProperty::parentName() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_Parent) {
    return m_Parent->objectName();
  } else {
    return QString();
  }
}

QString QcepProperty::toolTip() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_ToolTip;
}

void QcepProperty::setToolTip(QString tip)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_ToolTip = tip;
}

QString QcepProperty::expandedToolTip() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (widget) {
    QString tt = widget->toolTip();

    if (tt == "") {
      widget->setToolTip(expandedToolTip());
    } else {
      widget->setToolTip(tt + "\n=============\n" + expandedToolTip());
    }
  }
}

int QcepProperty::isStored() const
{
  return m_IsStored;
}

int QcepProperty::index()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Index.fetchAndAddOrdered(0);
}

int QcepProperty::incIndex(int step)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Index.fetchAndAddOrdered(step) + step;
}

void QcepProperty::setDebug(int dbg)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Debug = dbg;
}

int QcepProperty::debug() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Debug;
}

void QcepProperty::registerMetaTypes()
{
  qRegisterMetaType< QcepObject* >("QcepObject*");
  qRegisterMetaType< QcepDataObject*>("QcepDataObject*");
  qRegisterMetaType< QcepDoubleVector >("QcepDoubleVector");
  qRegisterMetaType< QcepBoolVector >("QcepBoolVector");
  qRegisterMetaType< QcepIntVector >("QcepIntVector");
  qRegisterMetaType< QcepStringVector >("QcepStringVector");
  qRegisterMetaType< QcepDoubleList >("QcepDoubleList");
  qRegisterMetaType< QcepBoolList >("QcepBoolList");
  qRegisterMetaType< QcepIntList >("QcepIntList");
  qRegisterMetaType< QcepStringList >("QcepStringList");
  qRegisterMetaType< QPointF >("QPointF");
  qRegisterMetaType< QRectF >("QRectF");
  qRegisterMetaType< QcepPolygon >("QcepPolygon");
  qRegisterMetaType< QVariant >("QVariant");
  qRegisterMetaType< QcepPropertyValue >("QcepPropertyValue");
  qRegisterMetaType< QcepPropertyValue* >("QcepPropertyValue*");
  qRegisterMetaType< QcepMatrix3x3 >("QcepMatrix3x3");
  qRegisterMetaType< QcepVector3D >("QcepVector3D");
  qRegisterMetaTypeStreamOperators< QcepDoubleVector >("QcepDoubleVector");
  qRegisterMetaTypeStreamOperators< QcepBoolVector >("QcepBoolVector");
  qRegisterMetaTypeStreamOperators< QcepIntVector >("QcepIntVector");
  qRegisterMetaTypeStreamOperators< QcepStringVector >("QcepStringVector");
  qRegisterMetaTypeStreamOperators< QcepDoubleList >("QcepDoubleList");
  qRegisterMetaTypeStreamOperators< QcepBoolList >("QcepBoolList");
  qRegisterMetaTypeStreamOperators< QcepIntList >("QcepIntList");
  qRegisterMetaTypeStreamOperators< QcepStringList >("QcepStringList");
  qRegisterMetaTypeStreamOperators< QPointF >("QPointF");
  qRegisterMetaTypeStreamOperators< QRectF >("QRectF");
  qRegisterMetaTypeStreamOperators< QcepPolygon >("QcepPolygon");
  qRegisterMetaTypeStreamOperators< QcepMatrix3x3 >("QcepMatrix3x3");
  qRegisterMetaTypeStreamOperators< QcepVector3D >("QcepVector3D");

  registerCustomSaver("QcepMatrix3x3", QcepMatrix3x3::customSaver);
  registerCustomSaver("QcepVector3D",  QcepVector3D::customSaver);
}

void QcepProperty::setSettingsValue(QSettings *settings, QString name, QVariant v)
{
  settings->setValue(name, v);

//  QVariant::Type typeId = v.type();
  QString typeName = v.typeName();

  if (m_CustomSavers.contains(typeName)) {
    CustomSettingsSaver *saver = m_CustomSavers[typeName];

    if (saver) {
      (*saver)(v, settings, name);
    }
  } else if (v.canConvert<QcepVector3D>()) {
    QcepVector3D pv = v.value<QcepVector3D>();

    pv.setSettingsValue(settings, name);
  } else if (v.canConvert<QcepMatrix3x3>()) {
    QcepMatrix3x3 pv = v.value<QcepMatrix3x3>();

    pv.setSettingsValue(settings, name);
  } else if (v.canConvert<QcepDoubleVector>()){
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
  } else if (v.canConvert<QPointF>()) {
    QPointF dv = v.value<QPointF>();

    settings->beginGroup(name);

    settings->setValue("x", dv.x());
    settings->setValue("y", dv.y());

    settings->endGroup();
  } else if (v.canConvert<QRectF>()) {
    QRectF dv = v.value<QRectF>();

    settings->beginGroup(name);

    settings->setValue("left",   dv.left());
    settings->setValue("top",    dv.top());
    settings->setValue("right",  dv.right());
    settings->setValue("bottom", dv.bottom());

    settings->endGroup();
//  } else if (v.canConvert<QVector3D>()) {
//    QVector3D dv = v.value<QVector3D>();

//    settings->beginGroup(name);

//    settings->setValue("x", dv.x());
//    settings->setValue("y", dv.y());
//    settings->setValue("z", dv.z());

//    settings->endGroup();
//  } else if (v.canConvert<QMatrix3x3>()) {
//    QMatrix3x3 dv = v.value<QMatrix3x3>();

//    settings->beginGroup(name);

//    for (int r=0; r<3; r++) {
//      for (int c=0; c<3; c++) {
//        settings->setValue(tr("r%1c%2").arg(r).arg(c), dv(r,c));
//      }
//    }

//    settings->endGroup();
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

void QcepProperty::writeSettings(QObject *object, QSettings *settings)
{
  if (settings && object) {
    const QMetaObject *meta = object->metaObject();
    int count  = meta->propertyCount();
    int offset = QObject::staticMetaObject.propertyCount();

//    settings->beginGroup("properties");

    for (int i=offset; i<count; i++) {
      QMetaProperty metaproperty = meta->property(i);

//      if (qcepDebug(DEBUG_PROPERTIES)) {
//        QString msg = tr("group = %2")
//            .arg(settings->group());

//        INVOKE_CHECK(QMetaObject::invokeMethod(object, "printMessage",
//                                               Q_ARG(QString, msg),
//                                               Q_ARG(QDateTime, QDateTime::currentDateTime())));
//      }

      if (metaproperty.isStored()) {
        const char *name = metaproperty.name();
        QVariant value = object -> property(name);

        if (qcepDebug(DEBUG_PREFS) || qcepDebug(DEBUG_PROPERTIES)) {
          QString msg = tr("Save %2 = %3 [%4]")
              .arg(name)
              .arg(value.toString())
              .arg(value.typeName());

          INVOKE_CHECK(QMetaObject::invokeMethod(object, "printMessage",
                                                 Q_ARG(QString, msg),
                                                 Q_ARG(QDateTime, QDateTime::currentDateTime())));
        }

        setSettingsValue(settings, name, value);
      }
    }

    QByteArray name;

    foreach (name, object->dynamicPropertyNames()) {
      if (qcepDebug(DEBUG_DYNPROPS)) {
        QString msg = tr("Save dynamic property %2")
            .arg(name.data());

        INVOKE_CHECK(QMetaObject::invokeMethod(object, "printMessage",
                                               Q_ARG(QString, msg),
                                               Q_ARG(QDateTime, QDateTime::currentDateTime())));
      }

      setSettingsValue(settings, name.data(), object->property(name.data()));
    }

//    settings->endGroup();
  }
}

void QcepProperty::readSettings(QObject *object, QSettings *settings)
{
  if (settings && object) {
    const QMetaObject *meta = object->metaObject();
//    settings->beginGroup("properties");

    QStringList keys = settings->childKeys();

    foreach (QString key, keys) {
      if (object && (qcepDebug(DEBUG_PREFS) || qcepDebug(DEBUG_PROPERTIES))) {
        QString msg = tr("Load %2 = %3 [%4]")
            .arg(key)
            .arg(settings->value(key).toString())
            .arg(settings->value(key).typeName());

        INVOKE_CHECK(QMetaObject::invokeMethod(object, "printMessage",
                                               Q_ARG(QString, msg),
                                               Q_ARG(QDateTime, QDateTime::currentDateTime())));
      }

      int metaindex = meta->indexOfProperty(qPrintable(key));

      if (metaindex == 0) { // objectName is ignored on reading
      } else if (metaindex > 0) {
        QMetaProperty metaproperty = meta->property(metaindex);

        if (metaproperty.isStored()) {
          object -> setProperty(qPrintable(key), settings->value(key));
        } else {
          if (object && (qcepDebug(DEBUG_PREFS) || qcepDebug(DEBUG_PROPERTIES))) {
            QString msg = tr("property %1 of %2 not stored").arg(key)
                .arg(meta -> className());

          }
        }
      } else {
        if (object && qcepDebug(DEBUG_DYNPROPS)) {
          QString msg = tr("Dyn Property %1 from group %2 of object %3 class %4")
              .arg(key).arg(settings->group()).arg(object->objectName())
              .arg(meta->className());
          INVOKE_CHECK(QMetaObject::invokeMethod(object, "printMessage",
                                                 Q_ARG(QString, msg),
                                                 Q_ARG(QDateTime, QDateTime::currentDateTime())));
        }

        object -> setProperty(qPrintable(key), settings->value(key));
      }
    }

//    settings->endGroup();
  }
}

void QcepProperty::printMessage(QString msg, QDateTime ts)
{
  if (m_Parent) {
    m_Parent->printMessage(msg, ts);
  } else {
    printf("%s\n", qPrintable(msg));
  }
}

void QcepProperty::registerCustomSaver(QString typeName, CustomSettingsSaver *saver)
{
  m_CustomSavers[typeName] = saver;
}

QcepDoubleProperty::QcepDoubleProperty(QcepObject *parent, const char *name, double value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

double QcepDoubleProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

double QcepDoubleProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepDoubleProperty::setValue(double val, int index)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoubleProperty::setValue(double %2)")
                 .arg(name()).arg(val));
  }

  if (val != value()) {
    if (debug()) {
      printMessage(tr("%1: QcepDoubleProperty::setValue(double %2) [%3]")
                   .arg(name()).arg(val).arg(index()));
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(value(), incIndex(1));
    emit stringValueChanged(tr("%1").arg(value()));
  }
}

void QcepDoubleProperty::incValue(double step)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepDoubleProperty::incValue(double %2)")
                 .arg(name()).arg(step));
  }

  if (step) {
    m_Value += step;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(value(), incIndex(1));
    emit stringValueChanged(tr("%1").arg(value()));
  }
}

void QcepDoubleProperty::setDefaultValue(double val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepDoubleProperty::resetValue()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoubleProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

void QcepDoubleProperty::copyTo(QDoubleSpinBox *spinBox)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  spinBox -> setValue(value());
  spinBox -> setKeyboardTracking(false);

  setWidgetToolTip(spinBox);
}

void QcepDoubleProperty::copyFrom(QDoubleSpinBox *spinBox)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setValue(spinBox->value());
}

void QcepDoubleProperty::linkTo(QDoubleSpinBox *spinBox)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepDoubleProperty::linkTo(QDoubleSpinBox *%2)")
                 .arg(name()).HEXARG(spinBox));
  }

  QcepDoublePropertyDoubleSpinBoxHelper *helper
      = new QcepDoublePropertyDoubleSpinBoxHelper(spinBox, this);

  helper->moveToThread(spinBox->thread());
  helper->connect();

  copyTo(spinBox);

  connect(this,   SIGNAL(valueChanged(double, int)), helper, SLOT(setValue(double, int)));
  connect(helper, SIGNAL(valueChanged(double, int)), this,   SLOT(setValue(double, int)));
}

void QcepDoubleProperty::copyTo(QLabel *label)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  label -> setText(tr("%1").arg(value()));

  setWidgetToolTip(label);
}

void QcepDoubleProperty::linkTo(QLabel *label)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  copyTo(label);

  connect(this, SIGNAL(stringValueChanged(QString)), label, SLOT(setText(QString)));
}

void QcepDoubleProperty::copyTo(QProgressBar *progress)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setValue(value());

  setWidgetToolTip(progress);
}

void QcepDoubleProperty::linkTo(QProgressBar *progress)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  copyTo(progress);

  connect(this, SIGNAL(valueChanged(double,int)), progress, SLOT(setValue(int)));
}

void QcepDoubleProperty::copyTo(QLCDNumber *number)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  number -> display(value());

  setWidgetToolTip(number);
}

void QcepDoubleProperty::linkTo(QLCDNumber *number)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  copyTo(number);

  connect(this, SIGNAL(valueChanged(double,int)), number, SLOT(display(double)));
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

QcepIntProperty::QcepIntProperty(QcepObject *parent, const char *name, int value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

int QcepIntProperty::value() const
{
  return m_Value.load();
}

int QcepIntProperty::defaultValue() const
{
  return m_Default.load();
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

  int curVal = m_Value.load();

  if (val != curVal) {
    if (debug()) {
      printMessage(tr("%1: QcepIntProperty::setValue(int %2) [%3]")
                   .arg(name()).arg(val).arg(index()));
    }

    m_Value.fetchAndStoreOrdered(val);

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(val, incIndex(1));
    emit stringValueChanged(tr("%1").arg(val));
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

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(value(), incIndex(1));
    emit stringValueChanged(tr("%1").arg(value()));
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

  setValue(defaultValue());
}

void QcepIntProperty::copyTo(QSpinBox *spinBox)
{
  spinBox -> setValue(value());
  spinBox -> setKeyboardTracking(false);

  setWidgetToolTip(spinBox);
}

void QcepIntProperty::copyFrom(QSpinBox *spinBox)
{
  setValue(spinBox->value());
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

  copyTo(spinBox);

  connect(this,   SIGNAL(valueChanged(int, int)), helper, SLOT(setValue(int, int)));
  connect(helper, SIGNAL(valueChanged(int, int)), this,   SLOT(setValue(int, int)));
}

void QcepIntProperty::copyTo(QComboBox *comboBox)
{
  comboBox -> setCurrentIndex(value());

  setWidgetToolTip(comboBox);
}

void QcepIntProperty::copyFrom(QComboBox *comboBox)
{
  setValue(comboBox->currentIndex());
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

  copyTo(comboBox);

  connect(this,   SIGNAL(valueChanged(int, int)),        helper, SLOT(setCurrentIndex(int, int)));
  connect(helper, SIGNAL(currentIndexChanged(int, int)), this,   SLOT(setValue(int, int)));
}

void QcepIntProperty::copyTo(QLabel *label)
{
  label -> setText(tr("%1").arg(value()));

  setWidgetToolTip(label);
}

void QcepIntProperty::linkTo(QLabel *label)
{
  copyTo(label);

  connect(this, SIGNAL(stringValueChanged(QString)), label, SLOT(setText(QString)));
}

void QcepIntProperty::copyTo(QProgressBar *progress)
{
  progress -> setValue(value());

  setWidgetToolTip(progress);
}
void QcepIntProperty::linkTo(QProgressBar *progress)
{
  copyTo(progress);

  connect(this, SIGNAL(valueChanged(int,int)), progress, SLOT(setValue(int)));
}

void QcepIntProperty::copyTo(QLCDNumber *number)
{
  number -> display(value());

  setWidgetToolTip(number);
}

void QcepIntProperty::linkTo(QLCDNumber *number)
{
  copyTo(number);

  connect(this, SIGNAL(valueChanged(int,int)), number, SLOT(display(int)));
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

QcepBoolProperty::QcepBoolProperty(QcepObject *parent, const char *name, bool value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

bool QcepBoolProperty::value() const
{
  return m_Value.load();
}

bool QcepBoolProperty::defaultValue() const
{
  return m_Default.load();
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

  int curVal = m_Value.load();

  if ((int) val != curVal) {
    if (debug()) {
      printMessage(tr("%1: QcepBoolProperty::setValue(bool %2) [%3]")
                   .arg(name()).arg(val).arg(index()));
    }

    m_Value.fetchAndStoreOrdered(val);

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(val, incIndex(1));
  }
}

void QcepBoolProperty::setDefaultValue(bool val)
{
  m_Default.fetchAndStoreOrdered(val);
}

void QcepBoolProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepBoolProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

void QcepBoolProperty::toggle()
{
  setValue(!m_Value.load());
}

void QcepBoolProperty::copyTo(QAbstractButton *button)
{
  button -> setChecked(value());

  setWidgetToolTip(button);
}

void QcepBoolProperty::copyFrom(QAbstractButton *button)
{
  setValue(button->isChecked());
}

void QcepBoolProperty::linkTo(QAbstractButton *button)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepBoolProperty::linkTo(QAbstractButton *%2)")
                 .arg(name()).HEXARG(button));
  }

  QcepBoolPropertyButtonHelper *helper
      = new QcepBoolPropertyButtonHelper(button, this);

  helper->moveToThread(button->thread());
  helper->connect();

  copyTo(button);

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
    m_Property->printMessage(tr("%1: QcepBoolPropertyButtonHelper::setChecked(bool %2)")
                 .arg(m_Property->name()).arg(value));
  }

  emit toggled(value, m_Property->incIndex(1));
}

QcepStringProperty::QcepStringProperty(QcepObject *parent, const char *name, QString value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QString QcepStringProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QString QcepStringProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepStringProperty::setValue(QString \"%2\")")
                 .arg(name()).arg(val));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepStringProperty::setValue(QString \"%2\") [%3]")
                   .arg(name()).arg(val).arg(index()));
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepStringProperty::setDefaultValue(QString val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepStringProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepStringProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

void QcepStringProperty::copyTo(QComboBox *comboBox)
{
  int index = comboBox->findText(value());

  comboBox -> setCurrentIndex(index);

  setWidgetToolTip(comboBox);
}

void QcepStringProperty::copyFrom(QComboBox *comboBox)
{
  setValue(comboBox->currentText());
}

void QcepStringProperty::linkTo(QComboBox *comboBox)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepStringProperty::linkTo(QComboBox *%2)")
                 .arg(name()).HEXARG(comboBox));
  }

  QcepStringPropertyComboBoxHelper *helper
      = new QcepStringPropertyComboBoxHelper(comboBox, this);

  helper -> moveToThread(comboBox->thread());
  helper -> connect();

  copyTo(comboBox);

  connect(this,   SIGNAL(valueChanged(QString, int)), helper, SLOT(setValue(QString, int)));
  connect(helper, SIGNAL(valueChanged(QString,int)),   this,   SLOT(setValue(QString, int)));
}

void QcepStringProperty::copyTo(QLineEdit *lineEdit)
{
  lineEdit -> setText(value());

  setWidgetToolTip(lineEdit);
}

void QcepStringProperty::copyFrom(QLineEdit *lineEdit)
{
  setValue(lineEdit->text());
}

void QcepStringProperty::linkTo(QLineEdit *lineEdit)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepStringProperty::linkTo(QLineEdit *%2)")
                 .arg(name()).HEXARG(lineEdit));
  }

  QcepStringPropertyLineEditHelper *helper
      = new QcepStringPropertyLineEditHelper(lineEdit, this);

  helper -> moveToThread(lineEdit->thread());
  helper -> connect();

  copyTo(lineEdit);

  connect(this,   SIGNAL(valueChanged(QString, int)), helper, SLOT(setText(QString, int)));
  connect(helper, SIGNAL(textEdited(QString, int)),   this,   SLOT(setValue(QString, int)));
}

void QcepStringProperty::copyTo(QLabel *label)
{
  label -> setText(value());

  setWidgetToolTip(label);
}

void QcepStringProperty::linkTo(QLabel *label)
{
  copyTo(label);

  connect(this, SIGNAL(valueChanged(QString,int)), label, SLOT(setText(QString)));
}

void QcepStringProperty::copyTo(QLCDNumber *number)
{
  number -> display(value());

  setWidgetToolTip(number);
}

void QcepStringProperty::linkTo(QLCDNumber *number)
{
  copyTo(number);

  connect(this, SIGNAL(valueChanged(QString,int)), number, SLOT(display(QString)));
}

void QcepStringProperty::copyTo(QTextEdit *textEdit)
{
  textEdit -> setText(value());

  setWidgetToolTip(textEdit);
}

void QcepStringProperty::copyFrom(QTextEdit *textEdit)
{
  setValue(textEdit->toPlainText());
}

void QcepStringProperty::linkTo(QTextEdit *textEdit)
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepStringProperty::linkTo(QTextEdit *%2)")
                 .arg(name()).HEXARG(textEdit));
  }

  QcepStringPropertyTextEditHelper *helper
      = new QcepStringPropertyTextEditHelper(textEdit, this);

  helper -> moveToThread(textEdit->thread());
  helper -> connect();

  copyTo(textEdit);

  connect(this,   SIGNAL(valueChanged(QString, int)), helper, SLOT(setText(QString, int)));
  connect(helper, SIGNAL(textEdited(QString, int)),   this,   SLOT(setValue(QString, int)));
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

QcepStringPropertyTextEditHelper::QcepStringPropertyTextEditHelper(QTextEdit *textEdit, QcepStringProperty *property)
  : QObject(textEdit),
    m_TextEdit(textEdit),
    m_Property(property)
{
}

void QcepStringPropertyTextEditHelper::connect()
{
  CONNECT_CHECK(QObject::connect(m_TextEdit, SIGNAL(textChanged()), this, SLOT(setText()), Qt::DirectConnection));
}

void QcepStringPropertyTextEditHelper::setText(QString value, int index)
{
  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    m_Property->printMessage(tr("%1: QcepStringPropertyTextEditHelper::setText(QString \"%2\", int %3) [%4,%5]")
                 .arg(m_Property->name()).arg(value).arg(index)
                 .arg(m_Property->index()).arg(m_TextEdit->toPlainText()));
  }

  if (m_Property->index() == index) {
    if (m_TextEdit->toPlainText() != value) {
      if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
        m_Property->printMessage(tr("%1: QcepStringPropertyTextEditHelper textEdit %2 set to %3")
                     .arg(m_Property->name()).arg(m_TextEdit->objectName()).arg(value));
      }

      bool block = m_TextEdit->blockSignals(true);
      m_TextEdit->setText(value);
      m_TextEdit->blockSignals(block);
    }
  }
}

void QcepStringPropertyTextEditHelper::setText()
{
  QString value = m_TextEdit->toPlainText();

  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    m_Property->printMessage(tr("%1: QcepStringPropertyTextEditHelper::setText(QString \"%2\")")
                 .arg(m_Property->name()).arg(value));
  }

  emit textEdited(value, m_Property->incIndex(1));
}

QcepStringPropertyComboBoxHelper::QcepStringPropertyComboBoxHelper(QComboBox *comboBox, QcepStringProperty *property)
  : QObject(comboBox),
    m_ComboBox(comboBox),
    m_Property(property)
{
}

void QcepStringPropertyComboBoxHelper::connect()
{
  CONNECT_CHECK(QObject::connect(m_ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentIndex(int)), Qt::DirectConnection));
}

void QcepStringPropertyComboBoxHelper::setValue(QString value, int index)
{
  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    m_Property->printMessage(tr("%1: QcepIntPropertyComboBoxHelper::setValue(int %2, int %3) [%4,%5]")
                 .arg(m_Property->name()).arg(value).arg(index)
                 .arg(m_Property->index()).arg(m_ComboBox->currentIndex()));
  }

  if (m_Property->index() == index) {
    int current = m_ComboBox->findData(value);

    if (m_ComboBox->currentIndex() != current) {
      if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
        m_Property->printMessage(tr("%1: QcepIntPropertyComboBoxHelper comboBox %2 set to %3")
                     .arg(m_Property->name()).arg(m_ComboBox->objectName()).arg(value));
      }

      bool block = m_ComboBox->blockSignals(true);

      m_ComboBox->setCurrentIndex(current);
      m_ComboBox->blockSignals(block);
    }
  }
}

void QcepStringPropertyComboBoxHelper::setCurrentIndex(int current)
{
  if (qcepDebug(DEBUG_PROPERTIES) || m_Property->debug()) {
    m_Property->printMessage(tr("%1: QcepStringPropertyComboBoxHelper::setCurrentIndex(int %2)")
                 .arg(m_Property->name()).arg(current));
  }

  QString value = m_ComboBox->itemData(current).toString();

  emit valueChanged(value, m_Property->incIndex(1));
}

QcepDateTimeProperty::QcepDateTimeProperty(QcepObject *parent, const char *name, QDateTime value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QDateTime QcepDateTimeProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QDateTime QcepDateTimeProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDateTimeProperty::setValue(QDateTime \"%2\")")
                 .arg(name()).arg(val.toString()));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepDateTimeProperty::setValue(QDateTime \"%2\") [%3]")
                   .arg(name()).arg(val.toString()).arg(index()));
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepDateTimeProperty::setDefaultValue(QDateTime val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (val != m_Default) {
    m_Default = val;
  }
}

void QcepDateTimeProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDateTimeProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QcepDoubleListProperty::QcepDoubleListProperty(QcepObject *parent, const char *name, QcepDoubleList value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QcepDoubleList QcepDoubleListProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QcepDoubleList QcepDoubleListProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepDoubleListProperty::incValue(QcepDoubleList %2...)")
                 .arg(name()).arg(step.value(0)));
  }

  for (int i=0; i<m_Value.count(); i++) {
    m_Value[i] += step.value(i);
  }

  if (m_Parent) {
    m_Parent->propertyChanged(this);
  }

  emit valueChanged(m_Value, incIndex(1));
}

void QcepDoubleListProperty::clear()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setValue(QcepDoubleList());
}

void QcepDoubleListProperty::appendValue(double val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepDoubleList list = value();
  list.append(val);

  setValue(list);
}

QString QcepDoubleListProperty::toString(const QcepDoubleList &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 QcepDoubleListProperty::setValue(QcepDoubleList %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepDoubleListProperty::setValue(QcepDoubleList %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepDoubleListProperty::setDefaultValue(QcepDoubleList val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepDoubleListProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoubleListProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QcepDoubleVectorProperty::QcepDoubleVectorProperty(QcepObject *parent, const char *name, QcepDoubleVector value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QcepDoubleVector QcepDoubleVectorProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QcepDoubleVector QcepDoubleVectorProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepDoubleVectorProperty::incValue(QcepDoubleVector %2...)")
                 .arg(name()).arg(step.value(0)));
  }

  for (int i=0; i<m_Value.count(); i++) {
    m_Value[i] += step.value(i);
  }

  if (m_Parent) {
    m_Parent->propertyChanged(this);
  }

  emit valueChanged(m_Value, incIndex(1));
}

void QcepDoubleVectorProperty::clear()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setValue(QcepDoubleVector());
}

void QcepDoubleVectorProperty::appendValue(double val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepDoubleVector list = value();
  list.append(val);

  setValue(list);
}

QString QcepDoubleVectorProperty::toString(const QcepDoubleVector &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 QcepDoubleVectorProperty::setValue(QcepDoubleVector %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepDoubleVectorProperty::setValue(QcepDoubleVector %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepDoubleVectorProperty::setDefaultValue(QcepDoubleVector val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepDoubleVectorProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoubleVectorProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QcepIntListProperty::QcepIntListProperty(QcepObject *parent, const char *name, QcepIntList value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QcepIntList QcepIntListProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QcepIntList QcepIntListProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepIntListProperty::incValue(QcepDoubleList %2...)")
                 .arg(name()).arg(step.value(0)));
  }

  for (int i=0; i<m_Value.count(); i++) {
    m_Value[i] += step.value(i);
  }

  if (m_Parent) {
    m_Parent->propertyChanged(this);
  }

  emit valueChanged(m_Value, incIndex(1));
}

void QcepIntListProperty::clear()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setValue(QcepIntList());
}

void QcepIntListProperty::appendValue(int val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepIntList list = value();
  list.append(val);

  setValue(list);
}

QString QcepIntListProperty::toString(const QcepIntList &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 QcepIntListProperty::setValue(QcepIntList %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepIntListProperty::setValue(QcepIntList %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepIntListProperty::setDefaultValue(QcepIntList val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepIntListProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepIntListProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QcepIntVectorProperty::QcepIntVectorProperty(QcepObject *parent, const char *name, QcepIntVector value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QcepIntVector QcepIntVectorProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QcepIntVector QcepIntVectorProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepIntVectorProperty::setValue(QcepIntVector val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepIntVectorProperty::setValue(QcepIntVector %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepIntVectorProperty::incValue(QcepIntVector step)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepIntVectorProperty::incValue(QcepIntVector %2...)")
                 .arg(name()).arg(step.value(0)));
  }

  for (int i=0; i<m_Value.count(); i++) {
    m_Value[i] += step.value(i);
  }

  if (m_Parent) {
    m_Parent->propertyChanged(this);
  }

  emit valueChanged(m_Value, incIndex(1));
}

void QcepIntVectorProperty::clear()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setValue(QcepIntVector());
}

void QcepIntVectorProperty::appendValue(int val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepIntVector list = value();
  list.append(val);

  setValue(list);
}

QString QcepIntVectorProperty::toString(const QcepIntVector &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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

void QcepIntVectorProperty::setValue(QcepIntVector val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 QcepIntVectorProperty::setValue(QcepIntVector %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepIntVectorProperty::setValue(QcepIntVector %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepIntVectorProperty::setDefaultValue(QcepIntVector val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepIntVectorProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepIntVectorProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QcepStringListProperty::QcepStringListProperty(QcepObject *parent, const char *name, QStringList value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QStringList QcepStringListProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QStringList QcepStringListProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setValue(QcepStringList());
}

void QcepStringListProperty::appendValue(QString val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QStringList list = value();
  list.append(val);

  setValue(list);
}

QString QcepStringListProperty::toString(const QStringList &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 QcepStringListProperty::setValue(QcepStringList %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepStringListProperty::setValue(QcepStringList %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepStringListProperty::setDefaultValue(QStringList val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepStringListProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepStringListProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QcepByteArrayProperty::QcepByteArrayProperty(QcepObject *parent, const char *name, QByteArray value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QByteArray QcepByteArrayProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QByteArray QcepByteArrayProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setValue(QByteArray());
}

QString QcepByteArrayProperty::toString(const QByteArray &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 QcepByteArrayProperty::setValue(QByteArray %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepByteArrayProperty::setValue(QByteArray %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepByteArrayProperty::setDefaultValue(QByteArray val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepByteArrayProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepByteArrayProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QcepDoublePointProperty::QcepDoublePointProperty(QcepObject *parent, const char *name, QPointF value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QPointF QcepDoublePointProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QPointF QcepDoublePointProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

double QcepDoublePointProperty::subValue(int axis) const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (axis == 0) {
    return m_Value.x();
  } else {
    return m_Value.y();
  }
}

void QcepDoublePointProperty::setValue(QPointF val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepDoublePointProperty::setValue(QPointF(%2,%3), int %4) [%5]")
                 .arg(name()).arg(val.x()).arg(val.y()).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepDoublePointProperty::setValue(QPointF val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoublePointProperty::setValue(QPointF(%2,%3)")
                 .arg(name()).arg(val.x()).arg(val.y()));
  }

  if (val != m_Value) {
    int newIndex = incIndex(1);

    if (debug()) {
      printMessage(tr("%1: QcepDoublePointProperty::setValue(QPointF(%2,%3)) [%4]")
                   .arg(name()).arg(val.x()).arg(val.y()).arg(index()));
    }

    if (val.x() != m_Value.x()) {
      emit subValueChanged(0, val.x(), newIndex);
    }

    if (val.y() != m_Value.y()) {
      emit subValueChanged(1, val.y(), newIndex);
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(m_Value, newIndex);
  }
}

void QcepDoublePointProperty::setSubValue(int axis, double value, int index)
{
  if (index == this->index()) {
    setSubValue(axis, value);
  }
}

void QcepDoublePointProperty::setSubValue(int axis, double value)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (axis == 0) {
    if (value != m_Value.x())  {
      int newIndex = incIndex(1);

      emit subValueChanged(0, value, newIndex);

      m_Value.setX(value);

      emit valueChanged(m_Value, newIndex);
    }
  } else {
    if (value != m_Value.y())  {
      int newIndex = incIndex(1);

      emit subValueChanged(1, value, newIndex);

      m_Value.setY(value);

      emit valueChanged(m_Value, newIndex);
    }
  }
}

void QcepDoublePointProperty::setDefaultValue(QPointF val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (val != m_Default) {
    m_Default = val;
  }
}

void QcepDoublePointProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoublePointProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

void QcepDoublePointProperty::copyTo(QDoubleSpinBox *xSpinBox, QDoubleSpinBox *ySpinBox)
{
  if (xSpinBox) {
    copyTo(0, xSpinBox);
  }

  if (ySpinBox) {
    copyTo(1, ySpinBox);
  }
}

void QcepDoublePointProperty::copyFrom(QDoubleSpinBox *xSpinBox, QDoubleSpinBox *ySpinBox)
{
  if (xSpinBox) {
    copyFrom(0, xSpinBox);
  }

  if (ySpinBox) {
    copyFrom(1, ySpinBox);
  }
}

void QcepDoublePointProperty::linkTo(QDoubleSpinBox *xSpinBox, QDoubleSpinBox *ySpinBox)
{
  if (xSpinBox) {
    linkTo(0, xSpinBox);
  }

  if (ySpinBox) {
    linkTo(1, ySpinBox);
  }
}

void QcepDoublePointProperty::copyTo(int axis, QDoubleSpinBox *spinBox)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  spinBox -> setValue(subValue(axis));
  spinBox -> setKeyboardTracking(false);

  setWidgetToolTip(spinBox);
}

void QcepDoublePointProperty::copyFrom(int axis, QDoubleSpinBox *spinBox)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  setSubValue(axis, spinBox->value());
}

void QcepDoublePointProperty::linkTo(int axis, QDoubleSpinBox *spinBox)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepDoublePointPropertyDoubleSpinBoxHelper *helper
      = new QcepDoublePointPropertyDoubleSpinBoxHelper(spinBox, this, axis);

  helper->moveToThread(spinBox->thread());
  helper->connect();

  copyTo(axis, spinBox);

  connect(this, SIGNAL(subValueChanged(int,double,int)), helper, SLOT(setSubValue(int,double,int)));
  connect(helper, SIGNAL(subValueChanged(int,double,int)), this, SLOT(setSubValue(int,double,int)));
}

QcepDoublePointPropertyDoubleSpinBoxHelper::QcepDoublePointPropertyDoubleSpinBoxHelper
  (QDoubleSpinBox *spinBox, QcepDoublePointProperty *property, int axis)
  : QObject(spinBox),
    m_DoubleSpinBox(spinBox),
    m_Property(property),
    m_Axis(axis)
{
}

void QcepDoublePointPropertyDoubleSpinBoxHelper::connect()
{
  CONNECT_CHECK(QObject::connect(m_DoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)), Qt::DirectConnection));
}

void QcepDoublePointPropertyDoubleSpinBoxHelper::setSubValue(int axis, double value, int index)
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

void QcepDoublePointPropertyDoubleSpinBoxHelper::setValue(double value)
{
  emit subValueChanged(m_Axis, value, m_Property->incIndex(1));
}

QcepDoubleRectProperty::QcepDoubleRectProperty(QcepObject *parent, const char *name, QRectF value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QRectF QcepDoubleRectProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QRectF QcepDoubleRectProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepDoubleRectProperty::setValue(QRectF val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepDoubleRectProperty::setValue(QRectF([%2,%3],[%4,%5]), int %6) [%7]")
                 .arg(name()).arg(val.left()).arg(val.top()).arg(val.right()).arg(val.bottom()).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepDoubleRectProperty::setValue(QRectF val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoubleRectProperty::setValue(QRectF([%2,%3],[%4,%5])")
                 .arg(name()).arg(val.top()).arg(val.right()).arg(val.bottom()));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepDoubleRectProperty::setValue(QRectF([%2,%3],[%4,%5])) [%6]")
                   .arg(name()).arg(val.top()).arg(val.right()).arg(val.bottom()).arg(index()));
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepDoubleRectProperty::setDefaultValue(QRectF val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (val != m_Default) {
    m_Default = val;
  }
}

void QcepDoubleRectProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepDoubleRectProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QcepPolygonProperty::QcepPolygonProperty(QcepObject *parent, const char *name, QcepPolygon value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

QcepPolygon QcepPolygonProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

QcepPolygon QcepPolygonProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepPolygonProperty::setValue(QcepPolygon(...)")
                 .arg(name()));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepPolygonProperty::setValue(QcepPolygon(...)) [%2]")
                   .arg(name()).arg(index()));
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void QcepPolygonProperty::appendValue(QPointF pt)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (val != m_Default) {
    m_Default = val;
  }
}

void QcepPolygonProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepPolygon::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QcepInt64Property::QcepInt64Property(QcepObject *parent, const char *name, qint64 value, QString toolTip)
  : QcepProperty(parent, name, toolTip),
    m_Default(value),
    m_Value(value)
{
}

qint64 QcepInt64Property::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

qint64 QcepInt64Property::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void QcepInt64Property::setValue(qint64 val, int index)
{
  if (debug()) {
    printMessage(tr("%1 QcepInt64Property::setValue(qint64 %2, int %3) [%4]")
                 .arg(name()).arg(val).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void QcepInt64Property::setValue(qint64 val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: QcepInt64Property::setValue(qint64 %2)")
                 .arg(name()).arg(val));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: QcepInt64Property::setValue(qint64 %2) [%3]")
                   .arg(name()).arg(val).arg(index()));
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(val, incIndex(1));
    emit stringValueChanged(tr("%1").arg(val));
  }
}

void QcepInt64Property::incValue(qint64 step)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepInt64Property::incValue(qint64 %2)")
                 .arg(name()).arg(step));
  }

  if (step) {
    m_Value += step;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(value(), incIndex(1));
    emit stringValueChanged(tr("%1").arg(value()));
  }
}

void QcepInt64Property::setDefaultValue(qint64 val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void QcepInt64Property::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: QcepInt64Property::resetValue").arg(name()));
  }

  setValue(defaultValue());
}


