#ifndef QCEPPROPERTY_H
#define QCEPPROPERTY_H

#include "qcepmacros.h"

#include <QObject>
#include <QVariant>
#include <QMutex>
#include <QVector>
#include <QList>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QStringList>
#include <QDateTime>
#include "qcepsettingssaver-ptr.h"
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QAbstractButton>
#include <QSettings>
#include <QProgressBar>
#include <qwt_double_rect.h>
#include <QLCDNumber>

class QcepProperty : public QObject {
  Q_OBJECT
public:
  QcepProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QString toolTip);

  QString name() const;

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

  void setSaver(QcepSettingsSaverWPtr saver);

  static void setSettingsValue(QSettings *settings, QString name, QVariant value);
  static void writeSettings(QObject *object, const QMetaObject *meta, QString groupName, QSettings *settings);
  static void readSettings(QObject *object, const QMetaObject *meta, QString groupName, QSettings *settings);
//  static void dumpMetaData(const QMetaObject *meta);

  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

protected:
  mutable QMutex           m_Mutex;
  QcepSettingsSaverWPtr    m_Saver;

private:
  int                      m_Debug;
  int                      m_IsStored;
  const char              *m_Name;
  QAtomicInt               m_Index;
  QObject                 *m_Parent;
  QString                  m_ToolTip;
};

class QcepDoubleProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepDoubleProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, double value, QString toolTip);

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
  void valueChanged(QString val);

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
  QcepIntProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, int value, QString toolTip);

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
  void valueChanged(QString val);

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
  QcepBoolProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, bool value, QString toolTip);

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
  QcepStringProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QString value, QString toolTip);

  QString value() const;
  QString defaultValue() const;

  void linkTo(QComboBox *comboBox);
  void linkTo(QLineEdit *lineEdit);
  void linkTo(QLabel    *label);
  void linkTo(QLCDNumber *number);

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

class QcepDateTimeProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepDateTimeProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QDateTime value, QString toolTip);

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
  QcepDoubleListProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QcepDoubleList value, QString toolTip);

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
  QcepDoubleVectorProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QcepDoubleVector value, QString toolTip);

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
  QcepIntListProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QcepIntList value, QString toolTip);

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

class QcepStringListProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepStringListProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QStringList value, QString toolTip);

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
  QcepByteArrayProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QByteArray value, QString toolTip);

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
  QcepDoublePointProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QwtDoublePoint value, QString toolTip);

  QwtDoublePoint value() const;
  QwtDoublePoint defaultValue() const;

public slots:
  void setValue(QwtDoublePoint val, int index);
  void setValue(QwtDoublePoint val);
  void setDefaultValue(QwtDoublePoint val);
  void resetValue();

signals:
  void valueChanged(QwtDoublePoint val, int index);

private:
  QwtDoublePoint m_Default;
  QwtDoublePoint m_Value;
};

class QcepDoubleRectProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepDoubleRectProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QwtDoubleRect value, QString toolTip);

  QwtDoubleRect value() const;
  QwtDoubleRect defaultValue() const;

public slots:
  void setValue(QwtDoubleRect val, int index);
  void setValue(QwtDoubleRect val);
  void setDefaultValue(QwtDoubleRect val);
  void resetValue();

signals:
  void valueChanged(QwtDoubleRect val, int index);

private:
  QwtDoubleRect m_Default;
  QwtDoubleRect m_Value;
};

class QcepPolygonProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepPolygonProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, QcepPolygon value, QString toolTip);

  QcepPolygon value() const;
  QcepPolygon defaultValue() const;

public slots:
  void setValue(QcepPolygon val, int index);
  void setValue(QcepPolygon val);
  void setDefaultValue(QcepPolygon val);
  void resetValue();
  void appendValue(QwtDoublePoint pt);
  void clear();

signals:
  void valueChanged(QcepPolygon val, int index);

private:
  QcepPolygon m_Default;
  QcepPolygon m_Value;
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
QwtDoublePoint get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QwtDoublePoint val) \
{ \
  m_##propname.setValue(val); \
} \
\
QwtDoublePoint def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QwtDoublePoint val) \
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
QwtDoubleRect get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(QwtDoubleRect val) \
{ \
  m_##propname.setValue(val); \
} \
\
QwtDoubleRect def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(QwtDoubleRect val) \
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

#endif // QCEPPROPERTY_H
