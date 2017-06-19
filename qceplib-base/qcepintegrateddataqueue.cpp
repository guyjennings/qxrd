#include "qcepdebug.h"
#include "qcepintegrateddataqueue.h"
#include "qcepapplication.h"
#include "qcepintegrateddata.h"

QcepIntegratedDataQueue::QcepIntegratedDataQueue(QString name) :
  m_Name(name)
{
}

QcepIntegratedDataQueue::~QcepIntegratedDataQueue()
{
  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QcepIntegratedDataQueue::~QcepIntegratedDataQueue(%1) %2 begin [contains %3]")
                      .arg(m_Name).HEXARG(this).arg(m_Queue.size()));
  }

  deallocate();

  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QcepIntegratedDataQueue::~QcepIntegratedDataQueue(%1) %2 end [contains %3]")
                      .arg(m_Name).HEXARG(this).arg(m_Queue.size()));
  }
}

QcepIntegratedDataPtr QcepIntegratedDataQueue::dequeue()
{
  QWriteLocker lock(&m_Lock);

  if (m_Queue.isEmpty()) {
    if (g_Application && qcepDebug(DEBUG_QUEUES)) {
      g_Application->printMessage(tr("QcepIntegratedDataQueue::dequeue() = NULL from %1").arg(m_Name));
    }

    return QcepIntegratedDataPtr(NULL);
  } else {
    QcepIntegratedDataPtr res = m_Queue.dequeue();

    if (g_Application && qcepDebug(DEBUG_QUEUES)) {
      g_Application->printMessage(tr("QcepIntegratedDataQueue::dequeue() = %1 from %2, leaving %3")
                        .HEXARG(res.data()).arg(m_Name).arg(m_Queue.size()));
    }

    return res;
  }
}

void QcepIntegratedDataQueue::enqueue(QcepIntegratedDataPtr data)
{
  QWriteLocker lock(&m_Lock);

  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QcepIntegratedDataQueue::enqueue(%1) into %2, starting with %3")
                      .HEXARG(data.data()).arg(m_Name).arg(m_Queue.size()));
  }

  if (data) {
    m_Queue.enqueue(data);
  }
}

int QcepIntegratedDataQueue::size() const
{
  QReadLocker lock(&m_Lock);

  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QcepIntegratedDataQueue::size() = %1 for %2")
                      .arg(m_Queue.size()).arg(m_Name));
  }

  return m_Queue.size();
}

QcepIntegratedDataPtr QcepIntegratedDataQueue::operator[](int n)
{
  QReadLocker lock(&m_Lock);

  if (n < 0 || n >= m_Queue.size()) {
    return QcepIntegratedDataPtr(NULL);
  } else {
    return m_Queue[n];
  }
}

void QcepIntegratedDataQueue::deallocate()
{
  while (!m_Queue.isEmpty()) {
    QcepIntegratedDataPtr img = m_Queue.dequeue();
    if (g_Application && qcepDebug(DEBUG_QUEUES)) {
      g_Application->printMessage(tr("Deallocate %1").HEXARG(img.data()));
    }
  }
}
