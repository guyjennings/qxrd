#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrdapplication.h"

template <typename T>
QxrdImageData<T>::QxrdImageData(QxrdAllocatorInterface *allocator, int width, int height, T def)
  : QcepImageData<T>(width, height, def),
    m_ObjectCounter(allocator),
    m_Mask(NULL),
    m_Overflow(NULL)
{

  if (qcepDebug(DEBUG_QUEUES + DEBUG_IMAGES)) {
    int count = m_ObjectCounter.value();

    g_Application->printMessage(QObject::tr("QxrdImageData<T>::QxrdImageData(%1,%2,%3) %4[%5] thr%6")
                                .HEXARG(allocator).arg(width).arg(height).HEXARG(this).arg(count).HEXARG(QThread::currentThread()));
  }

  m_ObjectCounter.allocate(sizeof(T), width, height);
}

template <typename T>
QxrdImageData<T>::~QxrdImageData()
{
  if (qcepDebug(DEBUG_QUEUES + DEBUG_IMAGES)) {
    int count = m_ObjectCounter.value();

    g_Application->printMessage(QObject::tr("QxrdImageData<T>::~QxrdImageData %1[%2], thr%3, cthr%4 titl:%5")
                                .HEXARG(this).arg(count).HEXARG(QThread::currentThread()).HEXARG(this->thread()).arg(this->get_Title()));
  }
}


template <typename T>
QString QxrdImageData<T>::rawFileName()
{
  QFileInfo info(QcepImageData<T>::get_FileName());

  QString name = info.dir().filePath(info.completeBaseName()+".raw.tif");

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

template <typename T>
template <typename T2>
void QxrdImageData<T>::copyFrom(QSharedPointer< QxrdImageData<T2> > img)
{
  if (img) {
    int ncols = img -> get_Width();
    int nrows = img -> get_Height();
    int npix = ncols*nrows;

    this -> resize(ncols, nrows);

    copyPropertiesFrom(img);

    T2 *srcp  = img -> data();
    T  *destp = this -> data();

    for (int i=0; i<npix; i++) {
      *destp++ = *srcp++;
    }
  }
}

template <typename T>
template <typename T2>
void QxrdImageData<T>::accumulateImage(QSharedPointer< QxrdImageData<T2> > image)
{
  if (image) {
    int ncols = this -> get_Width();
    int nrows = this -> get_Height();

    this->prop_SummedExposures()->incValue(image->get_SummedExposures());

    if (ncols == image->get_Width() && nrows == image->get_Height()) {
      int npix = ncols*nrows;

      T *srcp = this -> data();
      T2 *destp = image -> data();

      for (int i=0; i<npix; i++) {
        *srcp++ += *destp++;
      }
    } else {
      for (int row=0; row<nrows; row++) {
        for (int col=0; col<ncols; col++) {
          incValue(col, row, image->value(col, row));
        }
      }
    }
  }
}


template <typename T>
void QxrdImageData<T>::setMask(QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  m_Mask     = mask;
  m_Overflow = overflow;
}

template <typename T>
QxrdMaskDataPtr QxrdImageData<T>::mask() const
{
  return m_Mask;
}

template <typename T>
QxrdMaskDataPtr QxrdImageData<T>::overflow() const
{
  return m_Overflow;
}

template <typename T>
T QxrdImageData<T>::findMin() const
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  int first = true;
  T minv = 0;

  for (int row=0; row<nrows; row++) {
    for (int col=0; col<ncols; col++) {
      if (m_Mask == NULL || m_Mask->value(col,row)) {
        T val = this->value(col, row);

        if (first) {
          minv = val;
          first = false;
        } else if (val < minv){
          minv = val;
        }
      }
    }
  }

  return minv;
}

template <typename T>
T QxrdImageData<T>::findMax() const
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  int first = true;
  T maxv = 0;

  for (int row=0; row<nrows; row++) {
    for (int col=0; col<ncols; col++) {
      if (m_Mask == NULL || m_Mask->value(col,row)) {
        double val = this -> value(col, row);

        if (first) {
          maxv = val;
          first = false;
        } else if (val > maxv){
          maxv = val;
        }
      }
    }
  }

  return maxv;
}

template <typename T>
double QxrdImageData<T>::findAverage() const
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  double npix = 0;
  double sum = 0;

  for (int row=0; row<nrows; row++) {
    for (int col=0; col<ncols; col++) {
      if (m_Mask == NULL || m_Mask->value(col,row)) {
        double val = this -> value(col, row);
        npix += 1;
        sum += val;
      }
    }
  }

  if (npix <= 0) {
    return 0;
  } else {
    return sum/npix;
  }
}

template <typename T>
int QxrdImageData<T>::allocatedMemoryMB()
{
  return m_ObjectCounter.allocatedMemoryMB();
}

template <typename T>
void QxrdImageData<T>::correctBadBackgroundSubtraction(QxrdDoubleImageDataPtr dark, int nImgExposures, int nDarkExposures)
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();

  double badRatio = ((double) this->get_SummedExposures())/((double) dark->get_SummedExposures());
  double goodRatio = ((double) nImgExposures)/((double) nDarkExposures);

  for (int row=0; row<nrows; row++) {
    for (int col=0; col<ncols; col++) {
      double darkValue = dark->value(col,row);
      double imgValue  = this->value(col,row);

      imgValue += darkValue*badRatio;
      imgValue -= darkValue*goodRatio;

      this->setValue(col, row, imgValue);
    }
  }

  this->set_SummedExposures(nImgExposures);
}

template class QxrdImageData<unsigned short>;
template class QxrdImageData<short>;
template class QxrdImageData<unsigned int>;
template class QxrdImageData<int>;
template class QxrdImageData<double>;

template void QxrdImageData<double>::copyFrom<unsigned int>(QSharedPointer<QxrdImageData<unsigned int> >);
template void QxrdImageData<double>::copyFrom<unsigned short>(QSharedPointer<QxrdImageData<unsigned short> >);
template void QxrdImageData<double>::copyFrom<double>(QSharedPointer<QxrdImageData<double> >);
template void QxrdImageData<unsigned int>::copyFrom<unsigned int>(QSharedPointer<QxrdImageData<unsigned int> >);

template void QxrdImageData<double>::accumulateImage<double>(QSharedPointer<QxrdImageData<double> >);

