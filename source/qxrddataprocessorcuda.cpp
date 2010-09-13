/******************************************************************
*
*  $Id: qxrddataprocessorcuda.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#include "qxrdmutexlocker.h"
#include "qxrddataprocessorcuda.h"

#ifdef HAVE_CUDA
#include <cuda.h>
#endif

QxrdDataProcessorCuda::QxrdDataProcessorCuda(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent)
  : QxrdDataProcessorBase(acq, allocator, saver, parent),
    SOURCE_IDENT("$Id: qxrddataprocessorcuda.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $")
{
}

void QxrdDataProcessorCuda::beginAcquisition(int isDark)
{
}

void QxrdDataProcessorCuda::idleInt16Image(QxrdInt16ImageDataPtr image)
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

void QxrdDataProcessorCuda::acquiredInt16Image(QxrdInt16ImageDataPtr image)
{
//  emit printMessage(tr("int16 image acquired"));

  m_AcquiredInt16Images.enqueue(image);

  INVOKE_CHECK(QMetaObject::invokeMethod(this, "onAcquiredInt16ImageAvailable", Qt::QueuedConnection));
}

void QxrdDataProcessorCuda::acquiredInt32Image(QxrdInt32ImageDataPtr image)
{
//  emit printMessage(tr("int32 image acquired"));

  m_AcquiredInt32Images.enqueue(image);

  INVOKE_CHECK(QMetaObject::invokeMethod(this, "onAcquiredInt32ImageAvailable", Qt::QueuedConnection));
}

int QxrdDataProcessorCuda::available()
{
#ifdef HAVE_CUDA
  int deviceCount;

  cuInit(0);

  cuDeviceGetCount(&deviceCount);

  for (int i=0; i<deviceCount; i++) {
    CUdevice dev;
    cuDeviceGet(&dev, i);
    char devName[256]="";
    cuDeviceGetName(devName, sizeof(devName), dev);
    int major=0, minor=0;

    cuDeviceComputeCapability(&major, &minor, dev);

    printf("CUDA Device %d: %s: Capability %d.%d\n", i, devName, major, minor);

    unsigned int bytes;
    cuDeviceTotalMem(&bytes, dev);

    CUdevprop devprop;
    cuDeviceGetProperties(&devprop, dev);

    printf(" Memory : %u bytes\n", bytes);
  }

  return deviceCount;
#else
  return 0;
#endif
}

/******************************************************************
*
*  $Log: qxrddataprocessorcuda.cpp,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/08/17 19:20:50  jennings
*  Added INVOKE_CHECK macro to check returned result QMetaObject::invokeMethod calls
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.9  2010/06/23 21:44:42  jennings
*  Tracked frame correction times and integration times allowing for multiple threads
*
*  Revision 1.1.2.8  2010/06/16 03:21:19  jennings
*  Added CUDA presence test
*
*  Revision 1.1.2.7  2010/06/15 20:36:25  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.6  2010/06/15 20:18:54  jennings
*  First steps to adding CUDA support
*
*  Revision 1.1.2.5  2010/06/09 19:20:29  jennings
*  Removed references to the QxrdFileSaver - all public accesses now through QxrdFileSaverThread
*
*  Revision 1.1.2.4  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.1.2.3  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.1.2.2  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.1.2.1  2010/04/21 19:48:41  jennings
*  Added QxrdAcquisitionAreaDetector, QxrdAcquisitionPilatus, QxrdDataProcessorCuda, QxrdDataProcessorThreaded
*  and QxrdPreferencesDialog - they don't do much yet, though.
*
*
*******************************************************************/

