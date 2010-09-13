/******************************************************************
*
*  $Id: qxrdacquisitionthread.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include "qcepmacros.h"

#include <QThread>
#include <QVector>
#include <QVariant>
#include <QMutex>
#include <QWaitCondition>

#include "qxrdforwardtypes.h"
#include "qxrddataprocessor.h"

class QxrdAcquisitionThread : public QThread
{
  Q_OBJECT;

 public:
  QxrdAcquisitionThread(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator, int detectorType);
  ~QxrdAcquisitionThread();

  void shutdown();
  void msleep(int msec);

  static QStringList detectorTypeNames();
  static int detectorType();

public slots:
  void doAcquire();
  void cancel();

  void doAcquireDark();
  void cancelDark();

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public:
  QxrdAcquisitionPtr acquisition() const;

public:
  void sleep(double time);

protected:
  void run();

private:
  int                    m_Debug;
  QAtomicPointer<QxrdAllocator>       m_Allocator;
  QAtomicPointer<QxrdAcquisition>     m_Acquisition;
  QAtomicPointer<QxrdDataProcessor>   m_Processor;
  int                    m_DetectorType;

  HEADER_IDENT("$Id: qxrdacquisitionthread.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdacquisitionthread.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.30.4.11  2010/05/24 21:02:37  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.30.4.10  2010/05/21 18:30:32  jennings
*  Implemented preferences dialog allowing choise of detector type, processor type and debug level
*
*  Revision 1.30.4.9  2010/04/27 01:31:26  jennings
*  Starting to get QSharedPointer working
*
*  Revision 1.30.4.8  2010/04/26 23:46:12  jennings
*  *** empty log message ***
*
*  Revision 1.30.4.7  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.30.4.6  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.30.4.5  2010/04/21 19:49:36  jennings
*  Added option to specify detector type when creating an acquisition thread object
*
*  Revision 1.30.4.4  2010/04/21 16:58:27  jennings
*  Re-wrote QxrdAcquisitionThread startup code so that the acquisition object is constructed in the thread's run method
*  so it automatically ends up with the correct thread affinity
*
*  Revision 1.30.4.3  2010/04/20 21:36:01  jennings
*  Added m_Processor field to QxrdAcquisitionThread
*
*  Revision 1.30.4.2  2010/04/19 21:48:27  jennings
*  More rearrangement
*
*  Revision 1.30.4.1  2010/04/18 17:20:48  jennings
*  Further updates
*
*  Revision 1.30  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.29  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.28  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.27  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

