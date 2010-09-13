/******************************************************************
*
*  $Id: qxrddataprocessorsimple.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#include "qxrdmutexlocker.h"
#include "qxrddataprocessorsimple.h"

QxrdDataProcessorSimple::QxrdDataProcessorSimple(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent)
  : QxrdDataProcessorBase(acq, allocator, saver, parent),
    SOURCE_IDENT("$Id: qxrddataprocessorsimple.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $")
{
}

void QxrdDataProcessorSimple::beginAcquisition(int isDark)
{
}

void QxrdDataProcessorSimple::idleInt16Image(QxrdInt16ImageDataPtr image)
{
//  emit printMessage(tr("int16 image idled"));
  QxrdMutexLocker lock(__FILE__, __LINE__, image->mutex());
  int height = image->get_Height();
  int width  = image->get_Width();
  int nres = image-> get_SummedExposures();
  int npixels = width*height;
  if (nres <= 0) nres = 1;
  double avgraw = 0;
  quint16 *img = image->data();

  for (int i=0; i<npixels; i++) {
    avgraw += *img++;
  }

  set_AverageRaw(avgraw/npixels/nres);
  set_Average(get_AverageRaw() - get_AverageDark());
}

void QxrdDataProcessorSimple::acquiredInt16Image(QxrdInt16ImageDataPtr image)
{
//  emit printMessage(tr("int16 image acquired"));

  m_AcquiredInt16Images.enqueue(image);

  INVOKE_CHECK(QMetaObject::invokeMethod(this, "onAcquiredInt16ImageAvailable", Qt::QueuedConnection));
}

void QxrdDataProcessorSimple::acquiredInt32Image(QxrdInt32ImageDataPtr image)
{
//  emit printMessage(tr("int32 image acquired"));

  m_AcquiredInt32Images.enqueue(image);

  INVOKE_CHECK(QMetaObject::invokeMethod(this, "onAcquiredInt32ImageAvailable", Qt::QueuedConnection));
}

/******************************************************************
*
*  $Log: qxrddataprocessorsimple.cpp,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/08/17 19:20:50  jennings
*  Added INVOKE_CHECK macro to check returned result QMetaObject::invokeMethod calls
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.5  2010/06/23 21:44:42  jennings
*  Tracked frame correction times and integration times allowing for multiple threads
*
*  Revision 1.1.2.4  2010/06/15 20:18:54  jennings
*  First steps to adding CUDA support
*
*  Revision 1.1.2.3  2010/06/09 19:20:29  jennings
*  Removed references to the QxrdFileSaver - all public accesses now through QxrdFileSaverThread
*
*  Revision 1.1.2.2  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.1.2.1  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*
*******************************************************************/

