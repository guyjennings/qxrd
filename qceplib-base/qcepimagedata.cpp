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
#include "qcepsettingssaver.h"

#include "tiffio.h"

QAtomicInt allocCount = 0;

QcepImageDataBase::QcepImageDataBase(QcepSettingsSaverWPtr saver, int width, int height, int size, QcepObject *parent)
  : QcepDataObject(saver, tr("image"), size, parent),
    m_Width(saver, this, "width", width, "Image Width"),
    m_Height(saver, this, "height", height, "Image Height"),
    m_DataType(saver, this, "dataType", UndefinedData, "Data Type of Image"),
    m_FileBase(saver, this, "fileBase", "", "File Base of Image"),
    m_Title(saver, this, "title", "", "Title of Image"),
    m_ReadoutMode(saver, this, "readoutMode", 0, "Image Readout Mode"),
    m_ExposureTime(saver, this, "exposureTime", 0, "Image Exposure Time"),
    m_SummedExposures(saver, this, "summedExposures", 0, "Summed Exposures in Image"),
    m_ImageSequenceNumber(saver, this, "imageSequenceNumber", -1, "Image Sequence Number"),
    m_ImageNumber(saver, this, "imageNumber", 0, "Image Number"),
    m_PhaseNumber(saver, this, "phaseNumber", -1, "Image Phase Number"),
    m_NPhases(saver, this, "nPhases", -1, "Number of Image Phases"),
    m_DateTime(saver, this, "dateTime", QDateTime::currentDateTime(), "Image Creation Date and Time"),
    m_TimeStamp(saver, this, "timeStamp", secondsSinceEpoch(), "Image creation time (secs after UNIX epoch)"),
    m_HBinning(saver, this, "hBinning", 1, "Image Horizontal Binning Factor"),
    m_VBinning(saver, this, "vBinning", 1, "Image Vertical Binning Factor"),
    m_CameraGain(saver, this, "cameraGain", 0, "Image Detector Gain"),
    m_Triggered(saver, this, "triggered", 0, "Triggered Flag"),
    m_UserComment1(saver, this,"userComment1","", "User Comment 1"),
    m_UserComment2(saver, this,"userComment2","", "User Comment 2"),
    m_UserComment3(saver, this,"userComment3","", "User Comment 3"),
    m_UserComment4(saver, this,"userComment4","", "User Comment 4"),
    m_Normalization(saver, this, "normalization", QcepDoubleList(), "Normalization Values"),
    m_ExtraInputs(saver, this, "extraInputs", QcepDoubleList(), "Extra Input Values"),
    m_Used(saver, this, "used", true, "Image Used?"),
    m_ImageCounter(allocCount.fetchAndAddOrdered(1)),
    m_Mutex(QMutex::Recursive),
    m_Saver(saver)
{
  set_Type("Image");

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

void QcepImageDataBase::printMessage(QString msg, QDateTime ts) const
{
  QcepSettingsSaverPtr s(m_Saver);

  if (s) {
    s->printMessage(msg, ts);
  } else {
    printf("%s\n", qPrintable(msg));
  }
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
  dest -> set_Type(get_Type());
  dest -> set_Creator(get_Creator());
  dest -> set_Version(get_Version());
  dest -> set_QtVersion(get_QtVersion());
  dest -> set_DataType(get_DataType());
  dest -> set_FileBase(get_FileBase());
  dest -> set_FileName(get_FileName());
  dest -> set_Title(get_Title());
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
  set_Type(src -> get_Type());
  set_Creator(src -> get_Creator());
  set_Version(src -> get_Version());
  set_QtVersion(src->get_QtVersion());
  set_DataType(src -> get_DataType());
  set_FileBase(src -> get_FileBase());
  set_FileName(src -> get_FileName());
  set_Title(src -> get_Title());
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

    QcepProperty::readSettings(this, &settings, "metadata");

    if (settings.contains("metadata/qxrdVersion")) {
      set_Creator("QXRD");
      set_Version(settings.value("metadata/qxrdVersion").toString());
    }
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

    QcepProperty::writeSettings(this, &settings, "metadata");

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

QString QcepImageDataBase::fileFormatFilterString()
{
  return fileFormatTIFF() + ";;" +
      fileFormatTabDelimited() + ";;" +
      fileFormatTransposedTabDelimited() + ";;" +
      fileFormatCSV() + ";;" +
      fileFormatTransposedCSV();
}

QString QcepImageDataBase::fileFormatTIFF()
{
  return "TIFF (*.tif, *.tiff)";
}

QString QcepImageDataBase::fileFormatTabDelimited()
{
  return "Tab delimited (*.txt.*.dat)";
}

QString QcepImageDataBase::fileFormatTransposedTabDelimited()
{
  return "Transposed Tab delimited (*.txt,*.dat)";
}

QString QcepImageDataBase::fileFormatCSV()
{
  return "CSV (*.csv)";
}

QString QcepImageDataBase::fileFormatTransposedCSV()
{
  return "Transposed CSV (*.csv)";
}

double QcepImageDataBase::sumInRectangle(QRectF rect)
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
      np++;
      sum += getImageData(col,row);
    }
  }

  return sum;
}

double QcepImageDataBase::averageInRectangle(QRectF rect)
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
      np++;
      sum += getImageData(col,row);
    }
  }

  return sum/np;
}

double QcepImageDataBase::sumInEllipse(QRectF rect)
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
      np++;
      sum += getImageData(col,row);
    }
  }

  return sum;
}

double QcepImageDataBase::averageInEllipse(QRectF rect)
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
      np++;
      sum += getImageData(col,row);
    }
  }

  return sum/np;
}

double QcepImageDataBase::sumInPeak(QRectF rect)
{
  return 0;
}

template <typename T>
QcepImageData<T>::QcepImageData(QcepSettingsSaverWPtr saver, int width, int height, T def, QcepObject *parent)
  : QcepImageDataBase(saver, width, height, width*height*sizeof(T), parent),
    //    m_Image(width*height, def),
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

  set_Type("Image Data");
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
QSharedPointer< QcepImageData<T> > QcepImageData<T>::newImage(
    QcepSettingsSaverWPtr saver, QString name, int width, int height, QcepObject *parent)
{
  QSharedPointer< QcepImageData<T> > res(new QcepImageData<T>(saver, width, height, 0, parent));

  res->setObjectName(name);

  return res;
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

template <typename T>
void QcepImageData<T>::fill(T val)
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
  QcepImageData<T> temp(QcepSettingsSaverPtr(), get_Width(), get_Height(), 0, this);

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

      set_FileBase(fileBase);
      set_Title(fileBase);
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
      QcepDataObjectPtr p = qdobj->sharedFromThis();

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
}

template <typename T>
void QcepImageData<T>::saveTIFFData(QString name)
{
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

template class QcepImageData<unsigned short>;
template class QcepImageData<short>;
template class QcepImageData<unsigned int>;
template class QcepImageData<int>;
template class QcepImageData<double>;

template void QcepImageData<double>::subtractDark(const QSharedPointer< QcepImageData<unsigned short> > dark);
template void QcepImageData<double>::subtractDark(const QSharedPointer< QcepImageData<short> > dark);
template void QcepImageData<double>::subtractDark(const QSharedPointer< QcepImageData<unsigned int> > dark);
template void QcepImageData<double>::subtractDark(const QSharedPointer< QcepImageData<int> > dark);
template void QcepImageData<double>::subtractDark(const QSharedPointer< QcepImageData<double> > dark);

template void QcepImageData<double>::copyFrom<unsigned int>(QSharedPointer<QcepImageData<unsigned int> >);
template void QcepImageData<double>::copyFrom<unsigned short>(QSharedPointer<QcepImageData<unsigned short> >);
template void QcepImageData<double>::copyFrom<double>(QSharedPointer<QcepImageData<double> >);
template void QcepImageData<unsigned int>::copyFrom<unsigned int>(QSharedPointer<QcepImageData<unsigned int> >);

template void QcepImageData<double>::accumulateImage<double>(QSharedPointer<QcepImageData<double> >);

template void QcepImageData<double>::add<double>(QSharedPointer<QcepImageData<double> >);
template void QcepImageData<double>::subtract<double>(QSharedPointer<QcepImageData<double> >);
template void QcepImageData<double>::multiply<double>(QSharedPointer<QcepImageData<double> >);
template void QcepImageData<double>::divide<double>(QSharedPointer<QcepImageData<double> >);
