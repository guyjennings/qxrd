/******************************************************************
*
*  $Id: qxrdimagequeue.h,v 1.3 2010/10/21 16:31:24 jennings Exp $
*
*******************************************************************/

#ifndef QXRDIMAGEQUEUE_H
#define QXRDIMAGEQUEUE_H

#include "qcepmacros.h"

#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrddoubleimagedata.h"
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
  HEADER_IDENT("$Id: qxrdimagequeue.h,v 1.3 2010/10/21 16:31:24 jennings Exp $");
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
    m_Name(name),
    SOURCE_IDENT("$Id: qxrdimagequeue.h,v 1.3 2010/10/21 16:31:24 jennings Exp $")
{
}

template <typename T>
QxrdImageQueue<T>::~QxrdImageQueue()
{
  QCEP_DEBUG(DEBUG_QUEUES,
             printf("QxrdImageQueue<T>::~QxrdImageQueue(%s) %p begin [contains %d]\n", qPrintable(m_Name), this, m_Queue.size());
  );

  deallocate();

  QCEP_DEBUG(DEBUG_QUEUES,
             printf("QxrdImageQueue<T>::~QxrdImageQueue(%s) %p end [contains %d]\n", qPrintable(m_Name), this, m_Queue.size());
  );
}

template <typename T>
void QxrdImageQueue<T>::deallocate()
{
//  QWriteLocker lock(&m_Lock);
//
  while (!m_Queue.isEmpty()) {
    QSharedPointer<T> img = m_Queue.dequeue();
    QCEP_DEBUG(DEBUG_QUEUES,
               printf("Deallocate %p\n", img.data());
    );
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
    QCEP_DEBUG(DEBUG_QUEUES,
               printf("QxrdImageQueue::dequeue() = NULL from %s\n", qPrintable(m_Name));
    );

    return QSharedPointer<T>(NULL);
  } else {
    QSharedPointer<T> res = m_Queue.dequeue();

    QCEP_DEBUG(DEBUG_QUEUES,
               printf("QxrdImageQueue::dequeue() = %p from %s, leaving %d\n", res.data(), qPrintable(m_Name), m_Queue.size());
    );

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

  QCEP_DEBUG(DEBUG_QUEUES,
    printf("QxrdImageQueue::enqueue(%p) into %s, starting with %d\n", data.data(), qPrintable(m_Name), m_Queue.size());
  );

  if (data) {
    m_Queue.enqueue(data);
  }
}

template <typename T>
int QxrdImageQueue<T>::size() const
{
  QReadLocker lock(&m_Lock);

  QCEP_DEBUG(DEBUG_QUEUES,
    printf("QxrdImageQueue::size() = %d for %s\n", m_Queue.size(), qPrintable(m_Name));
  );

  return m_Queue.size();
}

#endif

/******************************************************************
*
*  $Log: qxrdimagequeue.h,v $
*  Revision 1.3  2010/10/21 16:31:24  jennings
*  Implemented saving of settings soon after they change, rather than at program exit
*
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.8.4.12  2010/06/17 16:02:36  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*  Revision 1.8.4.11  2010/06/11 21:23:30  jennings
*  When calling integrator, pass data, dark and mask images explicitly.
*
*  Revision 1.8.4.10  2010/05/25 18:47:15  jennings
*  Added memory limit handling
*
*  Revision 1.8.4.9  2010/05/24 21:02:39  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.8.4.8  2010/05/17 15:59:53  jennings
*  Changed debugging output to use QCEP_DEBUG macro
*
*  Revision 1.8.4.7  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.8.4.6  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.8.4.5  2010/04/27 19:39:18  jennings
*  More debugging output when deleting image queues
*
*  Revision 1.8.4.4  2010/04/27 15:19:42  jennings
*  More changes to help using QSharedPointer
*
*  Revision 1.8.4.3  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.8.4.2  2010/04/25 03:38:40  jennings
*  Added scrolling area to acquire dialog.  Instrument image queues.
*
*  Revision 1.8.4.1  2010/04/22 19:40:33  jennings
*  Further rearrangement of acquisition code
*
*  Revision 1.8  2009/08/26 20:54:34  jennings
*  Fixed deadlocks in preallocate
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

