/******************************************************************
*
*  $Id: qxrdimagequeue.h,v 1.8 2009/08/26 20:54:34 jennings Exp $
*
*******************************************************************/

#ifndef QXRDIMAGEQUEUE_H
#define QXRDIMAGEQUEUE_H

#include "qcepmacros.h"

#include "qxrdimagedata.h"
#include <QQueue>
#include <QReadWriteLock>
#include <QString>

template <typename T>
class QxrdImageQueue : QObject
{
public:
  QxrdImageQueue(QString name="");
  ~QxrdImageQueue();

  T* dequeue();
  void enqueue(T *data);
  int size() const;

  void deallocate();
  void preallocate(int nFrames, int nCols, int nRows);

private:
  mutable QReadWriteLock m_Lock;
  QQueue<T*>     m_Queue;
  QString        m_Name;
  int            m_Debug;
  HEADER_IDENT("$Id: qxrdimagequeue.h,v 1.8 2009/08/26 20:54:34 jennings Exp $");
};

typedef QxrdImageQueue<QxrdInt16ImageData>  QxrdInt16ImageQueue;
typedef QxrdImageQueue<QxrdInt32ImageData>  QxrdInt32ImageQueue;
typedef QxrdImageQueue<QxrdDoubleImageData> QxrdDoubleImageQueue;


template <typename T>
QxrdImageQueue<T>::QxrdImageQueue(QString name)
  : m_Name(name),
    m_Debug(0),
    SOURCE_IDENT("$Id: qxrdimagequeue.h,v 1.8 2009/08/26 20:54:34 jennings Exp $")
{
}

template <typename T>
QxrdImageQueue<T>::~QxrdImageQueue()
{
  deallocate();
}

template <typename T>
void QxrdImageQueue<T>::deallocate()
{
  QWriteLocker lock(&m_Lock);

  while (!m_Queue.isEmpty()) {
    T *img = m_Queue.dequeue();

    delete img;
  }
}

template <typename T>
void QxrdImageQueue<T>::preallocate(int nFrames, int nCols, int nRows)
{
  QWriteLocker lock(&m_Lock);

  int sz = m_Queue.size();

  for (int i=nFrames; i<sz; i++) {
    T *img = m_Queue.dequeue();

    delete img;
  }

  for (int i=0; i<m_Queue.size(); i++) {
    m_Queue[i]->resize(nCols, nRows);
  }

  for (int i=m_Queue.size(); i<nFrames; i++) {
    T *img = new T(nCols, nRows);

    m_Queue.enqueue(img);
  }
}

template <typename T>
T* QxrdImageQueue<T>::dequeue()
{
  QWriteLocker lock(&m_Lock);

  if (m_Queue.isEmpty()) {
    printf("QxrdImageQueue::dequeue() = NULL from %s\n", qPrintable(m_Name));
    return NULL;
  } else {
    T* res = m_Queue.dequeue();

    if (m_Debug) {
      printf("QxrdImageQueue::dequeue() = %p from %s\n", res, qPrintable(m_Name));
    }

    return res;
  }
}

template <typename T>
void QxrdImageQueue<T>::enqueue(T *data)
{
  QWriteLocker lock(&m_Lock);

  if (m_Debug) {
    printf("QxrdImageQueue::enqueue(%p) into %s\n", data, qPrintable(m_Name));
  }

  m_Queue.enqueue(data);
}

template <typename T>
int QxrdImageQueue<T>::size() const
{
  QReadLocker lock(&m_Lock);

  if (m_Debug) {
    printf("QxrdImageQueue::size() = %d for %s\n", m_Queue.size(), qPrintable(m_Name));
  }

  return m_Queue.size();
}

#endif

/******************************************************************
*
*  $Log: qxrdimagequeue.h,v $
*  Revision 1.8  2009/08/26 20:54:34  jennings
*  *** empty log message ***
*
*  Revision 1.7  2009/08/26 16:58:53  jennings
*  Partial implementation of the separate Int16 and Int32 acquisition paths
*
*  Revision 1.6  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

