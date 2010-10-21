/******************************************************************
*
*  $Id: qxrddataprocessorthread.cpp,v 1.3 2010/10/21 19:44:03 jennings Exp $
*
*******************************************************************/

#include "qxrddataprocessorthread.h"

#include "qxrddataprocessorthreaded.h"

QxrdDataProcessorThread::QxrdDataProcessorThread(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver)
  : QThread(),
    m_Allocator(allocator),
    m_FileSaverThread(saver),
    m_DataProcessor(NULL),
    m_Acquisition(acq),
    SOURCE_IDENT("$Id: qxrddataprocessorthread.cpp,v 1.3 2010/10/21 19:44:03 jennings Exp $")
{
}

QxrdDataProcessorThread::~QxrdDataProcessorThread()
{
  shutdown();

  delete m_DataProcessor;
}

void QxrdDataProcessorThread::run()
{
  QxrdDataProcessorPtr p;

  p = QxrdDataProcessorPtr(new QxrdDataProcessorThreaded(m_Acquisition, m_Allocator, m_FileSaverThread, NULL));

  m_DataProcessor.fetchAndStoreOrdered(p);

  int rc = exec();

  printf("Processor thread terminated with rc %d\n", rc);
}

void QxrdDataProcessorThread::shutdown()
{
  exit();

  wait();
}

QxrdDataProcessorPtr QxrdDataProcessorThread::dataProcessor() const
{
  while (m_DataProcessor == NULL) {
    QThread::msleep(500);
  }

  return m_DataProcessor;
}

void QxrdDataProcessorThread::msleep(unsigned long t)
{
  QThread::msleep(t);
}

/******************************************************************
*
*  $Log: qxrddataprocessorthread.cpp,v $
*  Revision 1.3  2010/10/21 19:44:03  jennings
*  Adding code to display overflow pixels, removed cuda and simple processors
*
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/09/09 16:45:29  jennings
*  Always use threaded data processor
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.5.4.14  2010/07/09 21:33:50  jennings
*  Tried to improve memory management by predicting memory requirements better
*  The allocators are more accurate in limiting allocated memory
*
*  Revision 1.5.4.13  2010/06/15 20:18:54  jennings
*  First steps to adding CUDA support
*
*  Revision 1.5.4.12  2010/06/09 19:44:21  jennings
*  Print out termination messages when threads quit
*
*  Revision 1.5.4.11  2010/06/09 19:20:29  jennings
*  Removed references to the QxrdFileSaver - all public accesses now through QxrdFileSaverThread
*
*  Revision 1.5.4.10  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.5.4.9  2010/05/21 20:04:42  jennings
*  Detector and Processor type names and numbers are intialized by the corresponding thread object
*
*  Revision 1.5.4.8  2010/05/21 18:30:32  jennings
*  Implemented preferences dialog allowing choise of detector type, processor type and debug level
*
*  Revision 1.5.4.7  2010/04/27 19:37:51  jennings
*  Explicitly delete some data no longer protected by QSharedPointer s
*
*  Revision 1.5.4.6  2010/04/27 01:31:26  jennings
*  Starting to get QSharedPointer working
*
*  Revision 1.5.4.5  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.5.4.4  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.5.4.3  2010/04/26 02:43:31  jennings
*  Called msleep(500) rather than sleep(0.5)
*
*  Revision 1.5.4.2  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.5.4.1  2010/04/21 19:50:14  jennings
*  Added option to specify processor type when creating a processor thread object
*
*  Revision 1.5  2009/10/01 21:43:44  jennings
*  Delete QxrdDataProcessor object at program exit
*
*  Revision 1.4  2009/08/27 21:02:17  jennings
*  Partial implementation of lazy plotting
*
*  Revision 1.3  2009/08/09 14:38:27  jennings
*  Fixed a couple of cvs Id keyword typos
*
*  Revision 1.2  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.1  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*
*******************************************************************/
