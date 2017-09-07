#include "qxrddebug.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrdapplication.h"
#include <QDir>
#include <typeinfo>
#include <QThread>
#include "qcepmutexlocker.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrator.h"

template <typename T>
QxrdImageData<T>::QxrdImageData(QxrdSettingsSaverWPtr saver, QxrdAllocatorWPtr allocator, int typ, int width, int height, T def)
  : QcepImageData<T>(saver, width, height, def),
    m_ObjectCounter(allocator, typ),
    m_Mask(NULL),
    m_Overflow(NULL)
{
  if (g_Application && (qcepDebug(DEBUG_QUEUES) || qcepDebug(DEBUG_IMAGES))) {
    int count = m_ObjectCounter.value();

    g_Application->printMessage(QObject::tr("QxrdImageData<%1>::QxrdImageData(%2,%3,%4) %5[%6] thr%7")
                      .arg(typeid(T).name())
                      .HEXARG(allocator.data())
                      .arg(width).arg(height)
                      .HEXARG(this).arg(count).HEXARG(QThread::currentThread()));
  }

  m_ObjectCounter.allocate(sizeof(T), width, height);
}

template <typename T>
QxrdImageData<T>::~QxrdImageData()
{
  if (g_Application && (qcepDebug(DEBUG_QUEUES) || qcepDebug(DEBUG_IMAGES))) {
    int count = m_ObjectCounter.value();

    g_Application->printMessage(QObject::tr("QxrdImageData<%1>::~QxrdImageData %2[%3], thr%4, cthr%5 titl:%6")
                      .arg(typeid(T).name())
                      .HEXARG(this).arg(count).HEXARG(QThread::currentThread()).HEXARG(this->thread()).arg(this->get_Title()));
  }
}


template <typename T>
void QxrdImageData<T>::saveMetaData(QxrdExperimentWPtr expt)
{
  saveMetaData(QcepImageDataBase::get_FileName(), expt);
}

template <typename T>
void QxrdImageData<T>::saveMetaData(QString name, QxrdExperimentWPtr expt)
{
//  g_Application->printMessage(tr("QxrdImageData::saveMetaData for file %1").arg(name));

  QTime tic;
  tic.start();

//  printf("type 266 = %s\n", QMetaType::typeName(266));

  {
    QcepMutexLocker lock(__FILE__, __LINE__, QcepImageDataBase::mutex());

    QSettings settings(name+".metadata", QSettings::IniFormat);

    QcepProperty::writeSettings(this, &QcepImageDataBase::staticMetaObject/*metaObject()*/, "metadata", &settings, true);

    settings.beginWriteArray("normalization");
    QcepDoubleList norm = QcepImageDataBase::get_Normalization();

    for (int i=0; i<norm.length(); i++) {
      settings.setArrayIndex(i);
      settings.setValue("val",norm[i]);
    }
    settings.endArray();

    QxrdExperimentPtr exper(expt);

    if (exper) {
      QxrdCenterFinderPtr cf = exper->centerFinder();

      if (cf) {
        cf->writeSettings(&settings, "centerfinder");
      }

      QxrdIntegratorPtr integ = exper->integrator();

      if (integ) {
        integ->writeSettings(&settings, "integrator");
      }
    }
  }
//
//  printf("QcepImageDataBase::saveMetaData for file %s took %d msec\n",  qPrintable(name), tic.elapsed());
}

template <typename T>
void QxrdImageData<T>::loadMetaData(QxrdExperimentWPtr expt)
{
  loadMetaData(QcepImageDataBase::get_FileName(), expt);
}

template <typename T>
void QxrdImageData<T>::loadMetaData(QString name, QxrdExperimentWPtr expt)
{
//  g_Application->printMessage(tr("QxrdImageData::loadMetaData for file %1").arg(name));

  QTime tic;
  tic.start();

  {
    QcepMutexLocker lock(__FILE__, __LINE__, QcepImageDataBase::mutex());

    QSettings settings(name+".metadata", QSettings::IniFormat);

    QcepProperty::readSettings(this, &settings, "metadata");

    int n = settings.beginReadArray("normalization");
    QcepDoubleList norm;

    for (int i=0; i<n; i++) {
      settings.setArrayIndex(i);
      norm.append(settings.value("val").toDouble());
    }
    settings.endArray();

    QcepImageDataBase::set_Normalization(norm);

    QxrdExperimentPtr exper(expt);

    if (exper) {
      QxrdCenterFinderPtr cf = exper->centerFinder();

      if (cf) {
        cf->readSettings(&settings, "centerfinder");
      }

      QxrdIntegratorPtr integ = exper->integrator();

      if (integ) {
        integ->readSettings(&settings, "integrator");
      }
    }
  }
//
//  printf("QcepImageDataBase::loadMetaData for file %s took %d msec\n",  qPrintable(name), tic.elapsed());
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

    this -> copyPropertiesFrom(img);

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
          this -> addValue(col, row, image->value(col, row));
        }
      }

      QcepDoubleList norm  = this->get_Normalization();
      QcepDoubleList extra = this->get_ExtraInputs();

      QcepDoubleList norm2 = image->get_Normalization();
      QcepDoubleList extra2= image->get_ExtraInputs();

      for (int i=0; i<norm.count(); i++) {
        norm[i] += norm2.value(i);
      }

      for (int i=0; i<extra.count(); i++) {
        extra[i] += extra2.value(i);
      }

      this->set_Normalization(norm);
      this->set_ExtraInputs(extra);
    }
  }
}

template <typename T>
template <typename T2>
void QxrdImageData<T>::add(QSharedPointer< QxrdImageData<T2> > image)
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
          this -> addValue(col, row, image->value(col, row));
        }
      }
    }
  }
}

template <typename T>
template <typename T2>
void QxrdImageData<T>::subtract(QSharedPointer< QxrdImageData<T2> > image)
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
        *srcp++ -= *destp++;
      }
    } else {
      for (int row=0; row<nrows; row++) {
        for (int col=0; col<ncols; col++) {
          this -> subtractValue(col, row, image->value(col, row));
        }
      }
    }
  }
}

template <typename T>
template <typename T2>
void QxrdImageData<T>::multiply(QSharedPointer< QxrdImageData<T2> > image)
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
        *srcp++ *= *destp++;
      }
    } else {
      for (int row=0; row<nrows; row++) {
        for (int col=0; col<ncols; col++) {
          this -> multiplyValue(col, row, image->value(col, row));
        }
      }
    }
  }
}

template <typename T>
template <typename T2>
void QxrdImageData<T>::divide(QSharedPointer< QxrdImageData<T2> > image)
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
        *srcp++ /= *destp++;
      }
    } else {
      for (int row=0; row<nrows; row++) {
        for (int col=0; col<ncols; col++) {
          this -> divideValue(col, row, image->value(col, row));
        }
      }
    }
  }
}

template <typename T>
double QxrdImageData<T>::correlate(QSharedPointer<QxrdImageData<T> > image, int dx, int dy, int mx, int my)
{
  double sum=0;

  if (image) {
    int wd = this->get_Width();
    int ht = this->get_Height();

    for (int y=my; y<ht-my; y++) {
      for (int x=mx; x<wd-mx; x++) {
        sum += this->value(x,y)*image->value(x+dx,y+dy);
      }
    }
  }

  return sum;
}

template <typename T>
void QxrdImageData<T>::shiftImage(QSharedPointer<QxrdImageData<T> > image, double dx, double dy)
{
  if (image) {
    int wd = this->get_Width();
    int ht = this->get_Height();

    for (int y=0; y<ht; y++) {
      for (int x=0; x<wd; x++) {
        this->setValue(x,y, image->value(x+dx,y+dy));
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
int QxrdImageData<T>::pixelsInRange(double min, double max)
{
  int nRows = this->get_Height();
  int nCols = this->get_Width();
  int nPix = 0;

  for (int row=0; row<nRows; row++) {
    for (int col=0; col<nCols; col++) {
      if (m_Mask == NULL || m_Mask->value(col, row)) {
        double val = this->value(col, row);

        if (val == val) {
          if (val >= min && val <= max) {
            nPix += 1;
          }
        }
      }
    }
  }

  return nPix;
}

template <typename T>
int QxrdImageData<T>::overflowCount(double level) const
{
  double ovlev = level;

  if (this->get_SummedExposures() > 1) {
    ovlev *= this->get_SummedExposures();
  }

  int nRows = this->get_Height();
  int nCols = this->get_Width();
  int nPix = 0;

  for (int row=0; row<nRows; row++) {
    for (int col=0; col<nCols; col++) {
      if (m_Mask == NULL || m_Mask->value(col, row)) {
        double val = this->value(col, row);

        if (val == val) {
          if (val >= ovlev) {
            nPix += 1;
          }
        }
      }
    }
  }

  return nPix;
}

template <typename T>
void QxrdImageData<T>::markOverflows(QxrdMaskDataPtr overflow, double level)
{
  if (overflow) {
    double ovlev = level;

    if (this->get_SummedExposures() > 1) {
      ovlev *= this->get_SummedExposures();
    }

    int nRows = this->get_Height();
    int nCols = this->get_Width();

    for (int row=0; row<nRows; row++) {
      for (int col=0; col<nCols; col++) {
        if (m_Mask == NULL || m_Mask->value(col, row)) {
          double val = this->value(col, row);

          if (val == val) {
            if (val >= ovlev) {
              overflow->setValue(col, row, 1);
            }
          }
        }
      }
    }
  }
}

template <typename T>
double QxrdImageData<T>::findMin() const
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  int first = true;
  double minv = 0;

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
double QxrdImageData<T>::findMax() const
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  int first = true;
  double maxv = 0;

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
void QxrdImageData<T>::calculateRange()
{
//  T *img = m_Image.data();
//  int total = m_Image.count();
  int first = true;

  double minV = 0;
  double maxV = 0;

  int ncols = this -> get_Width();
  int nrows = this -> get_Height();

  for (int row=0; row<nrows; row++) {
    for (int col=0; col<ncols; col++) {
      if (m_Mask == NULL || m_Mask->value(col,row)) {
        double val = this -> value(col, row);

        if (val==val) {
          if (first) {
            minV = val;
            maxV = val;
            first = false;
          } else if (val > maxV) {
            maxV = val;
          } else if (val < minV) {
            minV = val;
          }
        }
      }
    }
  }

  this->m_MinValue = minV;
  this->m_MaxValue = maxV;
}

template <typename T>
double QxrdImageData<T>::minValue()
{
  calculateRange();

  return this->m_MinValue;
}

template <typename T>
double QxrdImageData<T>::maxValue()
{
  calculateRange();

  return this->m_MaxValue;
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

template void QxrdImageData<double>::add<double>(QSharedPointer<QxrdImageData<double> >);
template void QxrdImageData<double>::subtract<double>(QSharedPointer<QxrdImageData<double> >);
template void QxrdImageData<double>::multiply<double>(QSharedPointer<QxrdImageData<double> >);
template void QxrdImageData<double>::divide<double>(QSharedPointer<QxrdImageData<double> >);
