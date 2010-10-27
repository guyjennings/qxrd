/******************************************************************
*
*  $Id: qxrdallocatorthread.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#include "qxrdallocatorthread.h"

#include "qxrdallocator.h"
#include "qxrdacquisition.h"

QxrdAllocatorThread::QxrdAllocatorThread(/*QxrdAcquisitionPtr acq*/)
  : QThread(),
    m_Allocator(NULL),
//    m_Acquisition(NULL),
    SOURCE_IDENT("$Id: qxrdallocatorthread.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $")
{
//  m_Acquisition.fetchAndStoreOrdered(acq);
}

QxrdAllocatorThread::~QxrdAllocatorThread()
{
  shutdown();

  delete m_Allocator;
}

void QxrdAllocatorThread::run()
{
  m_Allocator.fetchAndStoreOrdered(new QxrdAllocator(/*m_Acquisition*/));

  connect(m_Allocator, SIGNAL(printMessage(QString)), this, SIGNAL(printMessage(QString)));
  connect(m_Allocator, SIGNAL(statusMessage(QString)), this, SIGNAL(statusMessage(QString)));
  connect(m_Allocator, SIGNAL(criticalMessage(QString)), this, SIGNAL(criticalMessage(QString)));

  int rc = exec();

  printf("Allocator thread terminated with rc %d\n", rc);
}

void QxrdAllocatorThread::shutdown()
{
  exit();

  wait();
}

QxrdAllocatorPtr QxrdAllocatorThread::allocator() const
{
  while (m_Allocator == NULL) {
    QThread::msleep(500);
  }

  return m_Allocator;
}

/******************************************************************
*
*  $Log: qxrdallocatorthread.cpp,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.4  2010/06/23 21:49:35  jennings
*  Made allocator strategy more sophisticated, called it more often
*
*  Revision 1.1.2.3  2010/06/09 19:44:21  jennings
*  Print out termination messages when threads quit
*
*  Revision 1.1.2.2  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.1.2.1  2010/05/20 20:15:54  jennings
*  Initial files for QxrdAllocator and QxrdFileSaver threads
*
*
*******************************************************************/
