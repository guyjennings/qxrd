/******************************************************************
*
*  $Id: qxrdimagedata.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDIMAGEDATA_H
#define QXRDIMAGEDATA_H

#include "qcepmacros.h"

#include <QDir>
#include <QFileInfo>
#include <QAtomicInt>
#include <QSharedPointer>
#include "qcepimagedata.h"

class QxrdAllocatorInterface : public QObject
{
  Q_OBJECT;
public:
  QxrdAllocatorInterface(QObject *parent=0);

  virtual void allocate(int sz, int width, int height) = 0;
  virtual void deallocate(int sz, int width, int height) = 0;

  HEADER_IDENT("$Id: qxrdimagedata.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

class QxrdImageDataObjectCounter {
public:
  QxrdImageDataObjectCounter();
  ~QxrdImageDataObjectCounter();

  int value();
};

//template <typename T>
//class QxrdImageQueue;

template <typename T>
class QxrdImageData : public QcepImageData<T>
{
//  Q_OBJECT;
//
public:
  QxrdImageData(QxrdAllocatorInterface *allocator, int width=0, int height=0);
  ~QxrdImageData();

  QString rawFileName();

  template <typename T2>
  void copyImage(QSharedPointer< QxrdImageData<T2> > dest);

private:
  QxrdImageDataObjectCounter m_ImageDataObjectCounter; /* global counter to track allocation of QxrdImageData objects */
  QxrdAllocatorInterface    *m_Allocator;
};

typedef QxrdImageData<quint16> QxrdInt16ImageData;
typedef QxrdImageData<quint32> QxrdInt32ImageData;
//typedef QxrdImageData<double>  QxrdDoubleImageData;

typedef QSharedPointer<QxrdInt16ImageData> QxrdInt16ImageDataPtr;
typedef QSharedPointer<QxrdInt32ImageData> QxrdInt32ImageDataPtr;

template <typename T>
QxrdImageData<T>::QxrdImageData(QxrdAllocatorInterface *allocator, /*QxrdImageQueue< QxrdImageData<T> > *pool,*/ int width, int height)
  : QcepImageData<T>(width, height),
    m_Allocator(allocator)
{
  int count = m_ImageDataObjectCounter.value();

  QCEP_DEBUG(DEBUG_QUEUES + DEBUG_IMAGES,
             printf("QxrdImageData<T>::QxrdImageData(%p,%d,%d) %p[%d]\n", allocator, width, height, this, count);
  );

  if (m_Allocator) {
    m_Allocator -> allocate(sizeof(T), width, height);
  } else {
    printf("QxrdImageData<T>::QxrdImageData called with NULL allocator\n");
  }
}

template <typename T>
QxrdImageData<T>::~QxrdImageData()
{
  int count = m_ImageDataObjectCounter.value();

  QCEP_DEBUG(DEBUG_QUEUES + DEBUG_IMAGES,
             printf("QxrdImageData<T>::~QxrdImageData %p[%d]\n", this, count);
  );

  if (m_Allocator) {
    m_Allocator -> deallocate(sizeof(T), QcepImageDataBase::get_Width(), QcepImageDataBase::get_Height());
  }
}


template <typename T>
QString QxrdImageData<T>::rawFileName()
{
  QFileInfo info(QcepImageData<T>::get_FileName());

  QString name = info.dir().filePath(
      info.completeBaseName()+".raw.tif");

  return name;
}

template <typename T>
template <typename T2>
void QxrdImageData<T>::copyImage(QSharedPointer< QxrdImageData<T2> > dest)
{
  if (dest) {
    int ncols = this -> get_Width();
    int nrows = this -> get_Height();
    int npix = ncols*nrows;

    dest -> resize(ncols, nrows);

    this -> copyProperties(dest.data());

    T *srcp = this -> data();
    T2 *destp = dest -> data();

    for (int i=0; i<npix; i++) {
      *destp++ = *srcp++;
    }
  }
}

#endif

/******************************************************************
*
*  $Log: qxrdimagedata.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.12.4.13  2010/06/17 16:02:36  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*  Revision 1.12.4.12  2010/06/14 20:57:21  jennings
*  *** empty log message ***
*
*  Revision 1.12.4.11  2010/06/14 20:18:00  jennings
*  Implemented plotting and saving of integrated data - still need some kind of 'serializer' to maintain order of saving and plotting
*
*  Revision 1.12.4.10  2010/06/07 20:17:42  jennings
*  Added more cvs log and ident stuff - removed qxrdspecserver.cpp & qxrdspecserver.h
*
*  Revision 1.12.4.9  2010/05/24 21:02:39  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.12.4.8  2010/05/17 15:59:53  jennings
*  Changed debugging output to use QCEP_DEBUG macro
*
*  Revision 1.12.4.7  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.12.4.6  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.12.4.5  2010/04/27 19:38:57  jennings
*  *** empty log message ***
*
*  Revision 1.12.4.4  2010/04/27 15:19:42  jennings
*  More changes to help using QSharedPointer
*
*  Revision 1.12.4.3  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.12.4.2  2010/04/25 03:38:40  jennings
*  Added scrolling area to acquire dialog.  Instrument image queues.
*
*  Revision 1.12.4.1  2010/04/12 21:34:46  jennings
*  Added atomic int ref count to QxrdImageData
*
*  Revision 1.12  2009/09/12 14:44:20  jennings
*  Moved lock to base class, made into mutex
*
*  Revision 1.11  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.10  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

