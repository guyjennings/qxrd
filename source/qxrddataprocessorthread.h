/******************************************************************
*
*  $Id: qxrddataprocessorthread.h,v 1.3 2010/10/21 19:44:03 jennings Exp $
*
*******************************************************************/

#ifndef QXRDDATAPROCESSORTHREAD_H
#define QXRDDATAPROCESSORTHREAD_H

#include "qcepmacros.h"

#include <QThread>
#include "qxrdforwardtypes.h"
#include "qxrddataprocessor.h"

class QxrdDataProcessorThread : public QThread
{
  Q_OBJECT;

public:
  QxrdDataProcessorThread(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver);
  ~QxrdDataProcessorThread();

  void shutdown();

  QxrdDataProcessorPtr dataProcessor() const;

  static void msleep(long unsigned int);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

protected:
  void run();

private:
  QAtomicPointer<QxrdAllocator>       m_Allocator;
  QAtomicPointer<QxrdFileSaverThread> m_FileSaverThread;
  QAtomicPointer<QxrdDataProcessor>   m_DataProcessor;
  QAtomicPointer<QxrdAcquisition>     m_Acquisition;
  HEADER_IDENT("$Id: qxrddataprocessorthread.h,v 1.3 2010/10/21 19:44:03 jennings Exp $");
};

#endif // QXRDDATAPROCESSORTHREAD_H

/******************************************************************
*
*  $Log: qxrddataprocessorthread.h,v $
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
*  Revision 1.3.4.9  2010/07/09 21:33:50  jennings
*  Tried to improve memory management by predicting memory requirements better
*  The allocators are more accurate in limiting allocated memory
*
*  Revision 1.3.4.8  2010/06/09 19:20:29  jennings
*  Removed references to the QxrdFileSaver - all public accesses now through QxrdFileSaverThread
*
*  Revision 1.3.4.7  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.3.4.6  2010/05/21 18:30:32  jennings
*  Implemented preferences dialog allowing choise of detector type, processor type and debug level
*
*  Revision 1.3.4.5  2010/04/27 01:31:26  jennings
*  Starting to get QSharedPointer working
*
*  Revision 1.3.4.4  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.3.4.3  2010/04/26 20:53:26  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.3.4.2  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.3.4.1  2010/04/21 19:50:14  jennings
*  Added option to specify processor type when creating a processor thread object
*
*  Revision 1.3  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.2  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.1  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*
*******************************************************************/
