/******************************************************************
*
*  $Id: qxrdimagequeue.cpp,v 1.8 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdimagequeue.h"
#include "qxrdimagedata.h"

QxrdImageQueue::QxrdImageQueue(QString name)
  : m_Name(name),
    m_Debug(0),
    SOURCE_IDENT("$Id: qxrdimagequeue.cpp,v 1.8 2009/06/27 22:50:32 jennings Exp $")
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

  if (m_Debug) {
    printf("QxrdImageQueue::dequeue() = %p from %s\n", res, qPrintable(m_Name));
  }

  return res;
}

void QxrdImageQueue::enqueue(QxrdImageData *data)
{
  QWriteLocker lock(&m_Lock);

  if (m_Debug) {
    printf("QxrdImageQueue::enqueue(%p) into %s\n", data, qPrintable(m_Name));
  }

  inherited::enqueue(data);
}

int QxrdImageQueue::size() const
{
  QReadLocker lock(&m_Lock);

  if (m_Debug) {
    printf("QxrdImageQueue::size() = %d for %s\n", inherited::size(), qPrintable(m_Name));
  }

  return inherited::size();
}

/******************************************************************
*
*  $Log: qxrdimagequeue.cpp,v $
*  Revision 1.8  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

