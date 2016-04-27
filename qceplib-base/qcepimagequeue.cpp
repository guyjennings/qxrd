#include "qcepdebug.h"
#include "qcepimagequeue.h"
#include "qcepimagedata.h"
#include "qcepapplication.h"

template <typename T>
QcepImageQueue<T>::QcepImageQueue(QString name)
  : m_NRows(NULL, "nRows", 2048, "Number of Rows in Queued Images"),
    m_NCols(NULL, "nCols", 2048, "Number of Cols in Queued Images"),
    m_Name(name)
{
  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QcepImageQueue<T>::QcepImageQueue(%1) %2 begin [contains %3]")
                      .arg(m_Name).HEXARG(this).arg(m_Queue.size()));
  }
}

template <typename T>
QcepImageQueue<T>::~QcepImageQueue()
{
  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QcepImageQueue<T>::~QcepImageQueue(%1) %2 begin [contains %3]")
                      .arg(m_Name).HEXARG(this).arg(m_Queue.size()));
  }

  deallocate();

  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QcepImageQueue<T>::~QcepImageQueue(%1) %2 end [contains %3]")
                      .arg(m_Name).HEXARG(this).arg(m_Queue.size()));
  }
}

template <typename T>
void QcepImageQueue<T>::deallocate()
{
  QMutexLocker lock(&m_Lock);

  while (!m_Queue.isEmpty()) {
    QSharedPointer<T> img = m_Queue.dequeue();
    if (g_Application && qcepDebug(DEBUG_QUEUES)) {
      g_Application->printMessage(tr("Deallocate %1").HEXARG(img.data()));
    }
  }
}

template <typename T>
QSharedPointer<T> QcepImageQueue<T>::dequeue()
{
  QMutexLocker lock(&m_Lock);

  if (m_Queue.isEmpty()) {
    if (g_Application && qcepDebug(DEBUG_QUEUES)) {
      g_Application->printMessage(tr("QcepImageQueue::dequeue() = NULL from %1").arg(m_Name));
    }

    return QSharedPointer<T>(NULL);
  } else {
    QSharedPointer<T> res = m_Queue.dequeue();

    if (g_Application && qcepDebug(DEBUG_QUEUES)) {
      g_Application->printMessage(tr("QcepImageQueue::dequeue() = %1 from %2, leaving %3")
                        .HEXARG(res.data()).arg(m_Name).arg(m_Queue.size()));
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

  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QcepImageQueue::enqueue(%1) into %2, starting with %3")
                      .HEXARG(data.data()).arg(m_Name).arg(m_Queue.size()));
  }

  if (data) {
    m_Queue.enqueue(data);
  }
}

template <typename T>
int QcepImageQueue<T>::size() const
{
  QMutexLocker lock(&m_Lock);

  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QcepImageQueue::size() = %1 for %2")
                      .arg(m_Queue.size()).arg(m_Name));
  }

  return m_Queue.size();
}

template class QcepImageQueue<QcepImageDataBase>;
template class QcepImageQueue<QcepUInt16ImageData>;
template class QcepImageQueue<QcepInt16ImageData>;
template class QcepImageQueue<QcepUInt32ImageData>;
template class QcepImageQueue<QcepInt32ImageData>;
template class QcepImageQueue<QcepDoubleImageData>;
template class QcepImageQueue<QcepFloatImageData>;
