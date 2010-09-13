/******************************************************************
*
*  $Id: qcepproperty.h,v 1.2 2010/09/13 20:00:25 jennings Exp $
*
*******************************************************************/

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
  Q_OBJECT;
public:
  QcepProperty(QObject *parent, QString name, QVariant value);
  QVariant variant() const;
  QString name() const;
  static void registerMetaTypes();

//  int debug() const;
//  void setDebug(int dbg);

  static void writeSettings(QObject *object, const QMetaObject *meta, QString groupName, QSettings &settings);
  static void readSettings(QObject *object, const QMetaObject *meta, QString groupName, QSettings &settings);
  static void dumpMetaData(const QMetaObject *meta);

//  virtual void readSettings(QSettings &settings, QString section);
//  virtual void writeSettings(QSettings &settings, QString section);

public slots:
  void changeVariant(QVariant val);

public:
  template <typename T> void changeVariant(T val);

protected:
  mutable QMutex           m_Mutex;
  int                      m_NQueuedUpdates;

private:
//  int                      m_Debug;
  QObject                 *m_Parent;
  QString                  m_Name;
  QVariant                 m_Variant;

  HEADER_IDENT("$Id: qcepproperty.h,v 1.2 2010/09/13 20:00:25 jennings Exp $");
};

class QcepDoubleProperty : public QcepProperty {
  Q_OBJECT;
public:
  QcepDoubleProperty(QObject *parent, QString name, double value);

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
  Q_OBJECT;
public:
  QcepIntProperty(QObject *parent, QString name, int value);

  int value() const;
  int defaultValue() const;

  void linkTo(QSpinBox *spinBox);
  void linkTo(QComboBox *comboBox);
  void linkTo(QLabel *label);

public slots:
  void changeValue(int val);
  void setValue(int val);
  void incValue(int step);
  void setDefaultValue(int val);
  void resetValue();

signals:
  void changedValue(int val);
  void changedValue(QString val);
  void changedDefault(int val);

private:
  int  m_Default;
  int  m_Value;
};

class QcepBoolProperty : public QcepProperty {
  Q_OBJECT;
public:
  QcepBoolProperty(QObject *parent, QString name, bool value);

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
  Q_OBJECT;
public:
  QcepStringProperty(QObject *parent, QString name, QString value);

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
  Q_OBJECT;
public:
  QcepDateTimeProperty(QObject *parent, QString name, QDateTime value);

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
  Q_OBJECT;
public:
  QcepDoubleListProperty(QObject *parent, QString name, QcepDoubleList value);

  QcepDoubleList value() const;
  QcepDoubleList defaultValue() const;

public slots:
  void changeValue(QcepDoubleList val);
//  void changeValue(QString val);
  void setValue(QcepDoubleList val);
  void setDefaultValue(QcepDoubleList val);
  void resetValue();
  void clear();
  void appendValue(double val);

signals:
  void changedValue(QcepDoubleList val);
//  void changedValue(QString val);
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

/******************************************************************
*
*  $Log: qcepproperty.h,v $
*  Revision 1.2  2010/09/13 20:00:25  jennings
*  Merged
*
*  Revision 1.1.2.8  2010/08/16 21:43:58  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.7  2010/07/27 21:53:03  jennings
*  Added double list property type
*  Added 'testImage' script object, interface to QxrdGenerateTestImage
*  Added QxrdDetectorGeometry class
*
*  Revision 1.1.2.6  2010/06/17 18:36:32  jennings
*  Added incValue method to QxrdIntProperty and QxrdDoubleProperty
*  Added QxrdIntProperty -> linkTo(QLabel)
*
*  Revision 1.1.2.5  2010/05/17 15:59:53  jennings
*  Changed debugging output to use QCEP_DEBUG macro
*
*  Revision 1.1.2.4  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
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
*  Revision 1.6  2009/10/02 18:51:14  jennings
*  Added option to link double property to a label - needs more work, though
*
*  Revision 1.5  2009/09/09 22:32:12  jennings
*  Started to add TIFF metadata support
*
*  Revision 1.4  2009/07/03 17:25:53  jennings
*  Found a workable solution for the property widget update loops
*  by keeping track of the number of queued updates to the property
*  and not passing on the update events when the number of queued updates
*  is greater than one.
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
*  Revision 1.15  2009/06/27 05:03:15  jennings
*  Changed qavrgmacros.h to qceplib/qcepmacros.h
*
*  Revision 1.14  2009/06/27 01:19:36  jennings
*  Regsitered remaining container property types
*
*  Revision 1.13  2009/06/26 03:59:15  jennings
*  Added code for different fill pattern handling modes
*
*  Revision 1.12  2009/06/25 12:42:03  jennings
*  Implemented default values for properties - accessed by reset_xxx
*
*  Revision 1.11  2009/06/25 12:01:44  jennings
*  Implemented default values for properties - accessed by reset_xxx
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
*  Revision 1.5  2009/06/17 05:12:13  jennings
*  Made property m_xx member variables private.
*
*  Revision 1.4  2009/06/16 15:37:44  jennings
*  Added CVS log and ID entries
*
*
*******************************************************************/
