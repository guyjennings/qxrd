#ifndef QCEPSERIALIZABLEOBJECT_H
#define QCEPSERIALIZABLEOBJECT_H

#include "qcepobject.h"
#include "qcepserializableobject-ptr.h"
#include "qcepfileformatter-ptr.h"

class QcepSerializableObject : public QcepObject
{
  Q_OBJECT

public:
  QcepSerializableObject(QString name);

  virtual ~QcepSerializableObject();

  static QcepSerializableObjectPtr readDataObject(QcepFileFormatterPtr fmt);

  void readObject(QcepFileFormatterPtr fmt);
  void writeObject(QcepFileFormatterPtr fmt);

  virtual void readObjectData(QcepFileFormatterPtr fmt);
  virtual void writeObjectData(QcepFileFormatterPtr fmt);

  virtual void addChildPtr(QcepSerializableObjectPtr child);
  virtual void removeChildPtr(QcepSerializableObjectPtr child);

  virtual void propertyChanged(QcepProperty *prop);

  virtual void clearChildren();
  virtual void prependChildPtr(QcepSerializableObjectPtr child);
  virtual void insertChildPtr(int atRow, QcepSerializableObjectPtr child);

public slots:
  void dumpObjectTreePtr(int level=0);

  int childrenChanged() const;
  QString childrenChangedBy() const;

  int childCount() const;

  int checkChildren(int verbose=0, int level=0) const;

  QVector<QcepSerializableObjectPtr> childrenPtr() const;
  QcepSerializableObjectWPtr childPtr(int n) const;

protected:

#ifndef QT_NO_DEBUG
  void checkPointerMatchCount(QcepSerializableObjectWPtr ptr);
#endif

  template <typename T>
  inline bool checkPointer(QcepSerializableObjectWPtr ptr, QSharedPointer<T>& field)
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
  inline bool checkPointer(QcepSerializableObjectWPtr ptr, QWeakPointer<T>& field)
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

private:
  QVector<QcepSerializableObjectPtr>  m_Children;

#ifndef QT_NO_DEBUG
  int                                 m_PointerMatchCount;
#endif
};

#endif // QCEPSERIALIZABLEOBJECT_H
