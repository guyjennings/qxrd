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
};

class QxrdImageDataObjectCounter {
public:
  QxrdImageDataObjectCounter();
  ~QxrdImageDataObjectCounter();

  int value();
};

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
