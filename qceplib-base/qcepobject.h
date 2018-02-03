#ifndef QCEPOBJECT_H
#define QCEPOBJECT_H

#include "qceplib_global.h"
#include <QObject>
#include <QSettings>
#include "qcepobjectnamer.h"
#include <QDateTime>
#include <QSet>
#include "qobject-ptr.h"
#include "qcepobject-ptr.h"
#include <QVector>
#include "qcepproperty.h"
#include <QScriptValue>
#include "qcepfileformatter-ptr.h"

class QCEP_EXPORT QcepObject : public QObject, public QEnableSharedFromThis<QcepObject>
{
  Q_OBJECT
public:
  QcepObject(QString name);
  virtual ~QcepObject();
  virtual void initialize(QObjectWPtr parent);

  static int allocatedObjects();
  static int deletedObjects();

#ifndef QT_NO_DEBUG
  static QSet<QcepObject*> allocatedObjectsSet();
#endif

  void setParentPtr(QcepObjectWPtr parent);
  QObjectWPtr parentPtr();
  const QObjectWPtr parentPtr() const;

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

  void dumpObjectTreePtr(int level=0);

  int childrenChanged() const;
  QString childrenChangedBy() const;

  QString methodSignatures();
  QString methodSignature(int n);
  int     methodCount();

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

  static QcepObjectPtr readDataObject(QcepFileFormatterPtr fmt);

  void readObjectFmt(QcepFileFormatterPtr fmt);
  void writeObjectFmt(QcepFileFormatterPtr fmt);

  virtual void readObjectData(QcepFileFormatterPtr fmt);
  virtual void writeObjectData(QcepFileFormatterPtr fmt);

private:
  QcepObjectNamer                     m_ObjectNamer;
  bool                                m_Initialized;
  QAtomicInt                          m_ChangeCount;
  QAtomicPointer<QcepProperty>        m_LastChanged;
  QObjectWPtr                         m_Parent;
  //TODO: eliminate?
  QVector<QcepObjectPtr>              m_Children;

protected:
#ifndef QT_NO_DEBUG
  void checkObjectInitialization() const;
#endif

  //TODO: eliminate
#ifndef QT_NO_DEBUG
  void checkPointerMatchCount(QcepObjectWPtr ptr);
#endif

  //TODO: eliminate
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

  //TODO: eliminate
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
