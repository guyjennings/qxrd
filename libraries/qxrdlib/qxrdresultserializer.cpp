#include "qxrdresultserializer.h"
#include "qcepmutexlocker.h"
#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"
#include "qcepintegrateddata.h"
#include "qcepintegrateddata-ptr.h"
#include "qxrdhistogramdata.h"
#include "qxrdhistogramdata-ptr.h"

QxrdResultSerializerBase::QxrdResultSerializerBase(QString name, QcepIntProperty *ctr)
  : QcepObject(name),
    m_Counter(ctr),
    m_Size(this, "size", 0, "Queue Size")
{
}

QxrdResultSerializerBase::~QxrdResultSerializerBase()
{
}

template <typename T>
QxrdResultSerializer<T>::QxrdResultSerializer(QString name, QcepIntProperty *ctr)
 : QxrdResultSerializerBase(name, ctr)
{
  CONNECT_CHECK(connect(&m_Watcher, &QFutureWatcherBase::finished, this, &QxrdResultSerializerBase::resultAvailable));
}

template <typename T>
void QxrdResultSerializer<T>::enqueue(QFuture<T> future)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Results.enqueue(future);

  m_Watcher.setFuture(m_Results[0]);

  if (m_Counter) {
    m_Counter->incValue(1);
  }

  prop_Size()->incValue(1);
}

template <typename T>
T QxrdResultSerializer<T>::dequeue()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_Results.size() > 0) {
    QFuture<T> val = m_Results.dequeue();

    if (m_Results.size() > 0) {
      m_Watcher.setFuture(m_Results[0]);
    }

    if (m_Counter) {
      m_Counter->incValue(-1);
    }

    prop_Size()->incValue(-1);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Results.size();
}

QxrdDoubleSerializer::QxrdDoubleSerializer(QString name, QcepIntProperty *ctr)
  : QxrdResultSerializer<QcepDoubleImageDataPtr>(name, ctr)
{
}

QxrdIntegratedSerializer::QxrdIntegratedSerializer(QString name, QcepIntProperty *ctr)
  : QxrdResultSerializer<QcepIntegratedDataPtr>(name, ctr)
{
}

QxrdHistogramSerializer::QxrdHistogramSerializer(QString name, QcepIntProperty *ctr)
  : QxrdResultSerializer<QxrdHistogramDataPtr>(name, ctr)
{
}

template class QxrdResultSerializer<QcepDoubleImageDataPtr>;
template class QxrdResultSerializer<QcepIntegratedDataPtr>;
template class QxrdResultSerializer<QxrdHistogramDataPtr>;
