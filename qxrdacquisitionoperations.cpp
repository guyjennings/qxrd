/******************************************************************
*
*  $Id: qxrdacquisitionoperations.cpp,v 1.3 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionoperations.h"
#include "qxrdimagedata.h"

QxrdAcquisitionOperations::QxrdAcquisitionOperations(QxrdAcquisitionThread *thread)
  : QxrdAcquisitionScripting(thread),
    m_FreeImages("Free Image Pool"),
    m_AcquiredImages("Acquired Images"),
    SOURCE_IDENT("$Id: qxrdacquisitionoperations.cpp,v 1.3 2009/06/27 22:50:32 jennings Exp $")
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


/******************************************************************
*
*  $Log: qxrdacquisitionoperations.cpp,v $
*  Revision 1.3  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

