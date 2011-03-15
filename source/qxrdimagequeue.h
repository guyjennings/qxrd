#ifndef QXRDIMAGEQUEUE_H
#define QXRDIMAGEQUEUE_H

#include "qcepmacros.h"

#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include <QQueue>
#include <QReadWriteLock>
#include <QString>
#include <QSharedPointer>

template <typename T>
class QxrdImageQueue : QObject
{
public:
  QxrdImageQueue(QString name);
  ~QxrdImageQueue();

  QSharedPointer<T> dequeue();
  void enqueue(QSharedPointer<T> data);
  int size() const;
  QSharedPointer<T> operator[](int n);

  void deallocate();
//  void preallocate(int nFrames, int nCols, int nRows);

  Q_PROPERTY(int     nRows      READ get_NRows WRITE set_NRows);
  QCEP_INTEGER_PROPERTY(NRows);

  Q_PROPERTY(int     nCols      READ get_NCols WRITE set_NCols);
  QCEP_INTEGER_PROPERTY(NCols);

private:
  mutable QReadWriteLock m_Lock;
  QQueue< QSharedPointer<T> >     m_Queue;
  QString        m_Name;
};

typedef QxrdImageQueue<QxrdInt16ImageData>   QxrdInt16ImageQueue;
typedef QxrdImageQueue<QxrdInt32ImageData>   QxrdInt32ImageQueue;
typedef QxrdImageQueue<QxrdDoubleImageData>  QxrdDoubleImageQueue;
typedef QxrdImageQueue<QxrdMaskData>         QxrdMaskQueue;

typedef QSharedPointer<QxrdInt16ImageQueue>  QxrdInt16ImageQueuePtr;
typedef QSharedPointer<QxrdInt32ImageQueue>  QxrdInt32ImageQueuePtr;
typedef QSharedPointer<QxrdDoubleImageQueue> QxrdDoubleImageQueuePtr;
typedef QSharedPointer<QxrdMaskQueue>        QxrdMaskQueuePtr;


template <typename T>
QxrdImageQueue<T>::QxrdImageQueue(QString name)
  : m_NRows(NULL, "nRows", 2048),
    m_NCols(NULL, "nCols", 2048),
    m_Name(name)
{
}

template <typename T>
QxrdImageQueue<T>::~QxrdImageQueue()
{
  if (qcepDebug(DEBUG_QUEUES)) {
    printf("QxrdImageQueue<T>::~QxrdImageQueue(%s) %p begin [contains %d]\n", qPrintable(m_Name), this, m_Queue.size());
  }

  deallocate();

  if (qcepDebug(DEBUG_QUEUES)) {
    printf("QxrdImageQueue<T>::~QxrdImageQueue(%s) %p end [contains %d]\n", qPrintable(m_Name), this, m_Queue.size());
  }
}

template <typename T>
void QxrdImageQueue<T>::deallocate()
{
//  QWriteLocker lock(&m_Lock);
//
  while (!m_Queue.isEmpty()) {
    QSharedPointer<T> img = m_Queue.dequeue();
    if (qcepDebug(DEBUG_QUEUES)) {
      printf("Deallocate %p\n", img.data());
    }
//
//    delete img;
  }
}
//
//template <typename T>
//void QxrdImageQueue<T>::preallocate(int nFrames, int nCols, int nRows)
//{
//  set_NCols(nCols);
//  set_NRows(nRows);
//
//  QWriteLocker lock(&m_Lock);
//
//  int sz = m_Queue.size();
//
//  for (int i=nFrames; i<sz; i++) {
//    QSharedPointer<T> img = m_Queue.dequeue();
//  }
//
//  for (int i=0; i<m_Queue.size(); i++) {
//    m_Queue[i]->resize(get_NCols(), get_NRows());
//  }
//
//  for (int i=m_Queue.size(); i<nFrames; i++) {
//    T *img = new T(this, get_NCols(), get_NRows());
//
//    m_Queue.enqueue(QSharedPointer<T>(img,&T::recycle));
//  }
//}

template <typename T>
QSharedPointer<T> QxrdImageQueue<T>::dequeue()
{
  QWriteLocker lock(&m_Lock);

  if (m_Queue.isEmpty()) {
    if (qcepDebug(DEBUG_QUEUES)) {
      printf("QxrdImageQueue::dequeue() = NULL from %s\n", qPrintable(m_Name));
    }

    return QSharedPointer<T>(NULL);
  } else {
    QSharedPointer<T> res = m_Queue.dequeue();

    if (qcepDebug(DEBUG_QUEUES)) {
      printf("QxrdImageQueue::dequeue() = %p from %s, leaving %d\n",
             res.data(), qPrintable(m_Name), m_Queue.size());
    }

    return res;
  }
}

template <typename T>
QSharedPointer<T> QxrdImageQueue<T>::operator[](int n)
{
  QReadLocker lock(&m_Lock);

  if (n < 0 || n >= m_Queue.size()) {
    return QSharedPointer<T>(NULL);
  } else {
    return m_Queue[n];
  }
}

template <typename T>
void QxrdImageQueue<T>::enqueue(QSharedPointer<T> data)
{
  QWriteLocker lock(&m_Lock);

  if (qcepDebug(DEBUG_QUEUES)) {
    printf("QxrdImageQueue::enqueue(%p) into %s, starting with %d\n",
           data.data(), qPrintable(m_Name), m_Queue.size());
  }

  if (data) {
    m_Queue.enqueue(data);
  }
}

template <typename T>
int QxrdImageQueue<T>::size() const
{
  QReadLocker lock(&m_Lock);

  if (qcepDebug(DEBUG_QUEUES)) {
    printf("QxrdImageQueue::size() = %d for %s\n", m_Queue.size(), qPrintable(m_Name));
  }

  return m_Queue.size();
}

#endif
