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
#include "qcepfileformatter-ptr.h"
#include "qcepdebug.h"

class QCEP_EXPORT QcepObject : public QObject, public QEnableSharedFromThis<QcepObject>
{
  Q_OBJECT

public:
  QcepObject(QString name);
  virtual ~QcepObject();
  virtual void initializeRoot();
  virtual void initialize(QcepObjectWPtr parent);

  bool isInitialized() const;

  static int allocatedObjects();
  static int deletedObjects();

#ifndef QT_NO_DEBUG
  static QSet<QcepObject*> allocatedObjectsSet();
#endif

  void setParentPtr(QcepObjectWPtr parent);
  QcepObjectWPtr parentPtr();
  const QcepObjectWPtr parentPtr() const;

  static QString hexArg(const void *p);

signals:

public slots:
  virtual void printLine(QString line) const;
  virtual void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime()) const;
  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  virtual void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  virtual void splashMessage(QString msg, QDateTime dt=QDateTime::currentDateTime());

  virtual QString settingsScript();

  QString toScriptLiteral(QVariant v);
  QVariant fromScriptLiteral(QString lit);

  int isChanged() const;
  QString changedBy() const;
  void setChanged(int ct);

  int childCount() const;
  QcepObjectWPtr childPtr(int n) const;

  virtual void addChildPtr(QcepObjectWPtr child);
  virtual void removeChildPtr(QcepObjectWPtr child);
  virtual void clearChildren();
  virtual void prependChildPtr(QcepObjectWPtr child);
  virtual void insertChildPtr(int atRow, QcepObjectWPtr child);

  int checkChildren(int verbose=0, int level=0) const;

  void dumpObjectTreePtr(int level=0);

  void dumpParentage();

  void openObjectBrowser();

#ifndef QT_NO_DEBUG
  void checkInitialization();
#endif

  int childrenChanged() const;
  QString childrenChangedBy() const;

  QString methodSignatures();
  QString methodSignature(int n);
  int     methodCount();

public:
  static QcepObjectPtr readObject(QcepObjectWPtr parent, QSettings *set);
  static QcepObjectPtr construct(QString name, QString className);

  virtual void writeSettings(QSettings *set);
  virtual void readSettings(QSettings *set);

  QString get_Name() const;
  void    set_Name(QString name);

  QString className() const;

  QString get_Type() const;

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
  void addChildImpl(QcepObjectWPtr child);

private:
  QcepObjectNamer                     m_ObjectNamer;
  bool                                m_Initialized;
  QAtomicInt                          m_ChangeCount;
  QAtomicPointer<QcepProperty>        m_LastChanged;
  QcepObjectWPtr                      m_Parent;
  //TODO: eliminate?
  QVector<QcepObjectWPtr>             m_Children;

protected:
#ifndef QT_NO_DEBUG
  void checkObjectInitialization() const;
#endif

public:
  Q_PROPERTY(QString name READ get_Name WRITE set_Name STORED false)
//  QCEP_STRING_PROPERTY(Name)

  Q_PROPERTY(QString type READ get_Type /*WRITE set_Type*/ STORED false)
//  QCEP_STRING_PROPERTY(Type)

//  Q_PROPERTY(int typeID READ get_TypeID WRITE set_TypeID STORED false)
//  QCEP_INTEGER_PROPERTY(TypeID)
};

#ifndef QT_NO_DEBUG
#define INIT_CHECK checkObjectInitialization()
#else
#define INIT_CHECK do {} while(0)
#endif

#define HEXARG(a) arg(QcepObject::hexArg(a))

Q_DECLARE_METATYPE(QcepObject*)
Q_DECLARE_METATYPE(QcepObjectWPtr)
Q_DECLARE_METATYPE(QcepObjectPtr)

#endif // QCEPOBJECT_H
