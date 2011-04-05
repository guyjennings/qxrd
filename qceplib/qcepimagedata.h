#ifndef QCEPIMAGEDATA_H
#define QCEPIMAGEDATA_H

#include "qcepmacros.h"

#include <QMutex>
#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QFileInfo>
#include <math.h>
#include <stdio.h>

#include "qcepproperty.h"

template <typename T> class QcepImageData;
template <typename T> class QcepImageDataFormat;
template <typename T> class QcepImageDataFormatFactory;

#include "qcepimagedataformat.h"
#include "qcepimagedataformatfactory.h"

typedef struct tiff TIFF;

class QcepImageDataBase : public QObject
{
  Q_OBJECT;

public:
  QcepImageDataBase(int width=0, int height=0);
  virtual ~QcepImageDataBase();

  Q_PROPERTY(int width READ get_Width);
  //  QCEP_INTEGER_PROPERTY(Width);

  Q_PROPERTY(int height READ get_Height);
  //  QCEP_INTEGER_PROPERTY(Height);

  Q_PROPERTY(QString qxrdVersion READ get_QxrdVersion WRITE set_QxrdVersion);
  QCEP_STRING_PROPERTY(QxrdVersion);

  Q_PROPERTY(QString qtVersion READ get_QtVersion WRITE set_QtVersion);
  QCEP_STRING_PROPERTY(QtVersion);

  Q_PROPERTY(int dataType READ get_DataType WRITE set_DataType);
  QCEP_INTEGER_PROPERTY(DataType);

  Q_PROPERTY(QString dataTypeName READ get_DataTypeName);

  Q_PROPERTY(QString fileBase READ get_FileBase WRITE set_FileBase);
  QCEP_STRING_PROPERTY(FileBase);

  Q_PROPERTY(QString fileName READ get_FileName WRITE set_FileName);
  QCEP_STRING_PROPERTY(FileName);

  Q_PROPERTY(QString title READ get_Title WRITE set_Title);
  QCEP_STRING_PROPERTY(Title);

  Q_PROPERTY(int readoutMode READ get_ReadoutMode WRITE set_ReadoutMode);
  QCEP_INTEGER_PROPERTY(ReadoutMode);

  Q_PROPERTY(double exposureTime READ get_ExposureTime WRITE set_ExposureTime);
  QCEP_DOUBLE_PROPERTY(ExposureTime);

  Q_PROPERTY(int summedExposures READ get_SummedExposures WRITE set_SummedExposures);
  QCEP_INTEGER_PROPERTY(SummedExposures);

  Q_PROPERTY(int imageNumber READ get_ImageNumber WRITE set_ImageNumber);
  QCEP_INTEGER_PROPERTY(ImageNumber);

  Q_PROPERTY(int phaseNumber READ get_PhaseNumber WRITE set_PhaseNumber);
  QCEP_INTEGER_PROPERTY(PhaseNumber);

  Q_PROPERTY(int nPhases READ get_NPhases WRITE set_NPhases);
  QCEP_INTEGER_PROPERTY(NPhases);

  Q_PROPERTY(QDateTime dateTime READ get_DateTime WRITE set_DateTime);
  QCEP_DATETIME_PROPERTY(DateTime);

  Q_PROPERTY(QString dateString READ get_DateString);
  //  QCEP_STRING_PROPERTY(DateString);

  Q_PROPERTY(int hBinning READ get_HBinning WRITE set_HBinning);
  QCEP_INTEGER_PROPERTY(HBinning);

  Q_PROPERTY(int vBinning READ get_VBinning WRITE set_VBinning);
  QCEP_INTEGER_PROPERTY(VBinning);

  Q_PROPERTY(double cameraGain READ get_CameraGain WRITE set_CameraGain);
  QCEP_DOUBLE_PROPERTY(CameraGain);

  Q_PROPERTY(int triggered READ get_Triggered WRITE set_Triggered);
  QCEP_INTEGER_PROPERTY(Triggered);

  Q_PROPERTY(QString userComment1 READ get_UserComment1 WRITE set_UserComment1);
  QCEP_STRING_PROPERTY(UserComment1);

  Q_PROPERTY(QString userComment2 READ get_UserComment2 WRITE set_UserComment2);
  QCEP_STRING_PROPERTY(UserComment2);

  Q_PROPERTY(QString userComment3 READ get_UserComment3 WRITE set_UserComment3);
  QCEP_STRING_PROPERTY(UserComment3);

  Q_PROPERTY(QString userComment4 READ get_UserComment4 WRITE set_UserComment4);
  QCEP_STRING_PROPERTY(UserComment4);

  Q_PROPERTY(int imageSaved READ get_ImageSaved WRITE set_ImageSaved);
  QCEP_INTEGER_PROPERTY(ImageSaved);

  Q_PROPERTY(QcepDoubleList normalization READ get_Normalization WRITE set_Normalization);
  QCEP_DOUBLE_LIST_PROPERTY(Normalization);

  Q_PROPERTY(bool used READ get_Used WRITE set_Used);
  QCEP_BOOLEAN_PROPERTY(Used);

public:
  int get_Width() const
  {
    return m_Width;
  }

  int get_Height() const
  {
    return m_Height;
  }

  void set_Width(int width)
  {
    m_Width = width;
  }

  void set_Height(int height)
  {
    m_Height = height;
  }

  QString get_DateString() const
  {
    return get_DateTime().toString(tr("yyyy.MM.dd : hh:mm:ss.zzz"));
  }

  QString get_DataTypeName() const;

  void copyProperties(QcepImageDataBase *dest);
  void copyPropertiesFrom(QSharedPointer<QcepImageDataBase> src);

  void setTiffMetaData(TIFF *tif);
  void getTiffMetaData(TIFF *tif);

  void loadMetaData();
  void saveMetaData();
  void saveMetaData(QString name);

  void setDefaultFileName(QString path);

  enum {
    UndefinedData,
    Raw16Data,
    Raw32Data,
    DarkData,
    MaskData,
    SubtractedData,
    GainData,
    BadPixelsData
  };

  QMutex *mutex();

protected:
  int  m_Width;
  int  m_Height;

private:
  mutable QMutex m_Mutex;
};

template <typename T>
    class QcepImageData : public QcepImageDataBase
{
public:
  QcepImageData(int width=0, int height=0, T def=0);

public:
  bool readImage(QString filename);

  void resize(int width, int height);
  void clear();

//  template <typename T2>
//  void copyImage(QSharedPointer< QcepImageData<T2> > dest);

  void calculateRange();
  void calculateRangeInCircle();

  void dumpPixels(int x0, int y0, int x1, int y1);

  T value(int x, int y) const;
  T value(double x, double y) const;

  T minValue() const;
  T maxValue() const;

  T* data();

  void setValue(int x, int y, T val);
  void incValue(int x, int y, T val);
  void fill(T val);

  T defaultValue() const;
  void setDefaultValue(T def);

protected:
  QVector<T> m_Image;
  T m_MinValue;
  T m_MaxValue;
  T m_Default;
};

template <typename T>
QcepImageData<T>::QcepImageData(int width, int height, T def)
  : QcepImageDataBase(width, height),
//    m_Image(width*height, def),
    m_Image(width*height),
    m_MinValue(0),
    m_MaxValue(0),
    m_Default(def)
{
  if (def) {
    m_Image.fill(def);
  }
}

template <typename T>
T* QcepImageData<T>::data()
{
  return m_Image.data();
}

template <typename T>
T QcepImageData<T>::value(int x, int y) const
{
  if (x >= 0 && x < get_Width() && y >= 0 && y < get_Height()) {
    return m_Image.value((get_Height()-y-1)*get_Width()+x);
  }

  return m_Default;
}

template <typename T>
T QcepImageData<T>::value(double x, double y) const
{
  int ix = ((int) x), iy = ((int) y);
  double dx = x-ix, dy = y-iy;

  double f00 = value((ix)   , (iy));
  double f10 = value((ix+1) , (iy));
  double f01 = value((ix)   , (iy+1));
  double f11 = value((ix+1) , (iy+1));

  double f0 = f00*(1-dx)+f10*dx;
  double f1 = f01*(1-dx)+f11*dx;

  double f = f0*(1-dy)+f1*dy;

  return f;
}

template <typename T>
void QcepImageData<T>::fill(T val)
{
  m_Image.fill(val);
}

//template <typename T>
//template <typename T2>
//void QcepImageData<T>::copyImage(QSharedPointer< QcepImageData<T2> > dest)
//{
//  if (dest) {
//    int ncols = this -> get_Width();
//    int nrows = this -> get_Height();
//    int npix = ncols*nrows;
//
//    dest -> resize(ncols, nrows);
//
//    this -> copyProperties(dest);
//
//    T *srcp = this -> data();
//    T2 *destp = dest -> data();
//
//    for (int i=0; i<npix; i++) {
//      *destp++ = *srcp++;
//    }
//  }
//}

template <typename T>
void QcepImageData<T>::setValue(int x, int y, T val)
{
  if (x >= 0 && x < get_Width() && y >= 0 && y < get_Height()) {
    m_Image[(get_Height()-y-1)*get_Width()+x] = val;
  }
}

template <typename T>
void QcepImageData<T>::incValue(int x, int y, T val)
{
  if (x >= 0 && x < get_Width() && y >= 0 && y < get_Height()) {
    m_Image[(get_Height()-y-1)*get_Width()+x] += val;
  }
}

template <typename T>
T QcepImageData<T>::minValue() const
{
  return m_MinValue;
}

template <typename T>
T QcepImageData<T>::maxValue() const
{
  return m_MaxValue;
}

template <typename T>
void QcepImageData<T>::calculateRange()
{
  T *img = m_Image.data();
  int total = m_Image.count();
  int first = true;

  m_MinValue = 0;
  m_MaxValue = 0;

  for (int i = 0; i<total; i++) {
    T val = img[i];

    if (first) {
      m_MaxValue = val;
      m_MinValue = val;
      first = false;
    } else if (val > m_MaxValue) {
      m_MaxValue = val;
    } else if (val < m_MinValue) {
      m_MinValue = val;
    }
  }
}

template <typename T>
void QcepImageData<T>::calculateRangeInCircle()
{
  bool first = 1;

  double cx = get_Width()/2, cy = get_Height()/2;

  m_MinValue = 0;
  m_MaxValue = 0;

  for (int y=0; y<get_Height(); y++) {
    double dy = (((double)y)-cy)/cy;
    double dx = sqrt(1-dy*dy);
    int x0 = (int) (cx-dx*cx);
    int x1 = (int) (cx+dx*cx);

    for (int x=x0; x<x1; x++) {
      T val = value(x,y);

      if (first) {
        m_MinValue = val;
        m_MaxValue = val;
        first = false;
      } else if (val > m_MaxValue) {
        m_MaxValue = val;
      } else if (val < m_MinValue) {
        m_MinValue = val;
      }
    }
  }
}

template <typename T>
void QcepImageData<T>::dumpPixels(int x0, int y0, int x1, int y1)
{
  for (int y=y0; y<y1; y++) {
    for (int x=x0; x<x1; x++) {
      printf("%g ", (double) value(x,y));
    }
    printf("\n");
  }
}

template <typename T>
void QcepImageData<T>::resize(int width, int height)
{
  set_Width(width);
  set_Height(height);

  m_Image.resize(width*height);
}

template <typename T>
void QcepImageData<T>::clear()
{
  m_Image.fill(m_Default);
}

template <typename T>
T QcepImageData<T>::defaultValue() const
{
  return m_Default;
}

template <typename T>
void QcepImageData<T>::setDefaultValue(T def)
{
  m_Default = def;
}

template <typename T>
bool QcepImageData<T>::readImage(QString path)
{
  QcepImageDataFormat<T> *loader =
      QcepImageDataFormatFactory<T>::factory()->canLoad(path);

  if (loader) {
    bool res = loader -> loadFile(path, this);

    if (res) {
      QString fileBase = QFileInfo(path).fileName();

      set_FileBase(fileBase);
      set_Title(fileBase);
      set_FileName(path);
      set_ImageSaved(true);
    }

    return res;
  } else {
    return false;
  }
}

#endif
