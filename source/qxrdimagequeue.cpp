#include "qxrdimagequeue.h"
#include "qxrdimagedata.h"
#include "qxrdapplication.h"

template <typename T>
QxrdImageQueue<T>::QxrdImageQueue(QString name)
  : m_NRows(NULL, NULL, "nRows", 2048),
    m_NCols(NULL, NULL, "nCols", 2048),
    m_Name(name)
{
  if (qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QxrdImageQueue<T>::QxrdImageQueue(%1) %2 begin [contains %3]")
                                .arg(m_Name).HEXARG(this).arg(m_Queue.size()));
  }
}

template <typename T>
QxrdImageQueue<T>::~QxrdImageQueue()
{
  if (qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QxrdImageQueue<T>::~QxrdImageQueue(%1) %2 begin [contains %3]")
                                .arg(m_Name).HEXARG(this).arg(m_Queue.size()));
  }

  deallocate();

  if (qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QxrdImageQueue<T>::~QxrdImageQueue(%1) %2 end [contains %3]")
                                .arg(m_Name).HEXARG(this).arg(m_Queue.size()));
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
      g_Application->printMessage(tr("Deallocate %1").HEXARG(img.data()));
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
      g_Application->printMessage(tr("QxrdImageQueue::dequeue() = NULL from %1").arg(m_Name));
    }

    return QSharedPointer<T>(NULL);
  } else {
    QSharedPointer<T> res = m_Queue.dequeue();

    if (qcepDebug(DEBUG_QUEUES)) {
      g_Application->printMessage(tr("QxrdImageQueue::dequeue() = %1 from %2, leaving %3")
             .HEXARG(res.data()).arg(m_Name).arg(m_Queue.size()));
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
    g_Application->printMessage(tr("QxrdImageQueue::enqueue(%1) into %2, starting with %3")
           .HEXARG(data.data()).arg(m_Name).arg(m_Queue.size()));
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
    g_Application->printMessage(tr("QxrdImageQueue::size() = %1 for %2")
                                .arg(m_Queue.size()).arg(m_Name));
  }

  return m_Queue.size();
}

template class QxrdImageQueue<QxrdImageData<unsigned short> >;
template class QxrdImageQueue<QxrdImageData<unsigned int> >;
