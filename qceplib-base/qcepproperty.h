#ifndef QCEPPROPERTY_H
#define QCEPPROPERTY_H

#include "qcepmacros.h"

#include "qcepobject-ptr.h"
#include "qcepserializableobject-ptr.h"
#include <QVariant>
#include <QMutex>
#include <QVector>
#include <QList>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QStringList>
#include <QDateTime>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QAbstractButton>
#include <QSettings>
#include <QProgressBar>
#include <QLCDNumber>
#include <QTextEdit>

typedef void (CustomSettingsSaver)(const QVariant &val, QSettings *settings, QString name);

class QcepProperty : public QObject {
  Q_OBJECT
public:
  QcepProperty(QcepSerializableObject *parent, const char *name, QString toolTip);

  virtual void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime());

  QString name() const;
  void setName(QString name);

  QString parentName() const;
  QString toolTip() const;
  void setToolTip(QString tip);
  QString expandedToolTip() const;

  void setWidgetToolTip(QWidget *widget);

  int index();
  int incIndex(int step);

  static void registerMetaTypes();

  int debug() const;
  void setDebug(int dbg);

  int isStored() const;

  static void setSettingsValue(QSettings *settings, QString name, QVariant value);
  static void writeSettings(QObject *object, QSettings *settings);
  static void readSettings(QObject *object, QSettings *settings);

  static void registerCustomSaver(QString typeName, CustomSettingsSaver *saver);

protected:
  mutable QMutex           m_Mutex;
  QcepSerializableObject  *m_Parent;

private:
  int                      m_Debug;
  int                      m_IsStored;
  QString                  m_Name;
  QAtomicInt               m_Index;
  QString                  m_ToolTip;

  static QMap<QString, CustomSettingsSaver*> m_CustomSavers;
};

class QcepDoubleProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepDoubleProperty(QcepSerializableObject *parent, const char *name, double value, QString toolTip);

  double value() const;
  double defaultValue() const;

  void linkTo(QDoubleSpinBox *spinBox);
  void linkTo(QLabel *label);
  void linkTo(QProgressBar *progress);
  void linkTo(QLCDNumber *number);

public slots:
  void setValue(double val, int index);
  void setValue(double val);
  void incValue(double step);
  void setDefaultValue(double val);
  void resetValue();

signals:
  void valueChanged(double val, int index);
  void stringValueChanged(QString val);

private:
  double m_Default;
  double m_Value;
};

class QcepDoublePropertyDoubleSpinBoxHelper : public QObject {
  Q_OBJECT

public:
  QcepDoublePropertyDoubleSpinBoxHelper(QDoubleSpinBox *spinBox, QcepDoubleProperty *property);
  void connect();

public slots:
  void setValue(double value, int index);
  void setValue(double value);

signals:
  void valueChanged(double value, int index);

private:
  QDoubleSpinBox     *m_DoubleSpinBox;
  QcepDoubleProperty *m_Property;
};

class QcepIntProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepIntProperty(QcepSerializableObject *parent, const char *name, int value, QString toolTip);

  int value() const;
  int defaultValue() const;

  void linkTo(QSpinBox *spinBox);
  void linkTo(QComboBox *comboBox);
  void linkTo(QLabel *label);
  void linkTo(QProgressBar *progress);
  void linkTo(QLCDNumber *number);

public slots:
  void setValue(int val, int index);
  void setValue(int val);
  void incValue(int step);
  void setDefaultValue(int val);
  void resetValue();

signals:
  void valueChanged(int val, int index);
  void stringValueChanged(QString val);

private:
  QAtomicInt  m_Default;
  QAtomicInt  m_Value;
};

class QcepIntPropertySpinBoxHelper : public QObject {
  Q_OBJECT

public:
  QcepIntPropertySpinBoxHelper(QSpinBox *spinBox, QcepIntProperty *property);
  void connect();

public slots:
  void setValue(int value, int index);
  void setValue(int value);

signals:
  void valueChanged(int value, int index);

private:
  QSpinBox        *m_SpinBox;
  QcepIntProperty *m_Property;
};

class QcepIntPropertyComboBoxHelper : public QObject {
  Q_OBJECT

public:
  QcepIntPropertyComboBoxHelper(QComboBox *comboBox, QcepIntProperty *property);
  void connect();

public slots:
  void setCurrentIndex(int value, int index);
  void setCurrentIndex(int value);

signals:
  void currentIndexChanged(int value, int index);

private:
  QComboBox       *m_ComboBox;
  QcepIntProperty *m_Property;
};

class QcepBoolProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepBoolProperty(QcepSerializableObject *parent, const char *name, bool value, QString toolTip);

  bool value() const;
  bool defaultValue() const;

  void linkTo(QAbstractButton *button);

public slots:
  void setValue(bool val, int index);
  void setValue(bool val);
  void setDefaultValue(bool val);
  void resetValue();
  void toggle();

signals:
  void valueChanged(bool val, int index);

private:
  QAtomicInt   m_Default;
  QAtomicInt   m_Value;
};

class QcepBoolPropertyButtonHelper : public QObject {
  Q_OBJECT

public:
  QcepBoolPropertyButtonHelper(QAbstractButton *button, QcepBoolProperty *property);
  void connect();

public slots:
  void setChecked(bool value, int index);
  void setChecked(bool value);

signals:
  void toggled(bool value, int index);

private:
  QAbstractButton  *m_Button;
  QcepBoolProperty *m_Property;
};

class QcepStringProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepStringProperty(QcepSerializableObject *parent, const char *name, QString value, QString toolTip);

  QString value() const;
  QString defaultValue() const;

  void linkTo(QComboBox *comboBox);
  void linkTo(QLineEdit *lineEdit);
  void linkTo(QLabel    *label);
  void linkTo(QLCDNumber *number);
  void linkTo(QTextEdit *number);

public slots:
  void setValue(QString val, int index);
  void setValue(QString val);
  void setDefaultValue(QString val);
  void resetValue();

signals:
  void valueChanged(QString val, int index);

private:
  QString m_Default;
  QString m_Value;
};

class QcepStringPropertyComboBoxHelper : public QObject {
  Q_OBJECT

public:
  QcepStringPropertyComboBoxHelper(QComboBox *comboBox, QcepStringProperty *property);
  void connect();

public slots:
  void setValue(QString value, int index);
  void setCurrentIndex(int current);

signals:
  void valueChanged(QString value, int index);

private:
  QComboBox          *m_ComboBox;
  QcepStringProperty *m_Property;
};

class QcepStringPropertyLineEditHelper : public QObject {
  Q_OBJECT

public:
  QcepStringPropertyLineEditHelper(QLineEdit *lineEdit, QcepStringProperty *property);
  void connect();

public slots:
  void setText(QString value, int index);
  void setText(QString value);

signals:
  void textEdited(QString value, int index);

private:
  QLineEdit          *m_LineEdit;
  QcepStringProperty *m_Property;
};

class QcepStringPropertyTextEditHelper : public QObject {
  Q_OBJECT

public:
  QcepStringPropertyTextEditHelper(QTextEdit *textEdit, QcepStringProperty *property);
  void connect();

public slots:
  void setText(QString value, int index);
  void setText();

signals:
  void textEdited(QString value, int index);

private:
  QTextEdit          *m_TextEdit;
  QcepStringProperty *m_Property;
};

class QcepDateTimeProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepDateTimeProperty(QcepSerializableObject *parent, const char *name, QDateTime value, QString toolTip);

  QDateTime value() const;
  QDateTime defaultValue() const;

public slots:
  void setValue(QDateTime val, int index);
  void setValue(QDateTime val);
  void setDefaultValue(QDateTime val);
  void resetValue();

signals:
  void valueChanged(QDateTime val, int index);

private:
  QDateTime m_Default;
  QDateTime m_Value;
};


class QcepDoubleListProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepDoubleListProperty(QcepSerializableObject *parent, const char *name, QcepDoubleList value, QString toolTip);

  QcepDoubleList value() const;
  QcepDoubleList defaultValue() const;
  QString toString(const QcepDoubleList &list);

public slots:
  void setValue(QcepDoubleList val, int index);
  void setValue(QcepDoubleList val);
  void incValue(QcepDoubleList step);
  void setDefaultValue(QcepDoubleList val);
  void resetValue();
  void clear();
  void appendValue(double val);

signals:
  void valueChanged(QcepDoubleList val, int index);

private:
  QcepDoubleList m_Default;
  QcepDoubleList m_Value;
};

class QcepDoubleVectorProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepDoubleVectorProperty(QcepSerializableObject *parent, const char *name, QcepDoubleVector value, QString toolTip);

  QcepDoubleVector value() const;
  QcepDoubleVector defaultValue() const;
  QString toString(const QcepDoubleVector &list);

public slots:
  void setValue(QcepDoubleVector val, int index);
  void setValue(QcepDoubleVector val);
  void incValue(QcepDoubleVector step);
  void setDefaultValue(QcepDoubleVector val);
  void resetValue();
  void clear();
  void appendValue(double val);

signals:
  void valueChanged(QcepDoubleVector val, int index);

private:
  QcepDoubleVector m_Default;
  QcepDoubleVector m_Value;
};

class QcepIntListProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepIntListProperty(QcepSerializableObject *parent, const char *name, QcepIntList value, QString toolTip);

  QcepIntList value() const;
  QcepIntList defaultValue() const;
  QString toString(const QcepIntList &list);

public slots:
  void setValue(QcepIntList val, int index);
  void setValue(QcepIntList val);
  void incValue(QcepIntList step);
  void setDefaultValue(QcepIntList val);
  void resetValue();
  void clear();
  void appendValue(int val);

signals:
  void valueChanged(QcepIntList val, int index);

private:
  QcepIntList m_Default;
  QcepIntList m_Value;
};

class QcepIntVectorProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepIntVectorProperty(QcepSerializableObject *parent, const char *name, QcepIntVector value, QString toolTip);

  QcepIntVector value() const;
  QcepIntVector defaultValue() const;
  QString toString(const QcepIntVector &list);

public slots:
  void setValue(QcepIntVector val, int index);
  void setValue(QcepIntVector val);
  void incValue(QcepIntVector step);
  void setDefaultValue(QcepIntVector val);
  void resetValue();
  void clear();
  void appendValue(int val);

signals:
  void valueChanged(QcepIntVector val, int index);

private:
  QcepIntVector m_Default;
  QcepIntVector m_Value;
};

class QcepStringListProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepStringListProperty(QcepSerializableObject *parent, const char *name, QStringList value, QString toolTip);

  QStringList value() const;
  QStringList defaultValue() const;
  QString toString(const QStringList &list);

public slots:
  void setValue(QStringList val, int index);
  void setValue(QStringList val);
  void setDefaultValue(QStringList val);
  void resetValue();
  void clear();
  void appendValue(QString val);

signals:
  void valueChanged(QStringList val, int index);

private:
  QStringList m_Default;
  QStringList m_Value;
};

class QcepByteArrayProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepByteArrayProperty(QcepSerializableObject *parent, const char *name, QByteArray value, QString toolTip);

  QByteArray value() const;
  QByteArray defaultValue() const;
  QString toString(const QByteArray &list);

public slots:
  void setValue(QByteArray val, int index);
  void setValue(QByteArray val);
  void setDefaultValue(QByteArray val);
  void resetValue();
  void clear();

signals:
  void valueChanged(QByteArray val, int index);

private:
  QByteArray m_Default;
  QByteArray m_Value;
};

class QcepDoublePointProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepDoublePointProperty(QcepSerializableObject *parent, const char *name, QPointF value, QString toolTip);

  QPointF value() const;
  QPointF defaultValue() const;
  double subValue(int axis) const;

  void linkTo(QDoubleSpinBox *xSpinBox, QDoubleSpinBox *ySpinBox);
  void linkTo(int axis, QDoubleSpinBox *spinBox);

public slots:
  void setValue(QPointF val, int index);
  void setValue(QPointF val);
  void setDefaultValue(QPointF val);
  void resetValue();
  void setSubValue(int axis, double value, int index);
  void setSubValue(int axis, double value);

signals:
  void valueChanged(QPointF val, int index);
  void subValueChanged(int axis, double val, int index);

private:
  QPointF m_Default;
  QPointF m_Value;
};

class QcepDoublePointPropertyDoubleSpinBoxHelper : public QObject {
  Q_OBJECT

public:
  QcepDoublePointPropertyDoubleSpinBoxHelper(QDoubleSpinBox *spinBox, QcepDoublePointProperty *property, int axis);
  void connect();

public slots:
  void setSubValue(int axis, double value, int index);
  void setValue(double value);

signals:
  void subValueChanged(int axis, double value, int index);

private:
  QDoubleSpinBox             *m_DoubleSpinBox;
  QcepDoublePointProperty    *m_Property;
  int                         m_Axis;
};

class QcepDoubleRectProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepDoubleRectProperty(QcepSerializableObject *parent, const char *name, QRectF value, QString toolTip);

  QRectF value() const;
  QRectF defaultValue() const;

public slots:
  void setValue(QRectF val, int index);
  void setValue(QRectF val);
  void setDefaultValue(QRectF val);
  void resetValue();

signals:
  void valueChanged(QRectF val, int index);

private:
  QRectF m_Default;
  QRectF m_Value;
};

class QcepPolygonProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepPolygonProperty(QcepSerializableObject *parent, const char *name, QcepPolygon value, QString toolTip);

  QcepPolygon value() const;
  QcepPolygon defaultValue() const;

public slots:
  void setValue(QcepPolygon val, int index);
  void setValue(QcepPolygon val);
  void setDefaultValue(QcepPolygon val);
  void resetValue();
  void appendValue(QPointF pt);
  void clear();

signals:
  void valueChanged(QcepPolygon val, int index);

private:
  QcepPolygon m_Default;
  QcepPolygon m_Value;
};

class QcepInt64Property : public QcepProperty {
  Q_OBJECT
public:
  QcepInt64Property(QcepSerializableObject *parent, const char *name, qint64 value, QString toolTip);

  qint64 value() const;
  qint64 defaultValue() const;

//  void linkTo(QSpinBox *spinBox);
//  void linkTo(QComboBox *comboBox);
//  void linkTo(QLabel *label);
//  void linkTo(QProgressBar *progress);
//  void linkTo(QLCDNumber *number);

public slots:
  void setValue(qint64 val, int index);
  void setValue(qint64 val);
  void incValue(qint64 step);
  void setDefaultValue(qint64 val);
  void resetValue();

signals:
  void valueChanged(qint64 val, int index);
  void stringValueChanged(QString val);

private:
  qint64  m_Default;
  qint64  m_Value;
};

#define QCEP_DOUBLE_PROPERTY(propname) \
public: \
double get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(double val) \
{ \
  m_##propname.setValue(val); \
} \
\
double def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(double val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepDoubleProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepDoubleProperty m_##propname;


#define QCEP_STRING_PROPERTY(propname) \
public: \
QString get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QString val) \
{ \
  m_##propname.setValue(val); \
} \
\
QString def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QString val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepStringProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepStringProperty m_##propname;

#define QCEP_INTEGER_PROPERTY(propname) \
public: \
int get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(int val) \
{ \
  m_##propname.setValue(val); \
} \
\
int def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(int val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepIntProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepIntProperty m_##propname;

#define QCEP_BOOLEAN_PROPERTY(propname) \
public: \
bool get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(bool val) \
{ \
  m_##propname.setValue(val); \
} \
\
bool def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(bool val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
void toggle_##propname() \
{ \
  m_##propname.toggle(); \
} \
\
QcepBoolProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepBoolProperty m_##propname;

#define QCEP_DATETIME_PROPERTY(propname) \
public: \
QDateTime get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QDateTime val) \
{ \
  m_##propname.setValue(val); \
} \
\
QDateTime def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QDateTime val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepDateTimeProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepDateTimeProperty m_##propname;

#define QCEP_DOUBLE_LIST_PROPERTY(propname) \
public: \
QcepDoubleList get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QcepDoubleList val) \
{ \
  m_##propname.setValue(val); \
} \
\
QcepDoubleList def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QcepDoubleList val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepDoubleListProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepDoubleListProperty m_##propname;

#define QCEP_DOUBLE_VECTOR_PROPERTY(propname) \
public: \
QcepDoubleVector get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QcepDoubleVector val) \
{ \
  m_##propname.setValue(val); \
} \
\
QcepDoubleVector def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QcepDoubleVector val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepDoubleVectorProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepDoubleVectorProperty m_##propname;

#define QCEP_INTEGER_LIST_PROPERTY(propname) \
public: \
QcepIntList get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QcepIntList val) \
{ \
  m_##propname.setValue(val); \
} \
\
QcepIntList def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QcepIntList val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepIntListProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepIntListProperty m_##propname;

#define QCEP_INTEGER_VECTOR_PROPERTY(propname) \
public: \
QcepIntVector get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QcepIntVector val) \
{ \
  m_##propname.setValue(val); \
} \
\
QcepIntVector def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QcepIntVector val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepIntVectorProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepIntVectorProperty m_##propname;

#define QCEP_STRING_LIST_PROPERTY(propname) \
public: \
QStringList get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QStringList val) \
{ \
  m_##propname.setValue(val); \
} \
\
QcepStringList def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QStringList val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepStringListProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepStringListProperty m_##propname;

#define QCEP_BYTE_ARRAY_PROPERTY(propname) \
public: \
QByteArray get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QByteArray val) \
{ \
  m_##propname.setValue(val); \
} \
\
QByteArray def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QByteArray val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepByteArrayProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepByteArrayProperty m_##propname;

#define QCEP_DOUBLE_POINT_PROPERTY(propname) \
public: \
QPointF get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QPointF val) \
{ \
  m_##propname.setValue(val); \
} \
\
QPointF def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QPointF val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepDoublePointProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepDoublePointProperty m_##propname;

#define QCEP_DOUBLE_RECT_PROPERTY(propname) \
public: \
QRectF get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QRectF val) \
{ \
  m_##propname.setValue(val); \
} \
\
QRectF def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QRectF val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepDoubleRectProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepDoubleRectProperty m_##propname;

#define QCEP_POLYGON_PROPERTY(propname) \
public: \
QcepPolygon get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QcepPolygon val) \
{ \
  m_##propname.setValue(val); \
} \
\
QcepPolygon def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QcepPolygon val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepPolygonProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepPolygonProperty m_##propname;

#define QCEP_INTEGER64_PROPERTY(propname) \
public: \
qint64 get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(qint64 val) \
{ \
  m_##propname.setValue(val); \
} \
\
qint64 def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(qint64 val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
QcepInt64Property *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
QcepInt64Property m_##propname;

#endif // QCEPPROPERTY_H
