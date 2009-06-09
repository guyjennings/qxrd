#include "qxrdacquisitionoperations.h"
#include "qxrdimagedata.h"

QxrdAcquisitionOperations::QxrdAcquisitionOperations(QxrdAcquisitionThread *thread)
  : inherited(thread),
    m_FreeImages("Free Image Pool"),
    m_AcquiredImages("Acquired Images")
{
}

QxrdImageData *QxrdAcquisitionOperations::takeNextFreeImage()
{
  if (m_FreeImages.size() == 0) {
//    printf("Allocate new image\n");
    return new QxrdImageData(2048, 2048);
  } else {
    return m_FreeImages.dequeue();
  }
}

QxrdImageData *QxrdAcquisitionOperations::takeNextAcquiredImage()
{
  return m_AcquiredImages.dequeue();
}

void QxrdAcquisitionOperations::newAcquiredImage(QxrdImageData *img)
{
  m_AcquiredImages.enqueue(img);

  emit acquiredImageAvailable();
}

void QxrdAcquisitionOperations::returnImageToPool(QxrdImageData *img)
{
  m_FreeImages.enqueue(img);
}

