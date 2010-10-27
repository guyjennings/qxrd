/******************************************************************
*
*  $Id: qxrdallocatorthread.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDALLOCATORTHREAD_H
#define QXRDALLOCATORTHREAD_H

#include "qcepmacros.h"

#include <QThread>
#include "qxrdforwardtypes.h"
#include "qxrdallocator.h"

class QxrdAllocatorThread : public QThread
{
  Q_OBJECT;

public:
  QxrdAllocatorThread();
  ~QxrdAllocatorThread();

  void shutdown();

  QxrdAllocatorPtr allocator() const;

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

protected:
  void run();

private:
  QAtomicPointer<QxrdAllocator>     m_Allocator;
//  QAtomicPointer<QxrdAcquisition>   m_Acquisition;
  HEADER_IDENT("$Id: qxrdallocatorthread.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif // QXRDALLOCATORTHREAD_H

/******************************************************************
*
*  $Log: qxrdallocatorthread.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
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
*
*******************************************************************/
