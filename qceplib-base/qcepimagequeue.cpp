#include "qcepdebug.h"
#include "qcepimagequeue.h"
#include "qcepimagedata.h"
#include "qcepapplication.h"

QcepImageQueueBase::QcepImageQueueBase(QString name)
  : inherited(name),
    //    m_NRows(this, "nRows", 2048, "Number of Rows in Queued Images"),
    //    m_NCols(this, "nCols", 2048, "Number of Cols in Queued Images"),
    m_Size (this, "size",  0,    "Number of Queued Images"),
    m_Count(this, "count", 0,    "Count of Queued Images")
{
}

QcepImageQueueBase::~QcepImageQueueBase()
{
}

template <typename T>
QcepImageQueue<T>::QcepImageQueue(QString name)
  : inherited(name)
{
  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QcepImageQueue<T>::QcepImageQueue(%1) %2 begin [contains %3]")
                      .arg(get_Name()).HEXARG(this).arg(m_Queue.size()));
  }
}

template <typename T>
QcepImageQueue<T>::~QcepImageQueue()
{
  if (qcepDebug(DEBUG_QUEUES)) {
    printMessage(tr("QcepImageQueue<T>::~QcepImageQueue(%1) %2 begin [contains %3]")
                 .arg(get_Name()).HEXARG(this).arg(m_Queue.size()));
  }

  deallocate();

  if (qcepDebug(DEBUG_QUEUES)) {
    printMessage(tr("QcepImageQueue<T>::~QcepImageQueue(%1) %2 end [contains %3]")
                 .arg(get_Name()).HEXARG(this).arg(m_Queue.size()));
  }
}

template <typename T>
void QcepImageQueue<T>::deallocate()
{
  QMutexLocker lock(&m_Lock);

  while (!m_Queue.isEmpty()) {
    QSharedPointer<T> img = m_Queue.dequeue();
    if (qcepDebug(DEBUG_QUEUES)) {
      printMessage(tr("Deallocate %1").HEXARG(img.data()));
    }
  }
}

template <typename T>
QSharedPointer<T> QcepImageQueue<T>::dequeue()
{
  QMutexLocker lock(&m_Lock);

  if (m_Queue.isEmpty()) {
    if (qcepDebug(DEBUG_QUEUES)) {
      printMessage(tr("QcepImageQueue::dequeue() = NULL from %1").arg(get_Name()));
    }

    return QSharedPointer<T>(NULL);
  } else {
    QSharedPointer<T> res = m_Queue.dequeue();
    prop_Size() -> incValue(-1);

    if (qcepDebug(DEBUG_QUEUES)) {
      printMessage(tr("QcepImageQueue::dequeue() = %1 from %2, leaving %3")
                        .HEXARG(res.data()).arg(get_Name()).arg(m_Queue.size()));
    }

    return res;
  }
}

template <typename T>
QSharedPointer<T> QcepImageQueue<T>::operator[](int n)
{
  QMutexLocker lock(&m_Lock);

  if (n < 0 || n >= m_Queue.size()) {
    return QSharedPointer<T>(NULL);
  } else {
    return m_Queue[n];
  }
}

template <typename T>
void QcepImageQueue<T>::enqueue(QSharedPointer<T> data)
{
  QMutexLocker lock(&m_Lock);

  if (data) {
    m_Queue.enqueue(data);
    prop_Size()  -> incValue(1);
    prop_Count() -> incValue(1);
  }

  if (qcepDebug(DEBUG_QUEUES)) {
    printMessage(tr("QcepImageQueue::enqueue(%1) into %2, starting with %3")
                 .HEXARG(data.data()).arg(get_Name()).arg(m_Queue.size()));
  }
}

//template <typename T>
//int QcepImageQueue<T>::size() const
//{
//  QMutexLocker lock(&m_Lock);

//  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
//    g_Application->printMessage(tr("QcepImageQueue::size() = %1 for %2")
//                      .arg(m_Queue.size()).arg(get_Name()));
//  }

//  return m_Queue.size();
//}

template class QcepImageQueue<QcepImageDataBase>;
template class QcepImageQueue<QcepUInt16ImageData>;
template class QcepImageQueue<QcepInt16ImageData>;
template class QcepImageQueue<QcepUInt32ImageData>;
template class QcepImageQueue<QcepInt32ImageData>;
template class QcepImageQueue<QcepDoubleImageData>;
template class QcepImageQueue<QcepFloatImageData>;
