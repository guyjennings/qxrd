/******************************************************************
*
*  $Id: qxrdintegrateddataqueue.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#include "qxrdintegrateddataqueue.h"

QxrdIntegratedDataQueue::QxrdIntegratedDataQueue(QString name) :
    m_Name(name),
    SOURCE_IDENT("$Id: qxrdintegrateddataqueue.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $")
{
}

QxrdIntegratedDataQueue::~QxrdIntegratedDataQueue()
{
  QCEP_DEBUG(DEBUG_QUEUES,
             printf("QxrdIntegratedDataQueue::~QxrdIntegratedDataQueue(%s) %p begin [contains %d]\n", qPrintable(m_Name), this, m_Queue.size());
  );

  deallocate();

  QCEP_DEBUG(DEBUG_QUEUES,
             printf("QxrdIntegratedDataQueue::~QxrdIntegratedDataQueue(%s) %p end [contains %d]\n", qPrintable(m_Name), this, m_Queue.size());
  );
}

QxrdIntegratedDataPtr QxrdIntegratedDataQueue::dequeue()
{
  QWriteLocker lock(&m_Lock);

  if (m_Queue.isEmpty()) {
    QCEP_DEBUG(DEBUG_QUEUES,
               printf("QxrdIntegratedDataQueue::dequeue() = NULL from %s\n", qPrintable(m_Name));
    );

    return QxrdIntegratedDataPtr(NULL);
  } else {
    QxrdIntegratedDataPtr res = m_Queue.dequeue();

    QCEP_DEBUG(DEBUG_QUEUES,
               printf("QxrdIntegratedDataQueue::dequeue() = %p from %s, leaving %d\n", res.data(), qPrintable(m_Name), m_Queue.size());
    );

    return res;
  }
}

void QxrdIntegratedDataQueue::enqueue(QxrdIntegratedDataPtr data)
{
  QWriteLocker lock(&m_Lock);

  QCEP_DEBUG(DEBUG_QUEUES,
    printf("QxrdIntegratedDataQueue::enqueue(%p) into %s, starting with %d\n", data.data(), qPrintable(m_Name), m_Queue.size());
  );

  if (data) {
    m_Queue.enqueue(data);
  }
}

int QxrdIntegratedDataQueue::size() const
{
  QReadLocker lock(&m_Lock);

  QCEP_DEBUG(DEBUG_QUEUES,
    printf("QxrdIntegratedDataQueue::size() = %d for %s\n", m_Queue.size(), qPrintable(m_Name));
  );

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
    QCEP_DEBUG(DEBUG_QUEUES,
               printf("Deallocate %p\n", img.data());
    );
  }
}

/******************************************************************
*
*  $Log: qxrdintegrateddataqueue.cpp,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.1  2010/06/11 21:20:40  jennings
*  Added QxrdSharedPointer, QxrdIntegratedData and QxrdIntegratedDataQueue
*
*
*******************************************************************/
