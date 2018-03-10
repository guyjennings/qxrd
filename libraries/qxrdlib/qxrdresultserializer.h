#ifndef QXRDRESULTSERIALIZER_H
#define QXRDRESULTSERIALIZER_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepobject.h"
#include <QMutex>
#include <QMutexLocker>
#include <QFuture>
#include <QQueue>
#include <QFutureWatcher>
#include "qcepimagedata-ptr.h"
#include "qcepintegrateddata-ptr.h"
#include "qxrdhistogramdata-ptr.h"

class QXRD_EXPORT QxrdResultSerializerBase : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QxrdResultSerializerBase(QString name, QcepIntProperty *ctr);
  virtual ~QxrdResultSerializerBase();

signals:
  void resultAvailable();

protected:
  mutable QMutex   m_Mutex;
  QcepIntProperty *m_Counter;

public:
  Q_PROPERTY(int size READ get_Size WRITE set_Size STORED false)
  QCEP_INTEGER_PROPERTY(Size)
};

template <typename T>
class QXRD_EXPORT QxrdResultSerializer : public QxrdResultSerializerBase
{
private:
  typedef QxrdResultSerializerBase inherited;

public:
  explicit QxrdResultSerializer(QString name, QcepIntProperty *ctr);

  void enqueue(QFuture<T> future);
  T dequeue();

  int count() const;

public slots:

private:
  QQueue< QFuture<T> > m_Results;
  QFutureWatcher<T>    m_Watcher;
};

class QXRD_EXPORT QxrdDoubleSerializer : public QxrdResultSerializer<QcepDoubleImageDataPtr> {
  Q_OBJECT

public:
  QxrdDoubleSerializer(QString name, QcepIntProperty *ctr);
};

class QXRD_EXPORT QxrdIntegratedSerializer : public QxrdResultSerializer<QcepIntegratedDataPtr> {
  Q_OBJECT

public:
  QxrdIntegratedSerializer(QString name, QcepIntProperty *ctr);
};

class QXRD_EXPORT QxrdHistogramSerializer : public QxrdResultSerializer<QxrdHistogramDataPtr> {
  Q_OBJECT

public:
  QxrdHistogramSerializer(QString name, QcepIntProperty *ctr);
};

Q_DECLARE_METATYPE(QxrdDoubleSerializer*)
Q_DECLARE_METATYPE(QxrdIntegratedSerializer*)
Q_DECLARE_METATYPE(QxrdHistogramSerializer*)

#endif // QXRDRESULTSERIALIZER_H
