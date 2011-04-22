#include "qcepimagedata.h"
#include <tiffio.h>

#include <QSettings>
#include <QFileInfo>

//static int allocCount = 0;

QcepImageDataBase::QcepImageDataBase(int width, int height)
  : QObject(),
    m_QxrdVersion(this,"qxrdVersion","unknown"),
    m_QtVersion(this,"qtVersion","unknown"),
    m_DataType(this, "dataType", UndefinedData),
    m_FileBase(this, "fileBase", ""),
    m_FileName(this, "fileName", ""),
    m_Title(this, "title", ""),
    m_ReadoutMode(this, "readoutMode", 0),
    m_ExposureTime(this, "exposureTime", 0),
    m_SummedExposures(this, "summedExposures", 0),
    m_ImageNumber(this, "imageNumber", 0),
    m_PhaseNumber(this, "phaseNumber", -1),
    m_NPhases(this, "nPhases", -1),
    m_DateTime(this, "dateTime", QDateTime::currentDateTime()),
    m_HBinning(this, "hBinning", 1),
    m_VBinning(this, "vBinning", 1),
    m_CameraGain(this, "cameraGain", 0),
    m_Triggered(this, "triggered", 0),
    m_UserComment1(this,"userComment1",""),
    m_UserComment2(this,"userComment2",""),
    m_UserComment3(this,"userComment3",""),
    m_UserComment4(this,"userComment4",""),
    m_ImageSaved(this,"imageSaved",0),
    m_Normalization(this, "normalization", QcepDoubleList()),
    m_Used(this, "used", true),
    m_Width(width),
    m_Height(height),
    m_Mutex(QMutex::Recursive)
{
//    printf("QcepImageDataBase::QcepImageDataBase allocate %d\n", allocCount++);
}

QcepImageDataBase::~QcepImageDataBase()
{
//    printf("QcepImageDataBase::QcepImageDataBase deallocate %d\n", allocCount--);
}

QMutex *QcepImageDataBase::mutex()
{
  return &m_Mutex;
}

void QcepImageDataBase::copyProperties(QcepImageDataBase *dest)
{
  dest -> set_DataType(get_DataType());
  dest -> set_FileBase(get_FileBase());
  dest -> set_FileName(get_FileName());
  dest -> set_Title(get_Title());
  dest -> set_ReadoutMode(get_ReadoutMode());
  dest -> set_ExposureTime(get_ExposureTime());
  dest -> set_SummedExposures(get_SummedExposures());
  dest -> set_ImageNumber(get_ImageNumber());
  dest -> set_PhaseNumber(get_PhaseNumber());
  dest -> set_NPhases(get_NPhases());
  dest -> set_DateTime(get_DateTime());
  dest -> set_HBinning(get_HBinning());
  dest -> set_VBinning(get_VBinning());
  dest -> set_CameraGain(get_CameraGain());
  dest -> set_Triggered(get_Triggered());
  dest -> set_UserComment1(get_UserComment1());
  dest -> set_UserComment2(get_UserComment2());
  dest -> set_UserComment3(get_UserComment3());
  dest -> set_UserComment4(get_UserComment4());
  dest -> set_ImageSaved(get_ImageSaved());
  dest -> set_Normalization(get_Normalization());
  dest -> set_Used(get_Used());

  QByteArray name;

  foreach(name, dynamicPropertyNames()) {
    dest -> setProperty(name.data(), property(name.data()));
  }
}

void QcepImageDataBase::copyPropertiesFrom(QSharedPointer<QcepImageDataBase> src)
{
  set_DataType(src -> get_DataType());
  set_FileBase(src -> get_FileBase());
  set_FileName(src -> get_FileName());
  set_Title(src -> get_Title());
  set_ReadoutMode(src -> get_ReadoutMode());
  set_ExposureTime(src -> get_ExposureTime());
  set_SummedExposures(src -> get_SummedExposures());
  set_ImageNumber(src -> get_ImageNumber());
  set_PhaseNumber(src -> get_PhaseNumber());
  set_NPhases(src -> get_NPhases());
  set_DateTime(src -> get_DateTime());
  set_HBinning(src -> get_HBinning());
  set_VBinning(src -> get_VBinning());
  set_CameraGain(src -> get_CameraGain());
  set_Triggered(src -> get_Triggered());
  set_UserComment1(src -> get_UserComment1());
  set_UserComment2(src -> get_UserComment2());
  set_UserComment3(src -> get_UserComment3());
  set_UserComment4(src -> get_UserComment4());
  set_ImageSaved(src -> get_ImageSaved());
  set_Normalization(src -> get_Normalization());
  set_Used(src -> get_Used());

  QByteArray name;

  foreach(name, src -> dynamicPropertyNames()) {
    setProperty(name.data(), src -> property(name.data()));
  }
}

void QcepImageDataBase::setTiffMetaData(TIFF *tif)
{
  TIFFSetField(tif, TIFFTAG_DOCUMENTNAME, qPrintable(get_FileName()));
  TIFFSetField(tif, TIFFTAG_DATETIME, qPrintable(get_DateTime().toString("yyyy:MM:dd hh:mm:ss")));
}

void QcepImageDataBase::getTiffMetaData(TIFF * /*tif*/)
{
}

void QcepImageDataBase::loadMetaData()
{
//  printf("QcepImageDataBase::loadMetaData for file %s\n", qPrintable(get_FileName()));

  QTime tic;
  tic.start();

  {
    QMutexLocker lock(mutex());

    QSettings settings(get_FileName()+".metadata", QSettings::IniFormat);

    QcepProperty::readSettings(this, metaObject(), "metadata", settings);
  }
//
//  printf("QcepImageDataBase::loadMetaData for file %s took %d msec\n",  qPrintable(get_FileName()), tic.elapsed());
}

void QcepImageDataBase::saveMetaData()
{
  saveMetaData(get_FileName());
}

void QcepImageDataBase::saveMetaData(QString name)
{
//  printf("QcepImageDataBase::saveMetaData for file %s\n", qPrintable(name));

  QTime tic;
  tic.start();

//  printf("type 266 = %s\n", QMetaType::typeName(266));

  {
    QMutexLocker lock(mutex());

    QSettings settings(name+".metadata", QSettings::IniFormat);

    QcepProperty::writeSettings(this, &staticMetaObject/*metaObject()*/, "metadata", settings);

    settings.beginWriteArray("normalization");
    QcepDoubleList norm = get_Normalization();

    for (int i=0; i<norm.length(); i++) {
      settings.setArrayIndex(i);
      settings.setValue("val",norm[i]);
    }
    settings.endArray();
  }
//
//  printf("QcepImageDataBase::saveMetaData for file %s took %d msec\n",  qPrintable(name), tic.elapsed());
}

void QcepImageDataBase::setDefaultFileName(QString path)
{
  set_FileName(path);
  set_Title(QFileInfo(path).fileName());
}

QString QcepImageDataBase::get_DataTypeName() const
{
  switch (get_DataType()) {
  default:
  case UndefinedData:
    return "Undefined";

  case Raw16Data:
    return "16 Bit Raw Data";

  case Raw32Data:
    return "32 Bit Raw Data";

  case DarkData:
    return "Dark Data";

  case MaskData:
    return "Mask Data";

  case SubtractedData:
    return "Subtracted Data";

  case GainData:
    return "Pixel Gain Data";

  case BadPixelsData:
    return "Bad Pixel Data";
  }
}

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

template class QcepImageData<unsigned short>;
template class QcepImageData<short>;
template class QcepImageData<unsigned int>;
template class QcepImageData<int>;
template class QcepImageData<double>;
