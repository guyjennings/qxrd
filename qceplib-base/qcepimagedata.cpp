#include "qcepdebug.h"
#include "qcepdataobject.h"
#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include "qcepallocator.h"

#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QScriptEngine>
#include <typeinfo>
#include <QMetaProperty>

#include "qcepimagedataformat.h"
#include "qcepimagedataformatfactory.h"
#include "qcepmutexlocker.h"

#ifdef HAVE_TIFF
#include "tiffio.h"
#endif

QAtomicInt allocCount = 0;

QcepImageDataBase::QcepImageDataBase(QString name, int width, int height, int size)
  : QcepDataObject(name, size),
    m_Width(this, "width", width, "Image Width"),
    m_Height(this, "height", height, "Image Height"),
    m_HStart(this, "hStart", 0.0, "H Start Coord"),
    m_HStep(this, "hStep", 1.0, "H Step"),
    m_VStart(this, "vStart", 0.0, "V Start Coord"),
    m_VStep(this, "vStep", 1.0, "V Step"),
    m_HLabel(this, "hLabel", "X", "H Label"),
    m_HUnits(this, "hUnits", "pix", "H Units"),
    m_VLabel(this, "vLabel", "Y", "V Label"),
    m_VUnits(this, "vUnits", "pix", "V Units"),
    m_DataType(this, "dataType", UndefinedData, "Data Type of Image"),
    m_FileBase(this, "fileBase", "", "File Base of Image"),
//    m_Title(saver, this, "title", "", "Title of Image"),
    m_ReadoutMode(this, "readoutMode", 0, "Image Readout Mode"),
    m_ExposureTime(this, "exposureTime", 0, "Image Exposure Time"),
    m_SummedExposures(this, "summedExposures", 0, "Summed Exposures in Image"),
    m_ImageSequenceNumber(this, "imageSequenceNumber", -1, "Image Sequence Number"),
    m_ImageNumber(this, "imageNumber", 0, "Image Number"),
    m_PhaseNumber(this, "phaseNumber", -1, "Image Phase Number"),
    m_NPhases(this, "nPhases", -1, "Number of Image Phases"),
    m_DateTime(this, "dateTime", QDateTime::currentDateTime(), "Image Creation Date and Time"),
    m_TimeStamp(this, "timeStamp", secondsSinceEpoch(), "Image creation time (secs after UNIX epoch)"),
    m_HBinning(this, "hBinning", 1, "Image Horizontal Binning Factor"),
    m_VBinning(this, "vBinning", 1, "Image Vertical Binning Factor"),
    m_CameraGain(this, "cameraGain", 0, "Image Detector Gain"),
    m_Triggered(this, "triggered", 0, "Triggered Flag"),
    m_UserComment1(this,"userComment1","", "User Comment 1"),
    m_UserComment2(this,"userComment2","", "User Comment 2"),
    m_UserComment3(this,"userComment3","", "User Comment 3"),
    m_UserComment4(this,"userComment4","", "User Comment 4"),
    m_Normalization(this, "normalization", QcepDoubleList(), "Normalization Values"),
    m_ExtraInputs(this, "extraInputs", QcepDoubleList(), "Extra Input Values"),
    m_Used(this, "used", true, "Image Used?"),
    m_ImageCounter(allocCount.fetchAndAddOrdered(1)),
    m_Mutex(QMutex::Recursive)
{
  if (qcepDebug(DEBUG_IMAGE_CONSTRUCTORS)) {
    printf("QcepImageDataBase::QcepImageDataBase(%p)\n", this);
  }

  QcepAllocator::allocate(size);
}

QcepImageDataBase::~QcepImageDataBase()
{
  if (qcepDebug(DEBUG_IMAGE_CONSTRUCTORS)) {
    printf("QcepImageDataBase::~QcepImageDataBase(%p)\n", this);
//    QcepSettingsSaverPtr s(m_Saver);

//    if (s) {
//      s->printMessage(tr("QcepImageDataBase::~QcepImageDataBase %1[%2]")
//                      .HEXARG(this).arg(m_ImageCounter));
//    }
  }

  QcepAllocator::deallocate(get_ByteSize());
}

QString QcepImageDataBase::description() const
{
  return tr("[%1 x %2] elements").arg(get_Width()).arg(get_Height());
}

double QcepImageDataBase::secondsSinceEpoch()
{
#if QT_VERSION >= 0x040700
  return QDateTime::currentMSecsSinceEpoch()/1000.0;
#else
  QDateTime dt = QDateTime::currentDateTime();
  time_t dt_t = dt.toTime_t();
  int msec = dt.time().msec();

  return ((double) dt_t) + msec/1000.0;
#endif
}

QMutex *QcepImageDataBase::mutex()
{
  return &m_Mutex;
}

void QcepImageDataBase::copyProperties(QcepImageDataBase *dest)
{
  dest -> set_Creator(get_Creator());
  dest -> set_Version(get_Version());
  dest -> set_QtVersion(get_QtVersion());
  dest -> set_DataType(get_DataType());
  dest -> set_FileBase(get_FileBase());
  dest -> set_FileName(get_FileName());
  dest -> set_Name(get_Name()+" copy");
  dest -> set_ReadoutMode(get_ReadoutMode());
  dest -> set_ExposureTime(get_ExposureTime());
  dest -> set_SummedExposures(get_SummedExposures());
  dest -> set_ImageSequenceNumber(get_ImageSequenceNumber());
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
  dest -> set_ObjectSaved(get_ObjectSaved());
  dest -> set_Normalization(get_Normalization());
  dest -> set_ExtraInputs(get_ExtraInputs());
  dest -> set_Used(get_Used());

  QByteArray name;

  foreach(name, dynamicPropertyNames()) {
    dest -> setProperty(name.data(), property(name.data()));
  }
}

void QcepImageDataBase::copyPropertiesFrom(QSharedPointer<QcepImageDataBase> src)
{
  set_Creator(src -> get_Creator());
  set_Version(src -> get_Version());
  set_QtVersion(src->get_QtVersion());
  set_DataType(src -> get_DataType());
  set_FileBase(src -> get_FileBase());
  set_FileName(src -> get_FileName());
  set_Name(src -> get_Name()+" copy");
  set_ReadoutMode(src -> get_ReadoutMode());
  set_ExposureTime(src -> get_ExposureTime());
  set_SummedExposures(src -> get_SummedExposures());
  set_ImageSequenceNumber(src -> get_ImageSequenceNumber());
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
  set_ObjectSaved(src -> get_ObjectSaved());
  set_Normalization(src -> get_Normalization());
  set_ExtraInputs(src -> get_ExtraInputs());
  set_Used(src -> get_Used());

  QByteArray name;

  foreach(name, src -> dynamicPropertyNames()) {
    setProperty(name.data(), src -> property(name.data()));
  }
}

void QcepImageDataBase::loadMetaData()
{
//  printf("QcepImageDataBase::loadMetaData for file %s\n", qPrintable(get_FileName()));

  QTime tic;
  tic.start();

  {
    set_Creator("Unknown");
    set_Version("Unknown");
    set_QtVersion("Unknown");

    QcepMutexLocker lock(__FILE__, __LINE__, mutex());

    QSettings settings(get_FileName()+".metadata", QSettings::IniFormat);

    settings.beginGroup("metadata");
    QcepProperty::readSettings(this, &settings);

    if (settings.contains("qxrdVersion")) {
      set_Creator("QXRD");
      set_Version(settings.value("qxrdVersion").toString());
    }

    settings.endGroup();
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
    QcepMutexLocker lock(__FILE__, __LINE__, mutex());

    QSettings settings(name+".metadata", QSettings::IniFormat);

    settings.beginGroup("metadata");
    QcepProperty::writeSettings(this, &settings);
    settings.endGroup();

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
  set_Name(QFileInfo(path).fileName());
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

QString QcepImageDataBase::fileFormatFilterString()
{
  return fileFormatTIFF() + ";;" +
      fileFormatTabDelimited() + ";;" +
      fileFormatTransposedTabDelimited() + ";;" +
      fileFormatCSV() + ";;" +
      fileFormatTransposedCSV();
}

double QcepImageDataBase::hValue(int n) const
{
  return get_HStart() + n*get_HStep();
}

double QcepImageDataBase::vValue(int n) const
{
  return get_VStart() + n*get_VStep();
}

template <typename T>
QcepImageData<T>::QcepImageData(QString name, int width, int height, T def)
  : QcepImageDataBase(name, width, height, width*height*sizeof(T)),
    m_Image(width*height),
    m_MinValue(0),
    m_MaxValue(0),
    m_Default(def),
    m_Mask(NULL),
    m_Overflow(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printMessage(tr("QcepImageData<%1>::QcepImageData %2")
                                .arg(typeid(T).name())
                                .HEXARG(this));
  }

  if (def) {
    m_Image.fill(def);
  }
}

template <typename T>
QcepImageData<T>::~QcepImageData()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printMessage(tr("QcepImageData<%1>::~QcepImageData %2")
                      .arg(typeid(T).name())
                      .HEXARG(this));
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
QVector<double> QcepImageData<T>::getImageData(int x0, int y0, int x1, int y1) const
{
  QVector<double> res;

  for (int y=y0; y<y1; y++) {
    for (int x=x0; x<x1; x++) {
      res << this->value(x,y);
    }
  }

  return res;
}

template <typename T>
double QcepImageData<T>::getImageData(int x, int y) const
{
  return this->value(x,y);
}

//template <typename T>
//void QcepImageData<T>::fill(T val)
//{
//  m_Image.fill(val);
//}

template <typename T>
void QcepImageData<T>::fill(double val)
{
  m_Image.fill(val);
}

template <typename T>
void QcepImageData<T>::setImageData(int x, int y, double v)
{
  if (x >= 0 && x < get_Width() && y >= 0 && y < get_Height()) {
    m_Image[(get_Height()-y-1)*get_Width()+x] = v;
  }
}

template <typename T>
void QcepImageData<T>::setValue(int x, int y, T val)
{
  if (x >= 0 && x < get_Width() && y >= 0 && y < get_Height()) {
    m_Image[(get_Height()-y-1)*get_Width()+x] = val;
  }
}

template <typename T>
void QcepImageData<T>::addValue(int x, int y, T val)
{
  if (x >= 0 && x < get_Width() && y >= 0 && y < get_Height()) {
    m_Image[(get_Height()-y-1)*get_Width()+x] += val;
  }
}

template <typename T>
void QcepImageData<T>::subtractValue(int x, int y, T val)
{
  if (x >= 0 && x < get_Width() && y >= 0 && y < get_Height()) {
    m_Image[(get_Height()-y-1)*get_Width()+x] -= val;
  }
}

template <typename T>
void QcepImageData<T>::multiplyValue(int x, int y, T val)
{
  if (x >= 0 && x < get_Width() && y >= 0 && y < get_Height()) {
    m_Image[(get_Height()-y-1)*get_Width()+x] *= val;
  }
}

template <typename T>
void QcepImageData<T>::divideValue(int x, int y, T val)
{
  if (x >= 0 && x < get_Width() && y >= 0 && y < get_Height()) {
    m_Image[(get_Height()-y-1)*get_Width()+x] /= val;
  }
}

//template <typename T>
//T QcepImageData<T>::minValue() const
//{
//  return m_MinValue;
//}

//template <typename T>
//T QcepImageData<T>::maxValue() const
//{
//  return m_MaxValue;
//}

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

    if (val == val) { // NaN test...
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

      if (val == val) { // NaN test...
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
  QcepImageData<T> temp(get_Name(), get_Width(), get_Height(), 0);

  int oldwidth = get_Width();
  int oldheight= get_Height();

  for (int y=0; y<oldheight; y++) {
    for (int x=0; x<oldwidth; x++) {
      temp.setValue(x, y, value(x,y));
    }
  }

  set_Width(width);
  set_Height(height);

  m_Image.resize(width*height);

  for (int y=0; y<height; y++) {
    for (int x=0; x<width; x++) {
      setValue(x, y, temp.value(x,y));
    }
  }

  set_ByteSize(width*height*sizeof(T));

  QcepAllocator::allocate((width*height - oldwidth*oldheight)*sizeof(T));
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

      set_Name(fileBase);
      set_FileBase(fileBase);
      set_FileName(path);
      set_ObjectSaved(true);
    }

    return res;
  } else {
    return false;
  }
}

template <typename T>
template <typename T2>
void QcepImageData<T>::subtractDark(const QSharedPointer< QcepImageData<T2> > dark)
{
  //  if (get_PerformDarkSubtraction()) {
  if (dark->get_ExposureTime() != this->get_ExposureTime()) {
    printMessage("Exposure times of acquired data and dark image are different, skipping");
    return;
  }

  if (dark->get_Width() != this->get_Width() ||
      dark->get_Height() != this->get_Height()) {
    printMessage("Dimensions of acquired data and dark image are different, skipping");
    return;
  }

  if (dark->get_CameraGain() != this->get_CameraGain()) {
    printMessage("Gains of acquired data and dark image are different, skipping");
    return;
  }

  //    if (!(image->get_DataType() == QcepDoubleImageData::Raw16Data ||
  //          image->get_DataType() == QcepDoubleImageData::Raw32Data)) {
  //      printMessage("Acquired data is not a raw image, skipping background subtraction");
  //      return;
  //    }

  QcepMutexLocker lock1(__FILE__, __LINE__, dark->mutex());
  QcepMutexLocker lock2(__FILE__, __LINE__, this->mutex());

  int height = this->get_Height();
  int width  = this->get_Width();
  int nres = this -> get_SummedExposures();
  int ndrk = dark -> get_SummedExposures();
  int npixels = width*height;

  if (nres <= 0) nres = 1;

  double ratio = ((double) nres)/((double) ndrk);

  //      printf("Dark subtraction nres=%d, ndrk=%d, npixels=%d, ratio=%g\n",
  //             nres, ndrk, npixels, ratio);

  double *result = this->data();
  T2     *dk     = dark->data();
  double avgraw  = 0;
  //      double avgdark = 0;

  for (int i=0; i<npixels; i++) {
    //        avgdark  += dk[i];
    avgraw   += result[i];
    result[i] = result[i]-ratio*dk[i];
  }

  //      set_AverageDark(avgdark/npixels/ndrk);
//  set_AverageRaw(avgraw/npixels/nres);
//  set_Average(get_AverageRaw() - get_AverageDark());

  this -> set_DataType(SubtractedData);
}

template <typename T>
QString QcepImageData<T>::rawFileName()
{
  QFileInfo info(QcepImageData<T>::get_FileName());

  QString name = info.dir().filePath(info.completeBaseName()+".raw.tif");

  return name;
}

template <typename T>
template <typename T2>
void QcepImageData<T>::copyImage(QSharedPointer< QcepImageData<T2> > dest)
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
void QcepImageData<T>::copyFrom(QSharedPointer< QcepImageData<T2> > img)
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
void QcepImageData<T>::accumulateImage(QSharedPointer< QcepImageData<T2> > image)
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
void QcepImageData<T>::add(QSharedPointer< QcepImageData<T2> > image)
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
void QcepImageData<T>::add(double val)
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  int npix = ncols*nrows;

  T *srcp = this -> data();

  for (int i=0; i<npix; i++) {
    *srcp++ += val;
  }
}

template <typename T>
template <typename T2>
void QcepImageData<T>::subtract(QSharedPointer< QcepImageData<T2> > image)
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
void QcepImageData<T>::subtract(double val)
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  int npix = ncols*nrows;

  T *srcp = this -> data();

  for (int i=0; i<npix; i++) {
    *srcp++ -= val;
  }
}

template <typename T>
template <typename T2>
void QcepImageData<T>::multiply(QSharedPointer< QcepImageData<T2> > image)
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
void QcepImageData<T>::multiply(double val)
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  int npix = ncols*nrows;

  T *srcp = this -> data();

  for (int i=0; i<npix; i++) {
    *srcp++ *= val;
  }
}

template <typename T>
template <typename T2>
void QcepImageData<T>::divide(QSharedPointer< QcepImageData<T2> > image)
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
void QcepImageData<T>::divide(double val)
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  int npix = ncols*nrows;

  T *srcp = this -> data();

  for (int i=0; i<npix; i++) {
    *srcp++ /= val;
  }
}

template <typename T>
double QcepImageData<T>::correlate(QSharedPointer<QcepImageData<T> > image, int dx, int dy, int mx, int my)
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
void QcepImageData<T>::shiftImage(QSharedPointer<QcepImageData<T> > image, double dx, double dy)
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
void QcepImageData<T>::setMask(QcepMaskDataPtr mask, QcepMaskDataPtr overflow)
{
  m_Mask     = mask;
  m_Overflow = overflow;
}

template <typename T>
QcepMaskDataPtr QcepImageData<T>::mask() const
{
  return m_Mask;
}

template <typename T>
QcepMaskDataPtr QcepImageData<T>::overflow() const
{
  return m_Overflow;
}

template <typename T>
double QcepImageData<T>::minValue() const
{
  return findMin();
}

template <typename T>
double QcepImageData<T>::maxValue() const
{
  return findMax();
}

template <typename T>
double QcepImageData<T>::averageValue() const
{
  return findAverage();
}

template <typename T>
T QcepImageData<T>::findMin() const
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  int first = true;
  T minv = 0;

  for (int row=0; row<nrows; row++) {
    for (int col=0; col<ncols; col++) {
      if (m_Mask == NULL || m_Mask->value(col,row)) {
        T val = this->value(col, row);

        if (val == val) { // Skip NaNs
          if (first) {
            minv = val;
            first = false;
          } else if (val < minv){
            minv = val;
          }
        }
      }
    }
  }

  return minv;
}

template <typename T>
T QcepImageData<T>::findMax() const
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  int first = true;
  T maxv = 0;

  for (int row=0; row<nrows; row++) {
    for (int col=0; col<ncols; col++) {
      if (m_Mask == NULL || m_Mask->value(col,row)) {
        T val = this -> value(col, row);

        if (val == val) {
          if (first) {
            maxv = val;
            first = false;
          } else if (val > maxv){
            maxv = val;
          }
        }
      }
    }
  }

  return maxv;
}

template <typename T>
double QcepImageData<T>::findAverage() const
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  double npix = 0;
  double sum = 0;

  for (int row=0; row<nrows; row++) {
    for (int col=0; col<ncols; col++) {
      if (m_Mask == NULL || m_Mask->value(col,row)) {
        double val = this -> value(col, row);

        if (val==val) {
          npix += 1;
          sum += val;
        }
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
QPointF QcepImageData<T>::percentileRange(double lowpct, double highpct)
{
  const int histSize = 65536;
  QVector<int> histogramVec(histSize+1);
  histogramVec.fill(0.0);
  int *histogram = histogramVec.data();

  int ncols = this -> get_Width();
  int nrows = this -> get_Height();

  double minVal = minValue();
  double maxVal = maxValue();

  double histStep = (maxVal - minVal + 2)/histSize;
  int nAbove = 0, nBelow = 0, nTotal = 0;

  for (int row=0; row<nrows; row++) {
    for (int col=0; col<ncols; col++) {
      if (m_Mask == NULL || m_Mask->value(col, row)) {
        double val = value(col, row);

        if (val==val) {
          double bin = (val - minVal)/histStep;

          if (bin < 0) {
            nBelow += 1;
          } else if (bin >= histSize) {
            nAbove += 1;
          } else {
            histogram[(int) bin] += 1;
          }

          nTotal += 1;
        }
      }
    }
  }

  double lowCount = ((double) nTotal) * lowpct / 100.0;
  double highCount = ((double) nTotal) * highpct / 100.0;
  double count = nBelow;

  QPointF res(0,0);

  for (int i=0; i<histSize; i++) {
    double binVal = minVal + i*histStep;

    count += histogram[i];

    if (count < lowCount) {
      res.setX(binVal);
    }

    if (count < highCount) {
      res.setY(binVal);
    }
  }

  if (res.y() <= res.x()) {
    res.setY(res.x() + 1);
  }

  return res;
}

template <typename T>
void QcepImageData<T>::correctBadBackgroundSubtraction(QcepDoubleImageDataPtr dark, int nImgExposures, int nDarkExposures)
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

template <typename T>
QScriptValue QcepImageData<T>::toScriptValue(QScriptEngine *engine, const QSharedPointer< QcepImageData<T> > &data)
{
  return engine->newQObject(data.data());
}

template <typename T>
void QcepImageData<T>::fromScriptValue(const QScriptValue &obj, QSharedPointer<QcepImageData<T> > &data)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepDataObject *qdobj = qobject_cast<QcepDataObject*>(qobj);

    if (qdobj) {
      QcepObjectPtr p = qdobj->sharedFromThis();

      if (p) {
        QSharedPointer<QcepImageData<T> > cs = qSharedPointerDynamicCast< QcepImageData<T> >(p);

        if (cs) {
          data = cs;
        }
      }
    }
  }
}

#define TIFFCHECK(a) if (res && ((a)==0)) { res = 0; }

template <>
void QcepImageData<double>::saveTIFFData(QString name)
{
#ifdef HAVE_TIFF
  int nrows = get_Height();
  int ncols = get_Width();

  TIFF* tif = TIFFOpen(qPrintable(name),"w");
  int res = 1;

  if (tif) {
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));

    TIFFCHECK(TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP));

    TIFFCHECK(TIFFSetField(tif, TIFFTAG_DOCUMENTNAME, qPrintable(get_FileName())));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_DATETIME,     qPrintable(get_DateTime().toString("yyyy:MM:dd hh:mm:ss"))));

    QVector<float> buffvec(ncols);
    float* buffer = buffvec.data();

    for (int y=0; y<nrows; y++) {
      for (int x=0; x<ncols; x++) {
        buffer[x] = value(x,y);
      }

      TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
    }

    TIFFClose(tif);

    set_FileName(name);
    set_ObjectSaved(true);

    saveMetaData();
  }
#else
  printMessage("Unable to save TIFF data, TIFF support not configured");
#endif
}

template <typename T>
void QcepImageData<T>::saveTIFFData(QString name)
{
#ifdef HAVE_TIFF
  int nrows = get_Height();
  int ncols = get_Width();

  TIFF* tif = TIFFOpen(qPrintable(name),"w");
  int res = 1;

  if (tif) {
    int nsum = get_SummedExposures();

    TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));

    TIFFCHECK(TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));

    if (nsum == 0) {
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8));
    } else if (nsum == 1) {
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16));
    } else {
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32));
    }

    TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT));

    TIFFCHECK(TIFFSetField(tif, TIFFTAG_DOCUMENTNAME, qPrintable(get_FileName())));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_DATETIME,     qPrintable(get_DateTime().toString("yyyy:MM:dd hh:mm:ss"))));

    if (nsum == 0) {
      QVector<quint8> buffvec(ncols);
      quint8* buffer = buffvec.data();

      for (int y=0; y<nrows; y++) {
        for (int x=0; x<ncols; x++) {
          buffer[x] = value(x,y);
        }

        TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
      }
    } else if (nsum == 1) {
      QVector<quint16> buffvec(ncols);
      quint16* buffer = buffvec.data();

      for (int y=0; y<nrows; y++) {
        for (int x=0; x<ncols; x++) {
          buffer[x] = value(x,y);
        }

        TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
      }
    } else {
      QVector<quint32> buffvec(ncols);
      quint32* buffer = buffvec.data();

      for (int y=0; y<nrows; y++) {
        for (int x=0; x<ncols; x++) {
          buffer[x] = value(x,y);
        }

        TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
      }
    }

    TIFFClose(tif);

    set_FileName(name);
    set_ObjectSaved(true);

    saveMetaData();
  }
#else
  printMessage("Unable to save TIFF data, TIFF support not configured");
#endif
}

void QcepImageDataBase::saveTextData(QString name, QString sep, bool transp)
{
  FILE *f = fopen(qPrintable(name), "w+");

  if (f) {
    const QMetaObject *meta = metaObject();

    int count = meta->propertyCount();
    int offset = QObject::staticMetaObject.propertyOffset();

    for (int i=offset; i<count; i++) {
      QMetaProperty metaproperty = meta->property(i);
      const char *name = metaproperty.name();
      QVariant value = property(name);

      fprintf(f, "#%s = %s\n", name, qPrintable(value.toString()));
    }

    foreach (QByteArray name, dynamicPropertyNames()) {
      QVariant value = property(name);

      fprintf(f, "#%s = %s\n", name.data(), qPrintable(value.toString()));
    }

    int nrows = get_Height();
    int ncols = get_Width();

    if (transp == false) {
      for (int y=0; y<nrows; y++) {
        for (int x=0; x<ncols; x++) {
          if (x == 0) {
            fprintf(f, "%g", getImageData(x,y));
          } else {
            fprintf(f, "%s%g", qPrintable(sep), getImageData(x,y));
          }
        }
        fprintf(f, "\n");
      }
    } else { // Transposed
      for (int y=0; y<ncols; y++) {
        for (int x=0; x<nrows; x++) {
          if (x == 0) {
            fprintf(f, "%g", getImageData(y,x));
          } else {
            fprintf(f, "%s%g", qPrintable(sep), getImageData(y,x));
          }
        }
        fprintf(f, "\n");
      }
    }

    fclose(f);
  }
}

template <typename T>
void QcepImageData<T>::saveData(QString &name, QString filter, Overwrite canOverwrite)
{
  mkPath(name);

  if (canOverwrite == NoOverwrite) {
    name = uniqueFileName(name);
  }

  if (filter == fileFormatTIFF()) {
    saveTIFFData(name);
  } else if (filter == fileFormatTabDelimited()) {
    saveTextData(name, "\t", false);
  } else if (filter == fileFormatTransposedTabDelimited()) {
    saveTextData(name, "\t", true);
  } else if (filter == fileFormatCSV()) {
    saveTextData(name, ", ", false);
  } else if (filter == fileFormatTransposedCSV()) {
    saveTextData(name, ", ", true);
  }
}

template <typename T>
double QcepImageData<T>::sumInRectangle(QRectF rect)
{
  int l = qRound(rect.left());
  int t = qRound(rect.top());
  int r = qRound(rect.right());
  int b = qRound(rect.bottom());

  if (l > r) {
    l = qRound(rect.right());
    r = qRound(rect.left());
  }

  if (b > t) {
    b = qRound(rect.top());
    t = qRound(rect.bottom());
  }

  int np = 0;
  double sum = 0;

  for (int row=b; row<=t; row++) {
    for (int col=l; col<=r; col++) {
      if (m_Mask == NULL || m_Mask->value(col, row)) {
        T val = value(col, row);

        if (val==val) {
          np++;
          sum += val;
        }
      }
    }
  }

  return sum;
}

template <typename T>
double QcepImageData<T>::averageInRectangle(QRectF rect)
{
  int l = qRound(rect.left());
  int t = qRound(rect.top());
  int r = qRound(rect.right());
  int b = qRound(rect.bottom());

  if (l > r) {
    l = qRound(rect.right());
    r = qRound(rect.left());
  }

  if (b > t) {
    b = qRound(rect.top());
    t = qRound(rect.bottom());
  }

  int np = 0;
  double sum = 0;

  for (int row=b; row<=t; row++) {
    for (int col=l; col<=r; col++) {
      if (m_Mask == NULL || m_Mask->value(col, row)) {
        T val = value(col, row);

        if (val==val) {
          np++;
          sum += val;
        }
      }
    }
  }

  return sum/np;
}

template <typename T>
double QcepImageData<T>::minInRectangle(QRectF rect)
{
  int l = qRound(rect.left());
  int t = qRound(rect.top());
  int r = qRound(rect.right());
  int b = qRound(rect.bottom());

  if (l > r) {
    l = qRound(rect.right());
    r = qRound(rect.left());
  }

  if (b > t) {
    b = qRound(rect.top());
    t = qRound(rect.bottom());
  }

  int first  = true;
  double min = 0;

  for (int row=b; row<=t; row++) {
    for (int col=l; col<=r; col++) {
      if (m_Mask == NULL || m_Mask->value(col, row)) {
        T val = value(col, row);

        if (val==val) {
          if (first) {
            min = val;
            first = false;
          } else if (val < min) {
            min = val;
          }
        }
      }
    }
  }

  return min;
}

template <typename T>
double QcepImageData<T>::maxInRectangle(QRectF rect)
{
  int l = qRound(rect.left());
  int t = qRound(rect.top());
  int r = qRound(rect.right());
  int b = qRound(rect.bottom());

  if (l > r) {
    l = qRound(rect.right());
    r = qRound(rect.left());
  }

  if (b > t) {
    b = qRound(rect.top());
    t = qRound(rect.bottom());
  }

  int first  = true;
  double max = 0;

  for (int row=b; row<=t; row++) {
    for (int col=l; col<=r; col++) {
      if (m_Mask == NULL || m_Mask->value(col, row)) {
        T val = value(col, row);

        if (val==val) {
          if (first) {
            max = val;
            first = false;
          } else if (val > max) {
            max = val;
          }
        }
      }
    }
  }

  return max;
}

template <typename T>
double QcepImageData<T>::sumInEllipse(QRectF rect)
{
  QPointF c = rect.center();
  double  a = rect.width()/2;
  double  b = rect.height()/2;

  int bt = qRound(rect.bottom());
  int tp = qRound(rect.top());

  int np = 0;
  double sum = 0;

  for (int row=tp; row<=bt; row++) {
    double y=row - c.y();
    double xx = a*sqrt(1 - pow(y/b,2));
    int x1 = qRound(c.x() - xx);
    int x2 = qRound(c.x() + xx);

    for (int col=x1; col<=x2; col++) {
      if (m_Mask == NULL || m_Mask->value(col, row)) {
        T val = value(col, row);

        if (val==val) {
          np++;
          sum += val;
        }
      }
    }
  }

  return sum;
}

template <typename T>
double QcepImageData<T>::averageInEllipse(QRectF rect)
{
  QPointF c = rect.center();
  double  a = rect.width()/2;
  double  b = rect.height()/2;

  int bt = qRound(rect.bottom());
  int tp = qRound(rect.top());

  int np = 0;
  double sum = 0;

  for (int row=tp; row<=bt; row++) {
    double y=row - c.y();
    double xx = a*sqrt(1 - pow(y/b,2));
    int x1 = qRound(c.x() - xx);
    int x2 = qRound(c.x() + xx);

    for (int col=x1; col<=x2; col++) {
      if (m_Mask == NULL || m_Mask->value(col, row)) {
        T val = value(col, row);

        if (val==val) {
          np++;
          sum += val;
        }
      }
    }
  }

  return sum/np;
}

template <typename T>
double QcepImageData<T>::minInEllipse(QRectF rect)
{
  QPointF c = rect.center();
  double  a = rect.width()/2;
  double  b = rect.height()/2;

  int bt = qRound(rect.bottom());
  int tp = qRound(rect.top());

  int first = true;
  double min = 0;

  for (int row=tp; row<=bt; row++) {
    double y=row - c.y();
    double xx = a*sqrt(1 - pow(y/b,2));
    int x1 = qRound(c.x() - xx);
    int x2 = qRound(c.x() + xx);

    for (int col=x1; col<=x2; col++) {
      if (m_Mask == NULL || m_Mask->value(col, row)) {
        T val = value(col, row);

        if (val==val) {
          if (first) {
            min = val;
            first = false;
          } else if (val < min) {
            min = val;
          }
        }
      }
    }
  }

  return min;
}

template <typename T>
double QcepImageData<T>::maxInEllipse(QRectF rect)
{
  QPointF c = rect.center();
  double  a = rect.width()/2;
  double  b = rect.height()/2;

  int bt = qRound(rect.bottom());
  int tp = qRound(rect.top());

  int first = true;
  double max = 0;

  for (int row=tp; row<=bt; row++) {
    double y=row - c.y();
    double xx = a*sqrt(1 - pow(y/b,2));
    int x1 = qRound(c.x() - xx);
    int x2 = qRound(c.x() + xx);

    for (int col=x1; col<=x2; col++) {
      if (m_Mask == NULL || m_Mask->value(col, row)) {
        T val = value(col, row);

        if (val==val) {
          if (first) {
            max = val;
            first = false;
          } else if (val > max) {
            max = val;
          }
        }
      }
    }
  }

  return max;
}

template <typename T>
double QcepImageData<T>::sumInPeak(QRectF rect)
{
  QPointF c = rect.center();
  double  w = rect.width()/2;
  double  h = rect.height()/2;

  int bt = qRound(rect.bottom());
  int tp = qRound(rect.top());
  int lf = qRound(rect.left());
  int rt = qRound(rect.right());

  int ptp = qRound(c.y() - h/2);
  int pbt = qRound(c.y() + h/2);
  int plf = qRound(c.x() - w/2);
  int prt = qRound(c.x() + w/2);

  int    np    = 0;
  double sum   = 0;
  int    npk   = 0;
  double sumpk = 0;

  for (int row=tp; row<=bt; row++) {
    for (int col=lf; col<=rt; col++) {
      if (m_Mask == NULL || m_Mask->value(col,row)) {
        T val = value(col,row);

        if (val == val) {
          if (ptp <= row && row <= pbt && plf <= col && col <= prt) {
            npk++;
            sumpk += val;
          } else {
            np++;
            sum += val;
          }
        }
      }
    }
  }

  if (np > 0 && npk > 0) {
    double avbk = sum/np;
    double avpk = sumpk/npk;

    return (avpk-avbk)*npk;
  } else {
    return 0;
  }
}

template <typename T>
void QcepImageData<T>::setRegionTo(int regionType, int coordStyle, double x1, double y1, double x2, double y2, double val)
{
  int nRows = get_Height();
  int nCols = get_Width();

  double lf, tp, rt, bt;

  switch (coordStyle) {
  case 0:  //LTRB
    lf = qMin(x1,x2);
    rt = qMax(x1,x2);
    tp = qMin(y1,y2);
    bt = qMax(y1,y2);
    break;

  case 1:  //CxCyWdHt
    lf = x1-qAbs(x2);
    rt = x1+qAbs(x2);
    tp = y1-qAbs(y2);
    bt = y1+qAbs(y2);
    break;

  case 2:  //BlBtBrBb
    lf = x1;
    rt = nCols-x2;
    tp = y2;
    bt = nRows-y2;
    break;
  }

  double cx = (lf + rt)/2.0;
  double cy = (tp + bt)/2.0;
  double a  = (rt - lf)/2.0;
  double b  = (bt - tp)/2.0;

  for (int r=0; r<nRows; r++) {
//    int pct = r*80/nRows;

//    emit progressMade(pct);

    switch (regionType) {
    case 0: // Inside Rectangle
      for (int c=0; c<nCols; c++) {
        if (r >= tp && r <= bt && c >= lf && c <= rt) {
          setImageData(r,c,val);
        }
      }
      break;

    case 1: // Outside Rectangle
      for (int c=0; c<nCols; c++) {
        if (!(r >= tp && r <= bt && c >= lf && c <= rt)) {
          setImageData(r,c,val);
        }
      }
      break;

    case 2: // Inside Ellipse
      {
        double y = r - cy;
        double xx = a*sqrt(1 - pow(y/b,2));

        for (int c=0; c<nCols; c++) {
          if (xx == xx && c >= cx-xx && c <= cx+xx) {
            setImageData(r,c,val);
          }
        }
      }
      break;

    case 3: // Outside Ellipse
      {
        double y = r - cy;
        double xx = a*sqrt(1 - pow(y/b,2));

        for (int c=0; c<nCols; c++) {
          if (xx != xx || c < cx-xx || c > cx+xx) {
            setImageData(r,c,val);
          }
        }
      }
      break;
    }
  }
}

template <typename T>
void QcepImageData<T>::setValueRangeTo(int rangeChoice, double min, double max, double newValue)
{
  int nRows = get_Height();
  int nCols = get_Width();

  for (int r=0; r<nRows; r++) {
//    int pct = r*80/nRows;

//    emit progressMade(10+int(pct));

    for (int c=0; c<nCols; c++) {
      double v = getImageData(c, r);

      switch (rangeChoice) {
      case 0:
        if (v < min || v > max) {
          setImageData(c, r, newValue);
        }
        break;

      case 1:
        if (v >= min && v <= max) {
          setImageData(c, r, newValue);
        }
        break;

      case 2:
        if (v > max) {
          setImageData(c, r, newValue);
        }
        break;

      case 3:
        if (v < min) {
          setImageData(c, r, newValue);
        }
        break;

      case 4:
        if (qIsInf(v)) {
          setImageData(c, r, newValue);
        }
        break;

      case 5:
        if (qIsNaN(v)) {
          setImageData(c, r, newValue);
        }
      }
    }
  }
}

QcepInt16ImageData::QcepInt16ImageData(QString name,
                                       int width,
                                       int height,
                                       qint16 def) :
  QcepImageData<qint16>(name, width, height, def)
{
}

QcepUInt16ImageData::QcepUInt16ImageData(QString name,
                                         int width,
                                         int height,
                                         quint16 def) :
  QcepImageData<quint16>(name, width, height, def)
{
}

QcepUInt32ImageData::QcepUInt32ImageData(QString name,
                                         int width,
                                         int height,
                                         quint32 def) :
  QcepImageData<quint32>(name, width, height, def)
{
}

QcepInt32ImageData::QcepInt32ImageData(QString name,
                                       int width,
                                       int height,
                                       qint32 def) :
  QcepImageData<qint32>(name, width, height, def)
{
}

QcepFloatImageData::QcepFloatImageData(QString name,
                                       int width,
                                       int height,
                                       float def) :
  QcepImageData<float>(name, width, height, def)
{
}

QcepDoubleImageData::QcepDoubleImageData(QString name,
                                         int width,
                                         int height,
                                         double def)  :
  QcepImageData<double>(name, width, height, def)
{
}

void QcepDoubleImageData::add(QcepDoubleImageDataPtr img)
{
  QcepImageData<double>::add<double>(img);
}

void QcepDoubleImageData::add(double val)
{
  QcepImageData<double>::add(val);
}

void QcepDoubleImageData::subtract(QcepDoubleImageDataPtr img)
{
  QcepImageData<double>::subtract<double>(img);
}

void QcepDoubleImageData::subtract(double val)
{
  QcepImageData<double>::subtract(val);
}

void QcepDoubleImageData::multiply(QcepDoubleImageDataPtr img)
{
  QcepImageData<double>::multiply<double>(img);
}

void QcepDoubleImageData::multiply(double val)
{
  QcepImageData<double>::multiply(val);
}

void QcepDoubleImageData::divide(QcepDoubleImageDataPtr img)
{
  QcepImageData<double>::divide<double>(img);
}

void QcepDoubleImageData::divide(double val)
{
  QcepImageData<double>::divide(val);
}

void QcepDoubleImageData::copyFrom(QcepDoubleImageDataPtr img)
{
  QcepImageData<double>::copyFrom<double>(img);
}

void QcepDoubleImageData::copyFrom(QcepUInt16ImageDataPtr img)
{
  QcepImageData<double>::copyFrom<quint16>(img);
}

void QcepDoubleImageData::copyFrom(QcepUInt32ImageDataPtr img)
{
  QcepImageData<double>::copyFrom<quint32>(img);
}

void QcepDoubleImageData::accumulateImage(QcepDoubleImageDataPtr img)
{
  QcepImageData<double>::accumulateImage<double>(img);
}

QcepDoubleImageDataPtr QcepDoubleImageData::duplicate()
{
  QcepDoubleImageDataPtr dup = QcepAllocator::newDoubleImage("dup",
                                                             get_Width(), get_Height(), QcepAllocator::AlwaysAllocate);
  copyImage<double>(dup);

  dup->set_Name(tr("%1.dup").arg(get_Name()));

  return dup;
}

QcepDoubleImageDataPtr QcepDoubleImageData::differentiateH()
{
  QcepDoubleImageDataPtr dup = QcepAllocator::newDoubleImage("dup",
                                                             get_Width(), get_Height(), QcepAllocator::AlwaysAllocate);
  copyImage<double>(dup);

  dup->set_Name(tr("%1.dh").arg(get_Name()));

  int nRows = dup->get_Height();
  int nCols = dup->get_Width();

  for (int y=0; y<nRows; y++) {
    for (int x=0; x<nCols; x++) {
      double diff = getImageData(x, y) - getImageData(x-1, y);

      dup->setImageData(x, y, diff);
    }
  }

  return dup;
}

QcepDoubleImageDataPtr QcepDoubleImageData::differentiateV()
{
  QcepDoubleImageDataPtr dup = QcepAllocator::newDoubleImage("dup",
                                                             get_Width(), get_Height(), QcepAllocator::AlwaysAllocate);
  copyImage<double>(dup);

  dup->set_Name(tr("%1.dv").arg(get_Name()));

  int nRows = dup->get_Height();
  int nCols = dup->get_Width();

  for (int y=0; y<nRows; y++) {
    for (int x=0; x<nCols; x++) {
      double diff = getImageData(x, y) - getImageData(x, y-1);

      dup->setImageData(x, y, diff);
    }
  }

  return dup;
}

template class QcepImageData<quint16>;
template class QcepImageData<qint16>;
template class QcepImageData<quint32>;
template class QcepImageData<qint32>;
template class QcepImageData<double>;
template class QcepImageData<float>;

template void QcepImageData<double>::subtractDark(const QSharedPointer< QcepImageData<quint16> > dark);
template void QcepImageData<double>::subtractDark(const QSharedPointer< QcepImageData<qint16> > dark);
template void QcepImageData<double>::subtractDark(const QSharedPointer< QcepImageData<quint32> > dark);
template void QcepImageData<double>::subtractDark(const QSharedPointer< QcepImageData<qint32> > dark);
template void QcepImageData<double>::subtractDark(const QSharedPointer< QcepImageData<double> > dark);
template void QcepImageData<double>::subtractDark(const QSharedPointer< QcepImageData<float> > dark);

template void QcepImageData<double>::copyFrom<quint32>(QSharedPointer<QcepImageData<quint32> >);
template void QcepImageData<double>::copyFrom<quint16>(QSharedPointer<QcepImageData<quint16> >);
template void QcepImageData<double>::copyFrom<double>(QSharedPointer<QcepImageData<double> >);
//template void QcepImageData<quint32>::copyFrom<quint32>(QSharedPointer<QcepImageData<quint32> >);

template void QcepImageData<double>::accumulateImage<double>(QSharedPointer<QcepImageData<double> >);

template void QcepImageData<double>::add<double>(QSharedPointer<QcepImageData<double> >);
template void QcepImageData<double>::subtract<double>(QSharedPointer<QcepImageData<double> >);
template void QcepImageData<double>::multiply<double>(QSharedPointer<QcepImageData<double> >);
template void QcepImageData<double>::divide<double>(QSharedPointer<QcepImageData<double> >);
