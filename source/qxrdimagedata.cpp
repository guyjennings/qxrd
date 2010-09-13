/******************************************************************
*
*  $Id: qxrdimagedata.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#include "qxrdimagedata.h"

static QAtomicInt s_ImageDataObjectCounter;

QxrdAllocatorInterface::QxrdAllocatorInterface(QObject *parent)
  : QObject(parent),
  SOURCE_IDENT("$Id: qxrdimagedata.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $")
{
}

QxrdImageDataObjectCounter::QxrdImageDataObjectCounter()
{
  s_ImageDataObjectCounter.fetchAndAddOrdered(1);
}

QxrdImageDataObjectCounter::~QxrdImageDataObjectCounter()
{
  s_ImageDataObjectCounter.fetchAndAddOrdered(-1);
}

int QxrdImageDataObjectCounter::value()
{
  return s_ImageDataObjectCounter.fetchAndAddOrdered(0);
}

/******************************************************************
*
*  $Log: qxrdimagedata.cpp,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.9.4.4  2010/06/11 21:23:30  jennings
*  When calling integrator, pass data, dark and mask images explicitly.
*
*  Revision 1.9.4.3  2010/06/07 20:17:42  jennings
*  Added more cvs log and ident stuff - removed qxrdspecserver.cpp & qxrdspecserver.h
*
*  Revision 1.9.4.2  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.9.4.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.9  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.8  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

