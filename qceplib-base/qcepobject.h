#ifndef QCEPOBJECT_H
#define QCEPOBJECT_H

#include <QObject>
#include <QSettings>
#include "qcepobjectnamer.h"
#include <QDateTime>
#include <QSet>
#include "qcepobject-ptr.h"
#include <QVector>
#include "qcepproperty.h"

class QcepObject : public QObject, public QEnableSharedFromThis<QcepObject>
{
  Q_OBJECT
public:
  explicit QcepObject(QString name, QcepObjectWPtr parent);
  virtual ~QcepObject();

  static int allocatedObjects();
  static int deletedObjects();

#ifndef QT_NO_DEBUG
  static QSet<QcepObject*> allocatedObjectsSet();
#endif

signals:

public slots:
  virtual void printLine(QString line);
  virtual void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime()) const;
  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  virtual void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;

  virtual QString settingsScript();
  QString scriptValueLiteral(QVariant v);

  void readObjectSettings(QSettings *set, QString section);

  void dumpObjectTreePtr(int level=0);

public:
  virtual void writeSettings(QSettings *set, QString section);
  virtual void readSettings(QSettings *set, QString section);

  QString get_Name() const;
  void    set_Name(QString name);

  QString get_Type() const;
//  void    set_Type(QString name);

  static QString addSlashes(QString str);

  QcepObjectWPtr parentPtr();
  QVector<QcepObjectPtr> childrenPtr();
  void addChildPtr(QcepObject *child);

private:
  QcepObjectWPtr                      m_Parent;
  QVector<QcepObject*>                m_Children;
  QcepObjectNamer                     m_ObjectNamer;

public:
  Q_PROPERTY(QString name READ get_Name WRITE set_Name STORED false)
//  QCEP_STRING_PROPERTY(Name)

  Q_PROPERTY(QString type READ get_Type /*WRITE set_Type*/ STORED false)
//  QCEP_STRING_PROPERTY(Type)

//  Q_PROPERTY(int typeID READ get_TypeID WRITE set_TypeID STORED false)
//  QCEP_INTEGER_PROPERTY(TypeID)
};

#endif // QCEPOBJECT_H
