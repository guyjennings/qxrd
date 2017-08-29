#ifndef QXRDRESULTSERIALIZER_H
#define QXRDRESULTSERIALIZER_H

#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepobject.h"
#include <QMutex>
#include <QMutexLocker>
#include <QFuture>
#include <QQueue>
#include <QFutureWatcher>

class QxrdResultSerializerBase : public QcepObject
{
  Q_OBJECT
public:
  QxrdResultSerializerBase();
  virtual ~QxrdResultSerializerBase();

signals:
  void resultAvailable();

protected:
  mutable QMutex m_Mutex;
};

template <typename T>
class QxrdResultSerializer : public QxrdResultSerializerBase
{
public:
  explicit QxrdResultSerializer(QcepIntProperty *ctr, QcepObjectWPtr parent);

  void enqueue(QFuture<T> future);
  T dequeue();

  int count() const;

public slots:

private:
  QQueue< QFuture<T> > m_Results;
  QFutureWatcher<T>    m_Watcher;
  QcepIntProperty     *m_Counter;
};

#endif // QXRDRESULTSERIALIZER_H