#include "qxrdresultserializer.h"
#include "qxrdmutexlocker.h"

QxrdResultSerializerBase::QxrdResultSerializerBase(QObject *parent) :
    QObject(parent)
{
}

QxrdResultSerializerBase::~QxrdResultSerializerBase()
{
}


template <typename T>
QxrdResultSerializer<T>::QxrdResultSerializer(QcepIntProperty *ctr, QObject *parent)
 : QxrdResultSerializerBase(parent),
   m_Counter(ctr)
{
  connect(&m_Watcher, SIGNAL(finished()), this, SIGNAL(resultAvailable()));
}

template <typename T>
void QxrdResultSerializer<T>::enqueue(QFuture<T> future)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Results.enqueue(future);

  m_Watcher.setFuture(m_Results[0]);

  if (m_Counter) {
    m_Counter->incValue(1);
  }
}

template <typename T>
T QxrdResultSerializer<T>::dequeue()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_Results.size() > 0) {
    QFuture<T> val = m_Results.dequeue();

    if (m_Results.size() > 0) {
      m_Watcher.setFuture(m_Results[0]);
    }

    if (m_Counter) {
      m_Counter->incValue(-1);
    }

    return val.result();
  } else {
    if (m_Counter) {
      m_Counter->setValue(0);
    }

    return T();
  }
}

template <typename T>
int QxrdResultSerializer<T>::count() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Results.size();
}

#include "qxrdimagedata.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdintegrateddata.h"
#include "qxrdintegrateddata-ptr.h"
#include "qxrdroidata.h"
#include "qxrdroidata-ptr.h"
#include "qxrdhistogramdata.h"
#include "qxrdhistogramdata-ptr.h"

template class QxrdResultSerializer<QxrdDoubleImageDataPtr>;
template class QxrdResultSerializer<QxrdIntegratedDataPtr>;
template class QxrdResultSerializer<QxrdROIDataPtr>;
template class QxrdResultSerializer<QxrdHistogramDataPtr>;
