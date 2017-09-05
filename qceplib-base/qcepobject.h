#ifndef QCEPOBJECT_H
#define QCEPOBJECT_H

#include "qceplib_global.h"
#include <QObject>
#include <QSettings>
#include "qcepobjectnamer.h"
#include <QDateTime>
#include <QSet>
#include "qcepobject-ptr.h"
#include <QVector>
#include "qcepproperty.h"
#include <QScriptValue>

class QCEP_EXPORT QcepObject : public QObject, public QEnableSharedFromThis<QcepObject>
{
  Q_OBJECT
public:
  QcepObject(QString name);
  virtual ~QcepObject();

  static int allocatedObjects();
  static int deletedObjects();

#ifndef QT_NO_DEBUG
  static QSet<QcepObject*> allocatedObjectsSet();
#endif

  void setParentPtr(QcepObjectWPtr parent);
  QcepObjectWPtr parentPtr();
  const QcepObjectWPtr parentPtr() const;

signals:

public slots:
  virtual void printLine(QString line) const;
  virtual void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime()) const;
  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  virtual void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;

  virtual QString settingsScript();

  QString toScriptLiteral(QVariant v);
  QVariant fromScriptLiteral(QString lit);

  void readObjectSettings(QSettings *set);

  int isChanged() const;
  QString changedBy() const;
  void setChanged(int ct);

  int childCount() const;
  QcepObjectPtr childPtr(int n) const;

  virtual void addChildPtr(QcepObjectPtr child);
  virtual void removeChildPtr(QcepObjectPtr child);
  virtual void clearChildren();
  virtual void prependChildPtr(QcepObjectPtr child);
  virtual void insertChildPtr(int atRow, QcepObjectPtr child);

  int checkChildren(int verbose=0, int level=0) const;

public:
  static QcepObjectPtr readObject(QSettings *set);
  static QcepObjectPtr construct(QString name, QString className);

  virtual void writeObject(QSettings *set);
  virtual void writeSettings(QSettings *set);
  virtual void readSettings(QSettings *set);

  QString get_Name() const;
  void    set_Name(QString name);

  QString className() const;

  QString get_Type() const;
//  void    set_Type(QString name);

  static QString addSlashes(QString str);
  static QString removeSlashes(QString str);

  QVariant parseVariant(QString str);

  virtual void propertyChanged(QcepProperty *prop);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepObjectPtr &data);
  static void fromScriptValue(const QScriptValue &obj, QcepObjectPtr &data);

private:
  QcepObjectNamer                     m_ObjectNamer;
  QAtomicInt                          m_ChangeCount;
  QAtomicPointer<QcepProperty>        m_LastChanged;
  QcepObjectWPtr                      m_Parent;
  QVector<QcepObjectPtr>              m_Children;

protected:

#ifndef QT_NO_DEBUG
  void checkPointerMatchCount(QcepObjectWPtr ptr);
#endif

  template <typename T>
  inline bool checkPointer(QcepObjectWPtr ptr, QSharedPointer<T>& field)
  {
    QSharedPointer<T> fp = qSharedPointerDynamicCast<T>(ptr);

    if (fp) {
      field = fp;
#ifdef QT_NO_DEBUG
      return true;
#else
      checkPointerMatchCount(ptr);
      return false;
#endif
    } else {
      return false;
    }
  }

  template <typename T>
  inline bool checkPointer(QcepObjectWPtr ptr, QWeakPointer<T>& field)
  {
    QWeakPointer<T> fp = qSharedPointerDynamicCast<T>(ptr);

    if (fp) {
      field = fp;
#ifdef QT_NO_DEBUG
      return true;
#else
      checkPointerMatchCount(ptr);
      return false;
#endif
    } else {
      return false;
    }
  }

#ifndef QT_NO_DEBUG
  int                                 m_PointerMatchCount;
#endif

public:
  Q_PROPERTY(QString name READ get_Name WRITE set_Name STORED false)
//  QCEP_STRING_PROPERTY(Name)

  Q_PROPERTY(QString type READ get_Type /*WRITE set_Type*/ STORED false)
//  QCEP_STRING_PROPERTY(Type)

//  Q_PROPERTY(int typeID READ get_TypeID WRITE set_TypeID STORED false)
//  QCEP_INTEGER_PROPERTY(TypeID)
};

Q_DECLARE_METATYPE(QcepObject*)

#endif // QCEPOBJECT_H
