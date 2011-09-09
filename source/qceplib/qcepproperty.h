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
  QcepProperty(QObject *parent, const char *name, QVariant value);
  ~QcepProperty();

  QString name() const;

  int index();
  int incIndex(int step = 1);

  int wasLoaded() const;
  void setWasLoaded(int loaded);

  static void registerMetaTypes();

  int debug() const;
  void setDebug(int dbg);

  static void writeSettings(QObject *object, const QMetaObject *meta, QString groupName, QSettings &settings);
  static void readSettings(QObject *object, const QMetaObject *meta, QString groupName, QSettings &settings);
  static void dumpMetaData(const QMetaObject *meta);
  static int  getChangeCount();

protected:
  mutable QMutex           m_Mutex;
  int                      m_NQueuedUpdates;
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
  ~QcepDoubleProperty();

  double value() const;
  double defaultValue() const;

  void linkTo(QComboBox *comboBox);
  void linkTo(QDoubleSpinBox *spinBox);
  void linkTo(QLabel *label);

public slots:
  void changeValue(double val);
  void changeValue(QString val);
  void setValue(double val);
  void incValue(double step);
  void setDefaultValue(double val);
  void resetValue();

signals:
  void changedValue(double val);
  void changedValue(QString val);
  void changedDefault(double val);

private:
  double m_Default;
  double m_Value;
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
  void changedValue(int val, int index);
  void changedValue(int val);
  void changedValue(QString val);
  void changedDefault(int val);

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
  void valueChanged(int value);

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
  void currentIndexChanged(int value);

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
  void changeValue(bool val);
  void setValue(bool val);
  void setDefaultValue(bool val);
  void resetValue();

signals:
  void changedValue(bool val);
  void changedDefault(bool val);

private:
  bool   m_Default;
  bool   m_Value;
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
  void changeValue(QString val);
  void setValue(QString val);
  void setDefaultValue(QString val);
  void resetValue();

signals:
  void changedValue(QString val);
  void changedDefault(QString val);

private:
  QString m_Default;
  QString m_Value;
};

class QcepDateTimeProperty : public QcepProperty {
  Q_OBJECT
public:
  QcepDateTimeProperty(QObject *parent, const char *name, QDateTime value);

  QDateTime value() const;
  QDateTime defaultValue() const;

public slots:
  void changeValue(QDateTime val);
  void setValue(QDateTime val);
  void setDefaultValue(QDateTime val);
  void resetValue();

signals:
  void changedValue(QDateTime val);
  void changedDefault(QDateTime val);

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

public slots:
  void changeValue(QcepDoubleList val);
  void setValue(QcepDoubleList val);
  void setDefaultValue(QcepDoubleList val);
  void resetValue();
  void clear();
  void appendValue(double val);

signals:
  void changedValue(QcepDoubleList val);
  void changedDefault(QcepDoubleList val);

private:
  QcepDoubleList m_Default;
  QcepDoubleList m_Value;
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

#endif // QCEPPROPERTY_H
