#include "qxrdintegrateddataqueue.h"

QxrdIntegratedDataQueue::QxrdIntegratedDataQueue(QString name) :
    m_Name(name)
{
}

QxrdIntegratedDataQueue::~QxrdIntegratedDataQueue()
{
  if (qcepDebug(DEBUG_QUEUES)) {
    printf("QxrdIntegratedDataQueue::~QxrdIntegratedDataQueue(%s) %p begin [contains %d]\n",
           qPrintable(m_Name), this, m_Queue.size());
  }

  deallocate();

  if (qcepDebug(DEBUG_QUEUES)) {
    printf("QxrdIntegratedDataQueue::~QxrdIntegratedDataQueue(%s) %p end [contains %d]\n",
           qPrintable(m_Name), this, m_Queue.size());
  }
}

QxrdIntegratedDataPtr QxrdIntegratedDataQueue::dequeue()
{
  QWriteLocker lock(&m_Lock);

  if (m_Queue.isEmpty()) {
    if (qcepDebug(DEBUG_QUEUES)) {
      printf("QxrdIntegratedDataQueue::dequeue() = NULL from %s\n", qPrintable(m_Name));
    }

    return QxrdIntegratedDataPtr(NULL);
  } else {
    QxrdIntegratedDataPtr res = m_Queue.dequeue();

    if (qcepDebug(DEBUG_QUEUES)) {
      printf("QxrdIntegratedDataQueue::dequeue() = %p from %s, leaving %d\n",
             res.data(), qPrintable(m_Name), m_Queue.size());
    }

    return res;
  }
}

void QxrdIntegratedDataQueue::enqueue(QxrdIntegratedDataPtr data)
{
  QWriteLocker lock(&m_Lock);

  if (qcepDebug(DEBUG_QUEUES)) {
    printf("QxrdIntegratedDataQueue::enqueue(%p) into %s, starting with %d\n",
           data.data(), qPrintable(m_Name), m_Queue.size());
  }

  if (data) {
    m_Queue.enqueue(data);
  }
}

int QxrdIntegratedDataQueue::size() const
{
  QReadLocker lock(&m_Lock);

  if (qcepDebug(DEBUG_QUEUES)) {
    printf("QxrdIntegratedDataQueue::size() = %d for %s\n",
           m_Queue.size(), qPrintable(m_Name));
  }

  return m_Queue.size();
}

QxrdIntegratedDataPtr QxrdIntegratedDataQueue::operator[](int n)
{
  QReadLocker lock(&m_Lock);

  if (n < 0 || n >= m_Queue.size()) {
    return QxrdIntegratedDataPtr(NULL);
  } else {
    return m_Queue[n];
  }
}

void QxrdIntegratedDataQueue::deallocate()
{
  while (!m_Queue.isEmpty()) {
    QxrdIntegratedDataPtr img = m_Queue.dequeue();
    if (qcepDebug(DEBUG_QUEUES)) {
      printf("Deallocate %p\n", img.data());
    }
  }
}
