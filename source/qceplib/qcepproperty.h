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

class QSpinBox;
class QComboBox;
class QLineEdit;
class QDoubleSpinBox;
class QLabel;
class QAbstractButton;
class QSettings;

class QcepProperty : public QObject {
  Q_OBJECT
public:
  QcepProperty(QObject *parent, const char *name);

  QString name() const;

  int index();
  int incIndex(int step);

  int wasLoaded() const;
  void setWasLoaded(int loaded);

  static void registerMetaTypes();

  int debug() const;
  void setDebug(int dbg);

  static void writeSettings(QObject *object, const QMetaObject *meta, QString groupName, QSettings *settings);
  static void readSettings(QObject *object, const QMetaObject *meta, QString groupName, QSettings *settings);
  static void dumpMetaData(const QMetaObject *meta);
  static int  getChangeCount();

protected:
  mutable QMutex           m_Mutex;
  static QAtomicInt        m_ChangeCount;

private:
  int                      m_Debug;
  int                      m_IsStored;
  int                      m_WasLoaded;
  const char              *m_Name;
  QAtomicInt               m_Index;
};

class QcepDoubleProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepDoubleProperty(QObject *parent, const char *name, double value);

  double value() const;
  double defaultValue() const;

  void linkTo(QDoubleSpinBox *spinBox);
  void linkTo(QLabel *label);

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
  QcepIntProperty(QObject *parent, const char *name, int value);

  int value() const;
  int defaultValue() const;

  void linkTo(QSpinBox *spinBox);
  void linkTo(QComboBox *comboBox);
  void linkTo(QLabel *label);

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
  QcepBoolProperty(QObject *parent, const char *name, bool value);

  bool value() const;
  bool defaultValue() const;

  void linkTo(QAbstractButton *button);

public slots:
  void setValue(bool val, int index);
  void setValue(bool val);
  void setDefaultValue(bool val);
  void resetValue();

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
  QcepStringProperty(QObject *parent, const char *name, QString value);

  QString value() const;
  QString defaultValue() const;

  void linkTo(QComboBox *comboBox);
  void linkTo(QLineEdit *lineEdit);
  void linkTo(QLabel    *label);

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

//class QcepStringPropertyComboBoxHelper : public QObject {
//  Q_OBJECT

//public:
//  QcepStringPropertyComboBoxHelper(QComboBox *comboBox, QcepStringProperty *property);
//  void connect();

//public slots:
//  void setEditText(QString value, int index);
//  void setEditText(QString value);

//signals:
//  void editTextChanged(QString value, int index);

//private:
//  QComboBox          *m_ComboBox;
//  QcepStringProperty *m_Property;
//};

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
  QcepDateTimeProperty(QObject *parent, const char *name, QDateTime value);

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
  QcepDoubleListProperty(QObject *parent, const char *name, QcepDoubleList value);

  QcepDoubleList value() const;
  QcepDoubleList defaultValue() const;
  QString toString(const QcepDoubleList &list);

public slots:
  void setValue(QcepDoubleList val, int index);
  void setValue(QcepDoubleList val);
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

class QcepStringListProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepStringListProperty(QObject *parent, const char *name, QStringList value);

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

#endif // QCEPPROPERTY_H
