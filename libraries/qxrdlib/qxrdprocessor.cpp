#define _CRT_SECURE_NO_WARNINGS

#include "qxrddebug.h"
#include "qxrdprocessor.h"
#include "qcepallocator.h"
#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdexperiment.h"
#include "qxrdprocessorstep.h"
#include "qxrdfilesaver-ptr.h"
#include "qxrdfilesaver.h"
#include "qxrdmaskstack.h"

#include <QFileInfo>
#include <QThread>
#include <QDir>
#include <QPainter>

QxrdProcessor::QxrdProcessor(QString name) :
  QcepDataProcessorBase(name),
  m_DataPath(this,"dataPath", "", "Data Path"),
  m_DarkImagePath(this, "darkImagePath", "", "Dark Images Path"),
  m_BadPixelsPath(this, "badPixelsPath", "", "Bad Pixels Path"),
  m_GainMapPath(this, "gainMapPath", "", "Gain Map Path"),
  m_MaskPath(this, "maskPath", "", "Mask Path"),
  m_ScriptPath(this, "scriptPath", "", "Script Path"),
  m_Average(this,"average",0.0, "Average Value of Acquired Image (per exposure)"),
  m_AverageDark(this,"averageDark",0.0, "Average Value of Dark Image"),
  m_AverageRaw(this,"averageRaw",0.0, "Average Value of Raw Image"),
  m_MaskMinimumValue(this, "maskMinimumValue", 0, "Mask Minimum Value"),
  m_MaskMaximumValue(this, "maskMaximumValue", 20000, "Mask Maximum Value"),
  m_MaskCircleRadius(this, "maskCircleRadius", 10, "Mask Circle Radius"),
  m_MaskSetPixels(this, "maskSetPixels", true, "Mask Set Pixels"),
  m_Data(QcepAllocator::newDoubleImage("data", 2048, 2048, QcepAllocator::WaitTillAvailable)),
  m_Dark(NULL),
  m_BadPixels(NULL),
  m_GainMap(NULL)
{
  m_Mask = QxrdMaskStackPtr(
        new QxrdMaskStack("maskStack"));
}

QxrdProcessor::~QxrdProcessor()
{
}


QxrdExperimentWPtr QxrdProcessor::experiment() const
{
  QxrdExperimentWPtr expt(qSharedPointerDynamicCast<QxrdExperiment>(parentPtr()));

  if (expt == NULL) {
    printMessage("QxrdDataProcessorBase::experiment == NULL");
  }

  return expt;
}

QxrdFileSaverWPtr QxrdProcessor::fileSaver() const
{
  QxrdFileSaverWPtr res;

  QxrdExperimentPtr expt(experiment());

  if (expt) {
    res = expt->fileSaver();
  }

  return res;
}

void QxrdProcessor::readSettings(QSettings *settings)
{
  QcepDataProcessorBase::readSettings(settings);

  if (m_Mask) {
    settings->beginGroup("maskStack");
    m_Mask -> readSettings(settings);
    settings->endGroup();
  }

  int n = settings->beginReadArray("processorSteps");

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);

    QcepObjectPtr obj = QcepObject::readObject(settings);

    QxrdProcessorStepPtr s =
        qSharedPointerDynamicCast<QxrdProcessorStep>(obj);

    if (s) {
      m_ProcessorSteps.append(s);

      QxrdProcessorPtr me =
          qSharedPointerDynamicCast<QxrdProcessor>(sharedFromThis());

      s->initialize(me);
    }
  }

  settings->endArray();
}

void QxrdProcessor::writeSettings(QSettings *settings)
{
  QcepDataProcessorBase::writeSettings(settings);

  if (m_Mask) {
    settings->beginGroup("maskStack");
    m_Mask->writeSettings(settings);
    settings->endGroup();
  }

  settings->beginWriteArray("processorSteps");

  for (int i=0; i<m_ProcessorSteps.count(); i++) {
    settings->setArrayIndex(i);

    QxrdProcessorStepPtr s = m_ProcessorSteps.value(i);

    if (s) {
      s -> writeSettings(settings);
    }
  }

  settings->endArray();
}

void QxrdProcessor::loadData(QString name)
{
  THREAD_CHECK;

  if (qcepDebug(DEBUG_FILES)) {
    printMessage(
          tr("QxrdDataProcessorBase::loadData(%1)").arg(name));
  }

  QcepDoubleImageDataPtr res = QcepAllocator::newDoubleImage("data", 0,0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  if (res && res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();

    int typ = res->get_DataType();

    if ((typ == QcepDoubleImageData::Raw16Data) ||
        (typ == QcepDoubleImageData::Raw32Data))
    {
      subtractDarkImage(res, m_Dark);
    }

    newData(res);

    set_DataPath(res -> get_FileName());

    printMessage(tr("Loaded data from %1").arg(path));
  }
}

void QxrdProcessor::saveData(QString name, int canOverwrite)
{
  THREAD_CHECK;

  QString path = filePathInDataDirectory(name);

  saveNamedImageData(path, data(), QcepMaskDataPtr(), canOverwrite);

  set_DataPath(m_Data -> get_FileName());
}

void QxrdProcessor::clearData()
{
  THREAD_CHECK;

  newData(QcepDoubleImageDataPtr());

  set_DataPath("");
}

QcepImageDataBaseWPtr QxrdProcessor::data() const
{
  return m_Data;
}

void QxrdProcessor::newData(QcepImageDataBaseWPtr image)
{
  THREAD_CHECK;

  m_Data = image;
//  m_Overflow = overflow;

//  QxrdWindowPtr w = m_Window;

//  if (m_Data) {
  QcepDoubleImageDataPtr dimg(qSharedPointerDynamicCast<QcepDoubleImageData>(image));

  if (dimg) {
    emit dataAvailable(dimg);
  } else {
    printMessage("QxrdProcessor::newData != double");
  }
//  }

//  if (w) {
//    w -> newDataAvailable(m_Data, overflow);
//  }

//  if (m_CenterFinder) {
//    m_CenterFinder->setData(m_Data);
//  }

//  QxrdExperimentPtr exp(experiment());

//  if (exp) {
//    QcepDatasetModelPtr ds = exp->dataset();

//    if (ds && image) {
//      ds->append(image->get_Name(), image);
//    }
//  }
}

QString QxrdProcessor::filePathInDataDirectory(QString name) const
{
  return QDir(dataDirectory()).filePath(name);
}

QString QxrdProcessor::dataDirectory() const
{
  QxrdExperimentPtr exp(experiment());

  if (exp) {
    return QDir(exp->get_ExperimentDirectory()).filePath(exp->get_DataDirectory());
  } else {
    return QString();
  }
}

void QxrdProcessor::loadDark(QString name)
{
  THREAD_CHECK;

  if (qcepDebug(DEBUG_FILES)) {
    printMessage(
          tr("QxrdDataProcessorBase::loadDark(%1)").arg(name));
  }

  QcepDoubleImageDataPtr res =
      QcepAllocator::newDoubleImage("dark", 0,0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  if (res && res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QcepDoubleImageData::DarkData);

    newDark(res);

    set_DarkImagePath(res -> get_FileName());

    printMessage(tr("Loaded Dark Image from %1").arg(path));
  } else {
    printMessage(tr("loadDark(%1) failed").arg(name));
  }
}

void QxrdProcessor::saveDark(QString name, int canOverwrite)
{
  THREAD_CHECK;

  QString path = filePathInDataDirectory(name);

  if (m_Dark) {
    saveNamedImageData(path, m_Dark, QcepMaskDataPtr(), canOverwrite);

    set_DarkImagePath(m_Dark -> get_FileName());
  }
}

void QxrdProcessor::clearDark()
{
  newDark(QcepDoubleImageDataPtr());

  set_DarkImagePath("");
}

QcepDoubleImageDataWPtr QxrdProcessor::dark() const
{
  return m_Dark;
}

void QxrdProcessor::newDark(QcepDoubleImageDataWPtr imageW)
{
  m_Dark = imageW;

  if (m_Dark) {
    set_DarkImagePath(m_Dark->get_FileName());

    int height = m_Dark->get_Height();
    int width  = m_Dark->get_Width();
    int ndrk = m_Dark -> get_SummedExposures();
    int npixels = width*height;
    if (ndrk <= 0) ndrk = 1;

    double *dk     = m_Dark->data();
    double avgdark = 0;

    for (int i=0; i<npixels; i++) {
      avgdark  += dk[i];
    }
    set_AverageDark(avgdark/npixels/ndrk);
  } else {
    set_DarkImagePath("");
    set_AverageDark(0);
  }
}

void QxrdProcessor::newDarkInt16(QcepUInt16ImageDataWPtr imageW)
{
  QcepUInt16ImageDataPtr image(imageW);

  if (image) {
    QcepDoubleImageDataPtr d
        = QcepAllocator::newDoubleImage("dark",
                                        image->get_Width(),
                                        image->get_Height(),
                                        QcepAllocator::NullIfNotAvailable);

    if (d) {
      d -> copyFrom(image);
      newDark(d);
    }
  } else {
    newDark(QcepDoubleImageDataPtr(NULL));
  }
}

void QxrdProcessor::newDarkInt32(QcepUInt32ImageDataWPtr imageW)
{
  QcepUInt32ImageDataPtr image(imageW);

  if (image) {
    QcepDoubleImageDataPtr d
        = QcepAllocator::newDoubleImage("dark",
                                        image->get_Width(),
                                        image->get_Height(),
                                        QcepAllocator::NullIfNotAvailable);

    if (d) {
      d -> copyFrom(image);
      newDark(d);
    }
    //  set_DarkImagePath(m_DarkFrame -> get_FileName());
  } else {
    newDark(QcepDoubleImageDataPtr(NULL));
  }
}

void QxrdProcessor::loadMask(QString name)
{
  THREAD_CHECK;

  if (qcepDebug(DEBUG_FILES)) {
    printMessage(tr("QxrdDataProcessorBase::loadMask(%1)").arg(name));
  }

  QcepMaskDataPtr res =
      QcepAllocator::newMask("mask", 0,0, 0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  if (res && res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QcepMaskData::MaskData);

    //    res -> copyMaskTo(m_Mask);

    newMask(res);

    set_MaskPath(res -> get_FileName());

    printMessage(tr("Loaded Mask from %1").arg(path));
  } else {
    printMessage(tr("loadMask(%1) failed").arg(name));
  }
}

void QxrdProcessor::saveMask(QString name, int canOverwrite)
{
  THREAD_CHECK;

  if (m_Mask) {
    QString path = filePathInDataDirectory(name);

    QcepMaskDataPtr m(m_Mask->first());

    if (m) {
      saveNamedMaskData(path, m, canOverwrite);

      set_MaskPath(m -> get_FileName());
    }
  }
}

void QxrdProcessor::clearMask()
{
  THREAD_CHECK;

  if (m_Mask) {
    m_Mask -> clear();
  }

  set_MaskPath("");
}

QcepMaskDataWPtr QxrdProcessor::mask() const
{
  QcepMaskDataWPtr res;

  if (m_Mask) {
    res = m_Mask->first();
  }

  return res;
}

void QxrdProcessor::newMask(QcepMaskDataWPtr mask)
{
  if (m_Mask) {
    m_Mask->push_front(mask);

    emit maskAvailable(mask);
  }

//  QxrdWindowPtr w = m_Window;

//  if (w) {
//    w -> newMaskAvailable(mask());
//  }
}

void QxrdProcessor::loadBadPixels(QString name)
{
  if (qcepDebug(DEBUG_FILES)) {
    printMessage(tr("QxrdDataProcessorBase::loadBadPixels(%1)").arg(name));
  }

  QcepDoubleImageDataPtr res = QcepAllocator::newDoubleImage("bad", 0,0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  if (res && res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QcepDoubleImageData::BadPixelsData);

    newBadPixelsImage(res);

    set_BadPixelsPath(res -> get_FileName());
  }
}

void QxrdProcessor::saveBadPixels(QString name, int canOverwrite)
{
  QString path = filePathInDataDirectory(name);

  if (m_BadPixels) {
    saveNamedImageData(path, m_BadPixels, QcepMaskDataPtr(), canOverwrite);

    set_BadPixelsPath(m_BadPixels -> get_FileName());
  }
}

void QxrdProcessor::clearBadPixels()
{
  newBadPixelsImage(QcepDoubleImageDataPtr());

  set_BadPixelsPath("");
}

QcepDoubleImageDataWPtr QxrdProcessor::badPixels() const
{
  return m_BadPixels;
}

void QxrdProcessor::newBadPixelsImage(QcepDoubleImageDataWPtr image)
{
  m_BadPixels = image;

  if (m_BadPixels) {
    set_BadPixelsPath(m_BadPixels->get_FileName());
  } else {
    set_BadPixelsPath("");
  }
}

void QxrdProcessor::loadGainMap(QString name)
{
  if (qcepDebug(DEBUG_FILES)) {
    printMessage(tr("QxrdDataProcessorBase::loadGainMap(%1)").arg(name));
  }

  QcepDoubleImageDataPtr res = QcepAllocator::newDoubleImage("gain", 0,0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  if (res && res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QcepDoubleImageData::GainData);
    res -> setDefaultValue(1.0);

    newGainMapImage(res);

    set_GainMapPath(res -> get_FileName());
  }
}

void QxrdProcessor::saveGainMap(QString name, int canOverwrite)
{
  QString path = filePathInDataDirectory(name);

  if (m_GainMap) {
    saveNamedImageData(path, m_GainMap, QcepMaskDataPtr(), canOverwrite);

    set_GainMapPath(m_GainMap -> get_FileName());
  }
}

void QxrdProcessor::clearGainMap()
{
  newGainMapImage(QcepDoubleImageDataPtr());

  set_GainMapPath("");
}

QcepDoubleImageDataWPtr QxrdProcessor::gainMap() const
{
  return m_GainMap;
}


void QxrdProcessor::newGainMapImage(QcepDoubleImageDataWPtr image)
{
  m_GainMap = image;

  if (m_GainMap) {
    set_GainMapPath(m_GainMap->get_FileName());
  } else {
    set_GainMapPath("");
  }
}

QcepDoubleImageDataWPtr QxrdProcessor::liveData() const
{
  return m_LiveData;
}

void QxrdProcessor::newLiveData(QcepDoubleImageDataWPtr image)
{
  m_LiveData = image;

  emit liveDataAvailable(m_LiveData);
}

QcepMaskDataWPtr QxrdProcessor::overflow() const
{
  return m_Overflow;
}

void QxrdProcessor::newOverflow(QcepMaskDataWPtr ovf)
{
  m_Overflow = ovf;

  emit overflowAvailable(ovf);
}

void QxrdProcessor::loadDefaultImages()
{
  QString fileName = get_MaskPath();
  QFileInfo fileInfo(fileName);

  if (fileInfo.exists() && fileInfo.isFile()) {
    loadMask(fileName);
  }

  fileName = get_DarkImagePath();
  fileInfo.setFile(fileName);

  if (fileInfo.exists() && fileInfo.isFile()) {
    loadDark(fileName);
  }

  fileName = get_BadPixelsPath();
  fileInfo.setFile(fileName);

  if (fileInfo.exists() && fileInfo.isFile()) {
    loadBadPixels(fileName);
  }

  fileName = get_GainMapPath();
  fileInfo.setFile(fileName);

  if (fileInfo.exists() && fileInfo.isFile()) {
    loadGainMap(fileName);
  }
}

void QxrdProcessor::subtractDarkImage(QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark)
{
  //  if (get_PerformDarkSubtraction()) {
  if (dark && image) {
    if (dark->get_ExposureTime() != image->get_ExposureTime()) {
      printMessage("Exposure times of acquired data and dark image are different, skipping");
      return;
    }

    if (dark->get_Width() != image->get_Width() ||
        dark->get_Height() != image->get_Height()) {
      printMessage("Dimensions of acquired data and dark image are different, skipping");
      return;
    }

    if (dark->get_CameraGain() != image->get_CameraGain()) {
      printMessage("Gains of acquired data and dark image are different, skipping");
      return;
    }

//    if (!(image->get_DataType() == QcepDoubleImageData::Raw16Data ||
//          image->get_DataType() == QcepDoubleImageData::Raw32Data)) {
//      printMessage("Acquired data is not a raw image, skipping background subtraction");
//      return;
//    }

//    QcepMutexLocker lock1(__FILE__, __LINE__, dark->mutex());
//    QcepMutexLocker lock2(__FILE__, __LINE__, image->mutex());

    int height = image->get_Height();
    int width  = image->get_Width();
    int nres = image-> get_SummedExposures();
    int ndrk = dark -> get_SummedExposures();
    int npixels = width*height;

    if (nres <= 0) nres = 1;

    double ratio = ((double) nres)/((double) ndrk);

    //      printf("Dark subtraction nres=%d, ndrk=%d, npixels=%d, ratio=%g\n",
    //             nres, ndrk, npixels, ratio);

    double *result = image->data();
    double *dk     = dark->data();
    double avgraw  = 0;
    //      double avgdark = 0;

    for (int i=0; i<npixels; i++) {
      //        avgdark  += dk[i];
      avgraw   += result[i];
      result[i] = result[i]-ratio*dk[i];
    }

    //      set_AverageDark(avgdark/npixels/ndrk);
    set_AverageRaw(avgraw/npixels/nres);
    set_Average(get_AverageRaw() - get_AverageDark());

    image -> set_DataType(QcepDoubleImageData::SubtractedData);
  }
  //  }
}

void QxrdProcessor::unsubtractDarkImage(QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark)
{
  //  if (get_PerformDarkSubtraction()) {
  if (dark && image) {
    if (dark->get_ExposureTime() != image->get_ExposureTime()) {
      printMessage("Exposure times of acquired data and dark image are different, skipping");
      return;
    }

    if (dark->get_Width() != image->get_Width() ||
        dark->get_Height() != image->get_Height()) {
      printMessage("Dimensions of acquired data and dark image are different, skipping");
      return;
    }

    if (dark->get_CameraGain() != image->get_CameraGain()) {
      printMessage("Gains of acquired data and dark image are different, skipping");
      return;
    }

//    if (!(image->get_DataType() == QcepDoubleImageData::Raw16Data ||
//          image->get_DataType() == QcepDoubleImageData::Raw32Data)) {
//      printMessage("Acquired data is not a raw image, skipping background subtraction");
//      return;
//    }

//    QcepMutexLocker lock1(__FILE__, __LINE__, dark->mutex());
//    QcepMutexLocker lock2(__FILE__, __LINE__, image->mutex());

    int height = image->get_Height();
    int width  = image->get_Width();
    int nres = image-> get_SummedExposures();
    int ndrk = dark -> get_SummedExposures();
    int npixels = width*height;

    if (nres <= 0) nres = 1;

    double ratio = ((double) nres)/((double) ndrk);

    //      printf("Dark subtraction nres=%d, ndrk=%d, npixels=%d, ratio=%g\n",
    //             nres, ndrk, npixels, ratio);

    double *result = image->data();
    double *dk     = dark->data();
    double avgraw  = 0;
    //      double avgdark = 0;

    for (int i=0; i<npixels; i++) {
      //        avgdark  += dk[i];
      avgraw   += result[i];
      result[i] = result[i]+ratio*dk[i];
    }

    //      set_AverageDark(avgdark/npixels/ndrk);
    set_AverageRaw(avgraw/npixels/nres);
    set_Average(get_AverageRaw() - get_AverageDark());

    image -> set_DataType(QcepDoubleImageData::SubtractedData);
  }
  //  }
}

void QxrdProcessor::saveNamedImageData(QString name, QcepImageDataBasePtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveImageData(name, image, overflow, canOverwrite);
  }
}
void QxrdProcessor::saveNamedDoubleImageData(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveDoubleData(name, image, overflow, canOverwrite);
  }
}

void QxrdProcessor::saveNamedUInt16ImageData(QString name, QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveInt16Data(name, image, overflow, canOverwrite);
  }
}

void QxrdProcessor::saveNamedRawImageData(QString name, QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveRaw16Data(name, image, overflow, canOverwrite);
  }
}

void QxrdProcessor::saveNamedUInt32ImageData(QString name, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveInt32Data(name, image, overflow, canOverwrite);
  }
}

void QxrdProcessor::saveNamedRawImageData(QString name, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveRaw32Data(name, image, overflow, canOverwrite);
  }
}

void QxrdProcessor::saveNamedMaskData(QString name, QcepMaskDataPtr image, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveMaskData(name, image, canOverwrite);
  }
}

void QxrdProcessor::saveNamedImageDataAsText(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveTextData(name, image, overflow, canOverwrite);
  }
}

int QxrdProcessor::newMaskWidth() const
{
  QcepImageDataBasePtr   d(data());
  QcepMaskDataPtr        m(mask());

  int w=0;

  if (d) {
    w = d->get_Width();
  } else if (m) {
    w = m->get_Width();
  }

  return w;
}

int QxrdProcessor::newMaskHeight() const
{
  QcepImageDataBasePtr   d(data());
  QcepMaskDataPtr        m(mask());

  int h=0;

  if (d) {
    h = d->get_Height();
  } else if (m) {
    h = m->get_Height();
  }

  return h;
}

void QxrdProcessor::newEmptyMask()
{

  QcepMaskDataPtr m = QcepAllocator::newMask("mask",
                                             newMaskWidth(), newMaskHeight(), 1,
                                             QcepAllocator::NullIfNotAvailable);

  if (m && m_Mask) {
    m_Mask -> push_front(m);

    printMessage(tr("new mask, %1 on stack").arg(m_Mask -> maskCount()));

    newMask(m);
  }
}

void QxrdProcessor::duplicateMask()
{
  QcepMaskDataPtr m =  QcepAllocator::newMask("mask",
                                              newMaskWidth(), newMaskHeight(), 1,
                                              QcepAllocator::NullIfNotAvailable);

  if (m_Mask) {
    QcepMaskDataPtr m1 = mask();

    if (m1) {
      m1->copyMaskTo(m);
    }

    m_Mask -> push_front(m);

    printMessage(tr("dup mask, %1 on stack").arg(m_Mask -> maskCount()));

    newMask(m);
  }
}

void QxrdProcessor::showMaskRange()
{
  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  showMaskRange(min, max);
}

void QxrdProcessor::showMaskRange(double min, double max)
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m_Data && m) {
    m -> showMaskRange(m_Data, min, max);
  }
}

void QxrdProcessor::hideMaskAll()
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    m -> hideMaskAll();
  }
}

void QxrdProcessor::showMaskAll()
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    m -> showMaskAll();
  }
}

void QxrdProcessor::hideMaskRange()
{
  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  hideMaskRange(min, max);
}

void QxrdProcessor::hideMaskRange(double min, double max)
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m_Data && m) {
    m -> hideMaskRange(m_Data, min, max);
  }
}

void QxrdProcessor::invertMask()
{
  QcepMaskDataPtr m(mask());

  if (m) {
    m -> invertMask();
  }
}

void QxrdProcessor::growMask()
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    m -> growMask();
  }
}

void QxrdProcessor::shrinkMask()
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    m -> shrinkMask();
  }
}

void QxrdProcessor::maskCircle(QRectF rect)
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    if ((rect.left() == rect.right()) && (rect.bottom() == rect.top())) {
      m -> maskCircle(rect.left(), rect.top(), get_MaskCircleRadius(), get_MaskSetPixels());
    } else {
      double cx = rect.center().x();
      double cy = rect.center().y();
      double rad = rect.width()/2;

      m -> maskCircle(cx, cy, rad, get_MaskSetPixels());
    }
  }
}

void QxrdProcessor::maskPolygon(QVector<QPointF> poly)
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    //  printf("QxrdDataProcessorBase::maskPolygon(%d points ...)\n", poly.size());

    int nRows = m -> get_Height();
    int nCols = m -> get_Width();

    QImage polyImage(nCols, nRows, QImage::Format_RGB32);
    QPainter polyPainter(&polyImage);
    QPolygonF polygon;

    foreach(QPointF pt, poly) {
      polygon.append(pt);
    }

    polyPainter.setPen(Qt::white);
    polyPainter.fillRect(0,0,nCols,nRows,Qt::black);
    polyPainter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    polyPainter.drawPolygon(poly);

    bool newval = get_MaskSetPixels();

    for (int j=0; j<nRows; j++) {
      for (int i=0; i<nCols; i++) {
        if (qGray(polyImage.pixel(i,j))) {
          m -> setMaskValue(i, j, newval);
        }
      }
    }
  }
}

void QxrdProcessor::createMaskIfNeeded()
{
  if (m_Mask == NULL) {
    m_Mask = QxrdMaskStackPtr(new QxrdMaskStack("maskStack"));
  }

  if (m_Mask && m_Mask -> isEmpty()) {
    newEmptyMask();
  }
}

