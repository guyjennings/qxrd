/******************************************************************
*
*  $Id: qxrdacquisitionoperations.cpp,v 1.10 2009/11/17 20:42:59 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionoperations.h"
#include "qxrdimagedata.h"
#include "qxrddataprocessor.h"

QxrdAcquisitionOperations::QxrdAcquisitionOperations(QxrdDataProcessor *proc)
  : QxrdAcquisitionScripting(proc),
    m_DataProcessor(proc),
//    m_FreeImages("Free Image Pool"),
//    m_AcquiredImages("Acquired Images"),
    SOURCE_IDENT("$Id: qxrdacquisitionoperations.cpp,v 1.10 2009/11/17 20:42:59 jennings Exp $")
{
}

//QxrdImageData *QxrdAcquisitionOperations::takeNextAcquiredImage()
//{
//  QxrdMutexLocker lock(&m_Mutex);
//
//  return m_AcquiredImages.dequeue();
//}
//
//void QxrdAcquisitionOperations::newAcquiredImage(QxrdImageData *img)
//{
//  QxrdMutexLocker lock(&m_Mutex);
//
//  m_AcquiredImages.enqueue(img);
//
//  emit acquiredImageAvailable(img);
//}

/******************************************************************
*
*  $Log: qxrdacquisitionoperations.cpp,v $
*  Revision 1.10  2009/11/17 20:42:59  jennings
*  Added instrumented QxrdMutexLocker which tracks how long locks are held, and prints
*  info about any held for more than 100 msec
*
*  Revision 1.9  2009/07/20 00:32:04  jennings
*  Removed image queues for acquired and dark images - use 'connect' args instead
*
*  Revision 1.8  2009/07/17 12:41:33  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.7  2009/07/14 05:36:14  jennings
*  Moved data storage into individual channel objects, accelerated fitting (somewhat)
*
*  Revision 1.6  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.5  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.4  2009/07/09 01:15:09  jennings
*  Added some locks
*
*  Revision 1.3  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

