#include "qxrdimagequeue.h"
#include "qxrdimagedata.h"

QxrdImageQueue::QxrdImageQueue(QString name)
  : m_Name(name)
{
}

QxrdImageQueue::~QxrdImageQueue()
{
  QWriteLocker lock(&m_Lock);

  while (!isEmpty()) {
    QxrdImageData *img = inherited::dequeue();

    delete img;
  }
}

QxrdImageData* QxrdImageQueue::dequeue()
{
  QWriteLocker lock(&m_Lock);
  QxrdImageData* res = inherited::dequeue();

  printf("QxrdImageQueue::dequeue() = %p from %s\n", res, qPrintable(m_Name));

  return res;
}

void QxrdImageQueue::enqueue(QxrdImageData *data)
{
  QWriteLocker lock(&m_Lock);

  printf("QxrdImageQueue::enqueue(%p) into %s\n", data, qPrintable(m_Name));

  inherited::enqueue(data);
}

int QxrdImageQueue::size() const
{
  QReadLocker lock(&m_Lock);

  return inherited::size();
}
