/******************************************************************
*
*  $Id: qxrddataprocessorsimple.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDDATAPROCESSORSIMPLE_H
#define QXRDDATAPROCESSORSIMPLE_H

#include "qcepmacros.h"
#include "qxrddataprocessorbase.h"

class QxrdDataProcessorSimple : public QxrdDataProcessorBase
{
  Q_OBJECT;
public:
  QxrdDataProcessorSimple(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent=0);

public:
  void beginAcquisition(int isDark);
  void idleInt16Image(QxrdInt16ImageDataPtr image);
  void acquiredInt16Image(QxrdInt16ImageDataPtr image);
  void acquiredInt32Image(QxrdInt32ImageDataPtr image);

private:
  HEADER_IDENT("$Id: qxrddataprocessorsimple.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif // QXRDDATAPROCESSORSIMPLE_H

/******************************************************************
*
*  $Log: qxrddataprocessorsimple.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
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

