/******************************************************************
*
*  $Id: qcepimagedata.h,v 1.2 2010/09/13 20:00:25 jennings Exp $
*
*******************************************************************/

#ifndef QCEPIMAGEDATA_H
#define QCEPIMAGEDATA_H

#include "qcepmacros.h"

#include <QMutex>
#include <QString>
#include <QVector>
#include <QSharedPointer>
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

  Q_PROPERTY(int dataType READ get_DataType WRITE set_DataType);
  QCEP_INTEGER_PROPERTY(DataType);

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
  HEADER_IDENT("$Id: qcepimagedata.h,v 1.2 2010/09/13 20:00:25 jennings Exp $");
};

template <typename T>
    class QcepImageData : public QcepImageDataBase
{
public:
  QcepImageData(int width=0, int height=0);

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
  void fill(T val);

protected:
  QVector<T> m_Image;
  T m_MinValue;
  T m_MaxValue;
};

template <typename T>
QcepImageData<T>::QcepImageData(int width, int height)
  : QcepImageDataBase(width, height),
    m_Image(width*height),
    m_MinValue(0),
    m_MaxValue(0)
{
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

  return 0;
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
  m_Image.fill(0);
}

template <typename T>
bool QcepImageData<T>::readImage(QString path)
{
  QcepImageDataFormat<T> *loader =
      QcepImageDataFormatFactory<T>::factory()->canLoad(path);

  if (loader) {
    bool res = loader -> loadFile(path, this);

    if (res) {
      set_FileName(path);
      set_ImageSaved(true);
    }

    return res;
  } else {
    return false;
  }
}

#endif

/******************************************************************
*
*  $Log: qcepimagedata.h,v $
*  Revision 1.2  2010/09/13 20:00:25  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/06/17 18:35:33  jennings
*  Added copyPropertiesFrom method
*
*  Revision 1.1.2.2  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.1.2.1  2010/04/13 19:29:12  jennings
*  Added qceplib to cvs
*
*  Revision 1.35  2009/12/11 17:53:09  jennings
*  Added ImageSaved property to image data
*
*  Revision 1.34  2009/12/01 14:10:37  jennings
*  Added interpolating T value(double x, double y) method to QcepImageData
*  Changed center finder plot to use interpolating value routine
*
*  Revision 1.33  2009/11/13 20:16:05  jennings
*  *** empty log message ***
*
*  Revision 1.32  2009/10/21 21:33:45  jennings
*  Added string format date property
*
*  Revision 1.31  2009/09/26 04:57:31  jennings
*  Removed some commented out sections
*
*  Revision 1.30  2009/09/25 16:09:02  jennings
*  Added copyImage function to QcepImageData<>
*
*  Revision 1.29  2009/09/22 20:41:07  jennings
*  Set filename and title properties when loading data files
*
*  Revision 1.28  2009/09/22 19:45:33  jennings
*  Small changes to range calculating code for image data
*
*  Revision 1.27  2009/09/21 18:12:55  jennings
*  Added 'triggered', 'usercomment{1-4}' properties to data
*
*  Revision 1.26  2009/09/14 19:09:33  jennings
*  Added hBinning, vBinning and cameraGain properties to images
*
*  Revision 1.25  2009/09/12 14:44:53  jennings
*  Moved lock to base class, made into mutex
*  Added DataType property
*
*  Revision 1.24  2009/09/12 13:35:45  jennings
*  Removed 'threshold' from QcepImageData
*
*  Revision 1.23  2009/09/09 22:32:12  jennings
*  Started to add TIFF metadata support
*
*  Revision 1.22  2009/09/08 12:26:04  jennings
*  Added error return value to loadImage
*
*  Revision 1.21  2009/08/26 20:53:04  jennings
*  Added copyProperties function to facilitate conversion between image data types
*
*  Revision 1.20  2009/08/25 18:49:19  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.19  2009/08/08 00:16:24  jennings
*  Added fill operation
*
*  Revision 1.18  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.17  2009/08/03 13:27:25  jennings
*  Moved mask-related routines into non-templated base class
*  Added maskCircle routine
*
*  Revision 1.16  2009/06/28 16:40:18  jennings
*  Removed setData and setMask operations as they can make the image and/or mask the wrong size
*
*  Revision 1.15  2009/06/28 16:34:27  jennings
*  Fixed problems with copyMask which could result in image and mask dimensions getting out of sync.
*
*  Revision 1.14  2009/06/28 11:52:49  jennings
*  Optimized image operations by special case handling of width and height properties
*
*  Revision 1.13  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

