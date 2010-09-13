/******************************************************************
*
*  $Id: qxrdresultserializer.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDRESULTSERIALIZER_H
#define QXRDRESULTSERIALIZER_H

#include "qcepmacros.h"

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QFuture>
#include <QQueue>
#include <QFutureWatcher>

class QxrdResultSerializerBase : public QObject
{
  Q_OBJECT;
public:
  QxrdResultSerializerBase(QObject *parent = 0);
  virtual ~QxrdResultSerializerBase();

signals:
  void resultAvailable();

protected:
  mutable QMutex m_Mutex;

private:
  HEADER_IDENT("$Id: qxrdresultserializer.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

template <typename T>
class QxrdResultSerializer : public QxrdResultSerializerBase
{
public:
    explicit QxrdResultSerializer(QObject *parent = 0);

    void enqueue(QFuture<T> future);
    T dequeue();

public slots:

private:
    QQueue< QFuture<T> > m_Results;
    QFutureWatcher<T>    m_Watcher;
};

template <typename T>
QxrdResultSerializer<T>::QxrdResultSerializer(QObject *parent)
 : QxrdResultSerializerBase(parent)
{
  connect(&m_Watcher, SIGNAL(finished()), this, SIGNAL(resultAvailable()));
}

template <typename T>
void QxrdResultSerializer<T>::enqueue(QFuture<T> future)
{
  QMutexLocker lock(&m_Mutex);

  m_Results.enqueue(future);

  m_Watcher.setFuture(m_Results[0]);
}

template <typename T>
T QxrdResultSerializer<T>::dequeue()
{
  QMutexLocker lock(&m_Mutex);

  if (m_Results.size() > 0) {
    QFuture<T> val = m_Results.dequeue();

    if (m_Results.size() > 0) {
      m_Watcher.setFuture(m_Results[0]);
    }

    return val.result();
  } else {
    return T();
  }
}

#endif // QXRDRESULTSERIALIZER_H

/******************************************************************
*
*  $Log: qxrdresultserializer.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.1  2010/06/17 16:02:36  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*
*******************************************************************/

