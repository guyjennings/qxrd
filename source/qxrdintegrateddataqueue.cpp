#include "qxrddebug.h"
#include "qxrdintegrateddataqueue.h"
#include "qxrdapplication.h"
#include "qxrdintegrateddata.h"

QxrdIntegratedDataQueue::QxrdIntegratedDataQueue(QString name) :
  m_Name(name)
{
}

QxrdIntegratedDataQueue::~QxrdIntegratedDataQueue()
{
  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QxrdIntegratedDataQueue::~QxrdIntegratedDataQueue(%1) %2 begin [contains %3]")
                      .arg(m_Name).HEXARG(this).arg(m_Queue.size()));
  }

  deallocate();

  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QxrdIntegratedDataQueue::~QxrdIntegratedDataQueue(%1) %2 end [contains %3]")
                      .arg(m_Name).HEXARG(this).arg(m_Queue.size()));
  }
}

QxrdIntegratedDataPtr QxrdIntegratedDataQueue::dequeue()
{
  QWriteLocker lock(&m_Lock);

  if (m_Queue.isEmpty()) {
    if (g_Application && qcepDebug(DEBUG_QUEUES)) {
      g_Application->printMessage(tr("QxrdIntegratedDataQueue::dequeue() = NULL from %1").arg(m_Name));
    }

    return QxrdIntegratedDataPtr(NULL);
  } else {
    QxrdIntegratedDataPtr res = m_Queue.dequeue();

    if (g_Application && qcepDebug(DEBUG_QUEUES)) {
      g_Application->printMessage(tr("QxrdIntegratedDataQueue::dequeue() = %1 from %2, leaving %3")
                        .HEXARG(res.data()).arg(m_Name).arg(m_Queue.size()));
    }

    return res;
  }
}

void QxrdIntegratedDataQueue::enqueue(QxrdIntegratedDataPtr data)
{
  QWriteLocker lock(&m_Lock);

  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QxrdIntegratedDataQueue::enqueue(%1) into %2, starting with %3")
                      .HEXARG(data.data()).arg(m_Name).arg(m_Queue.size()));
  }

  if (data) {
    m_Queue.enqueue(data);
  }
}

int QxrdIntegratedDataQueue::size() const
{
  QReadLocker lock(&m_Lock);

  if (g_Application && qcepDebug(DEBUG_QUEUES)) {
    g_Application->printMessage(tr("QxrdIntegratedDataQueue::size() = %1 for %2")
                      .arg(m_Queue.size()).arg(m_Name));
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
    if (g_Application && qcepDebug(DEBUG_QUEUES)) {
      g_Application->printMessage(tr("Deallocate %1").HEXARG(img.data()));
    }
  }
}
