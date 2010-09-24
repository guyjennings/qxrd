/******************************************************************
*
*  $Id: qxrdacquisitionthread.cpp,v 1.4 2010/09/24 22:29:37 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionthread.h"

#ifdef HAVE_PERKIN_ELMER
#include "qxrdacquisitionperkinelmer.h"
#endif

#include "qxrdacquisitionpilatus.h"
#include "qxrdacquisitionareadetector.h"
#include "qxrdacquisitionsimulated.h"
#include "qxrddataprocessor.h"
#include <QFuture>
#include <QVariant>
#include <QMetaObject>

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

static int g_DetectorType = -1;

#ifdef HAVE_PERKIN_ELMER
static int g_PEAvailable = false;
#endif

QxrdAcquisitionThread::QxrdAcquisitionThread(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator, int detectorType)
  : QThread(),
    m_Debug(true),
    m_Allocator(NULL),
    m_Acquisition(NULL),
    m_Processor(NULL),
    m_DetectorType(detectorType),
    SOURCE_IDENT("$Id: qxrdacquisitionthread.cpp,v 1.4 2010/09/24 22:29:37 jennings Exp $")
{
  m_Allocator.fetchAndStoreOrdered(allocator);
  m_Processor.fetchAndStoreOrdered(proc);

#ifdef HAVE_PERKIN_ELMER
  HINSTANCE xisllib;

  xisllib = LoadLibrary(L"XISL.dll");

  if (xisllib == NULL) {
    printf("XISL library is not available - cannot use PE detector\n");
  } else {
    g_PEAvailable = true;
  }
#endif
}

QxrdAcquisitionThread::~QxrdAcquisitionThread()
{
  shutdown();

  delete m_Acquisition;
}

void QxrdAcquisitionThread::run()
{
  QxrdAcquisitionPtr p;

  switch(m_DetectorType) {
  case 0:
  default:
    p = QxrdAcquisitionPtr(new QxrdAcquisitionSimulated(m_Processor, m_Allocator));
    g_DetectorType = 0;
    break;

#ifdef HAVE_PERKIN_ELMER
  case 1:
    if (g_PEAvailable) {
      p = QxrdAcquisitionPtr(new QxrdAcquisitionPerkinElmer(m_Processor, m_Allocator));
      g_DetectorType = 1;
    } else {
      p = QxrdAcquisitionPtr(new QxrdAcquisitionSimulated(m_Processor, m_Allocator));
      g_DetectorType = 0;
    }
    break;
#endif

#ifdef HAVE_PILATUS
  case 2:
    p = QxrdAcquisitionPtr(new QxrdAcquisitionPilatus(m_Processor, m_Allocator));
    g_DetectorType = 2;
    break;
#endif

#ifdef HAVE_AREADETECTOR
  case 3:
    p = QxrdAcquisitionPtr(new QxrdAcquisitionAreaDetector(m_Processor, m_Allocator));
    g_DetectorType = 3;
    break;
#endif
  }


//  m_Processor -> setAcquisition(m_Acquisition);

  m_Acquisition.fetchAndStoreOrdered(p);


  int rc = exec();

  printf("Acquisition thread terminated with rc %d\n", rc);
}

void QxrdAcquisitionThread::initialize()
{
  if (m_Acquisition) {
    INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition,"initialize",Qt::BlockingQueuedConnection));
    m_Acquisition->set_DetectorType(g_DetectorType);
    m_Acquisition->set_DetectorTypeName(detectorTypeNames()[g_DetectorType]);
  }
}

void QxrdAcquisitionThread::shutdown()
{
  exit();

  wait();
}

void QxrdAcquisitionThread::doAcquire()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition, "acquire", Qt::QueuedConnection));
}

void QxrdAcquisitionThread::doAcquireDark()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition, "acquireDark", Qt::QueuedConnection));
}

void QxrdAcquisitionThread::msleep(int msec)
{
  QThread::msleep(msec);
}

void QxrdAcquisitionThread::cancel()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition, "cancel", Qt::QueuedConnection));
}

void QxrdAcquisitionThread::cancelDark()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition, "cancelDark", Qt::QueuedConnection));
}

QxrdAcquisitionPtr QxrdAcquisitionThread::acquisition() const
{
  while (m_Acquisition == NULL) {
    QThread::msleep(500);
  }

  return m_Acquisition;
}

void QxrdAcquisitionThread::sleep(double time)
{
  QThread::usleep((int)(time*1e6));
}

QStringList QxrdAcquisitionThread::detectorTypeNames()
{
  QStringList res;

  res << "Simulated Detector"
      << "Perkin Elmer Flat Panel"
      << "Pilatus"
      << "EPICS Area Detector";

  return res;
}

int QxrdAcquisitionThread::detectorType()
{
  return g_DetectorType;
}

/******************************************************************
*
*  $Log: qxrdacquisitionthread.cpp,v $
*  Revision 1.4  2010/09/24 22:29:37  jennings
*  Work on NSIS installer
*  Fixed startup problem on debug builds when calling QxrdAcquisitionThread->initialize()
*
*  Revision 1.3  2010/09/23 19:57:32  jennings
*  Modified plugins for perkin elmer - now works in 64 bit mode
*
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/08/17 19:20:50  jennings
*  Added INVOKE_CHECK macro to check returned result QMetaObject::invokeMethod calls
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.33.2.23  2010/07/21 19:38:04  jennings
*  Made Pilatus and AreaDetector code conditional on HAVE_PILATUS and HAVE_AREADETECTOR
*
*  Revision 1.33.2.22  2010/06/09 19:44:21  jennings
*  Print out termination messages when threads quit
*
*  Revision 1.33.2.21  2010/05/26 19:44:02  jennings
*  Adding support for weak linking of XISL dll
*
*  Revision 1.33.2.20  2010/05/26 19:20:43  jennings
*  Modified interface to XISL.dll so that it is accessed via 'LoadLibrary' and
*  'GetProcAddress' rather than being explicitly linked.  Should allow
*  qxrd to run if the PE libraries are not present.
*
*  Revision 1.33.2.19  2010/05/24 21:02:37  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.33.2.18  2010/05/21 20:04:42  jennings
*  Detector and Processor type names and numbers are intialized by the corresponding thread object
*
*  Revision 1.33.2.17  2010/05/21 18:30:32  jennings
*  Implemented preferences dialog allowing choise of detector type, processor type and debug level
*
*  Revision 1.33.2.16  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.33.2.15  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.33.2.14  2010/04/27 19:37:51  jennings
*  Explicitly delete some data no longer protected by QSharedPointer s
*
*  Revision 1.33.2.13  2010/04/27 01:31:26  jennings
*  Starting to get QSharedPointer working
*
*  Revision 1.33.2.12  2010/04/26 23:46:12  jennings
*  *** empty log message ***
*
*  Revision 1.33.2.11  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.33.2.10  2010/04/26 02:43:31  jennings
*  Called msleep(500) rather than sleep(0.5)
*
*  Revision 1.33.2.9  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.33.2.8  2010/04/24 19:38:40  jennings
*  Added HAVE_PERKIN_ELMER config symbol to conditionalise support for
*  Perkin Elmer detector.
*
*  Revision 1.33.2.7  2010/04/21 19:49:36  jennings
*  Added option to specify detector type when creating an acquisition thread object
*
*  Revision 1.33.2.6  2010/04/21 16:58:27  jennings
*  Re-wrote QxrdAcquisitionThread startup code so that the acquisition object is constructed in the thread's run method
*  so it automatically ends up with the correct thread affinity
*
*  Revision 1.33.2.5  2010/04/20 21:36:01  jennings
*  Added m_Processor field to QxrdAcquisitionThread
*
*  Revision 1.33.2.4  2010/04/19 21:48:27  jennings
*  More rearrangement
*
*  Revision 1.33.2.3  2010/04/17 05:13:58  jennings
*  Partially working
*
*  Revision 1.33.2.2  2010/04/15 19:33:50  jennings
*  Create simulated acquisition object, for now
*
*  Revision 1.33.2.1  2010/04/12 21:21:46  jennings
*  Version 0.4.0, some rearrangement of the Acquisition classes
*
*  Revision 1.33  2009/11/17 20:42:59  jennings
*  Added instrumented QxrdMutexLocker which tracks how long locks are held, and prints
*  info about any held for more than 100 msec
*
*  Revision 1.32  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.31  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.30  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.29  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

