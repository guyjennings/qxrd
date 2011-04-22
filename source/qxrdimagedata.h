#ifndef QXRDIMAGEDATA_H
#define QXRDIMAGEDATA_H

#include "qcepmacros.h"

#include <QDir>
#include <QFileInfo>
#include <QAtomicInt>
#include <QSharedPointer>
#include <QThread>
#include <typeinfo>
#include "qcepimagedata.h"
#include "qxrdallocatorinterface.h"
#include "qxrdimagedataobjectcounter.h"

template <typename T> class QxrdImageData;

typedef QxrdImageData<quint16> QxrdInt16ImageData;
typedef QxrdImageData<quint32> QxrdInt32ImageData;
typedef QxrdImageData<double>  QxrdDoubleImageData;
class QxrdMaskData;

typedef QSharedPointer<QxrdInt16ImageData> QxrdInt16ImageDataPtr;
typedef QSharedPointer<QxrdInt32ImageData> QxrdInt32ImageDataPtr;
typedef QSharedPointer<QxrdDoubleImageData> QxrdDoubleImageDataPtr;
typedef QSharedPointer<QxrdMaskData> QxrdMaskDataPtr;

template <typename T>
class QxrdImageData : public QcepImageData<T>
{
//  Q_OBJECT;
//
public:
  QxrdImageData(QxrdAllocatorInterface *allocator, int width=0, int height=0, T def=0);
  ~QxrdImageData();

  QString rawFileName();

  template <typename T2>
  void copyImage(QSharedPointer< QxrdImageData<T2> > dest);

  template <typename T2>
      void copyFrom(const QSharedPointer< QxrdImageData<T2> > img);

  template <typename T2>
  void accumulateImage(QSharedPointer< QxrdImageData<T2> > image);

  void setMask(QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdMaskDataPtr mask() const;
  QxrdMaskDataPtr overflow() const;

  T findMin() const;
  T findMax() const;
  double findAverage() const;

  int allocatedMemoryMB();

  void correctBadBackgroundSubtraction(QxrdDoubleImageDataPtr dark, int nImgExposures, int nDarkExposures);

protected:
  QxrdImageDataObjectCounter m_ObjectCounter; /* global counter to track allocation of QxrdImageData objects */
  QxrdMaskDataPtr            m_Mask;
  QxrdMaskDataPtr            m_Overflow;
};

#endif
