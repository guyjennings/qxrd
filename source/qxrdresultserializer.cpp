#include "qxrdresultserializer.h"

QxrdResultSerializerBase::QxrdResultSerializerBase(QObject *parent) :
    QObject(parent)
{
}

QxrdResultSerializerBase::~QxrdResultSerializerBase()
{
}


template <typename T>
QxrdResultSerializer<T>::QxrdResultSerializer(QObject *parent)
 : QxrdResultSerializerBase(parent)
{
  connect(&m_Watcher, SIGNAL(finished()), this, SIGNAL(resultAvailable()));
}

template <typename T>
void QxrdResultSerializer<T>::enqueue(QFuture<T> future)
{
  QMutexLocker lock(&m_Mutex);

  m_Results.enqueue(future);

  m_Watcher.setFuture(m_Results[0]);
}

template <typename T>
T QxrdResultSerializer<T>::dequeue()
{
  QMutexLocker lock(&m_Mutex);

  if (m_Results.size() > 0) {
    QFuture<T> val = m_Results.dequeue();

    if (m_Results.size() > 0) {
      m_Watcher.setFuture(m_Results[0]);
    }

    return val.result();
  } else {
    return T();
  }
}

#include "qxrdimagedata.h"
#include "qxrdintegrateddata.h"
#include "qxrdroidata.h"
#include "qxrdhistogramdata.h"

template class QxrdResultSerializer<QxrdDoubleImageDataPtr>;
template class QxrdResultSerializer<QxrdIntegratedDataPtr>;
template class QxrdResultSerializer<QxrdROIDataPtr>;
template class QxrdResultSerializer<QxrdHistogramDataPtr>;
