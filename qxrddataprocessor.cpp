/******************************************************************
*
*  $Id: qxrddataprocessor.cpp,v 1.56 2009/09/28 22:06:55 jennings Exp $
*
*******************************************************************/

#include "qxrddataprocessor.h"
#include <QtConcurrentRun>
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdimagedata.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrator.h"

#include "tiffio.h"

#include <QTime>
#include <QPainter>
#include <math.h>

QxrdDataProcessor::QxrdDataProcessor
    (QxrdWindow *win, QxrdAcquisition *acq, QObject *parent)
  : QObject(parent),
    m_OutputDirectory(this,"outputDirectory", ""),
    m_DataPath(this,"dataPath", ""),
    m_DarkImagePath(this, "darkImagePath", ""),
    m_BadPixelsPath(this, "badPixelsPath", ""),
    m_GainMapPath(this, "gainMapPath", ""),
    m_MaskPath(this, "maskPath", ""),
    m_LogFilePath(this, "logFilePath", "qxrd.log"),
    m_PerformDarkSubtraction(this, "performDarkSubtraction", true),
    m_SaveRawImages(this, "saveRawImages", true),
    m_PerformBadPixels(this, "performBadPixels", true),
    m_PerformGainCorrection(this, "performGainCorrection", true),
    m_SaveSubtracted(this, "saveSubtracted", true),
    m_PerformIntegration(this, "performIntegration", true),
    m_DisplayIntegratedData(this, "displayIntegratedData", true),
    m_SaveIntegratedData(this, "saveIntegratedData", true),
    m_PerformDarkSubtractionTime(this, "performDarkSubtractionTime", 0.01),
    m_PerformBadPixelsTime(this, "performBadPixelsTime", 0.01),
    m_PerformGainCorrectionTime(this, "performGainCorrectionTime", 0.01),
    m_SaveSubtractedTime(this, "saveSubractedTime", 0.1),
    m_PerformIntegrationTime(this, "performIntegrationTime", 0.05),
    m_DisplayIntegratedDataTime(this, "displayIntegratedDataTime", 0.2),
    m_SaveIntegratedDataTime(this, "saveIntegratedDataTime", 0.01),
    m_EstimatedProcessingTime(this, "estimatedProcessingTime", 0.1),
    m_AveragingRatio(this, "averagingRatio", 0.1),
    m_FileName(this,"fileName",""),
    m_MaskMinimumValue(this, "maskMinimumValue", 0),
    m_MaskMaximumValue(this, "maskMaximumValue", 20000),
    m_MaskCircleRadius(this, "maskCircleRadius", 10),
    m_MaskSetPixels(this, "maskSetPixels", true),
    m_CompressImages(this, "compressImages", false),
    m_Mutex(QMutex::Recursive),
    m_LogFileMutex(QMutex::Recursive),
    m_Window(win),
    m_Acquisition(acq),
    m_DarkUsage(QReadWriteLock::Recursive),
    m_Data(new QxrdDoubleImageData(2048,2048)),
    m_DarkFrame(NULL),
    m_BadPixels(NULL),
    m_GainMap(NULL),
    m_Mask(new QxrdMaskData(2048, 2048)),
    m_AcquiredCount(0),
    m_ProcessedCount(0),
    m_CenterFinder(NULL),
    m_Integrator(NULL),
    m_LogFile(NULL),
    SOURCE_IDENT("$Id: qxrddataprocessor.cpp,v 1.56 2009/09/28 22:06:55 jennings Exp $")
{
  m_CenterFinder = new QxrdCenterFinder(this);
  m_Integrator   = new QxrdIntegrator(this, this);

//  m_DarkImagePath.setDebug(10);
}

void QxrdDataProcessor::setAcquisition(QxrdAcquisition*acq)
{
  m_Acquisition = acq;

  connect(m_Acquisition, SIGNAL(acquiredInt16ImageAvailable(QxrdInt16ImageData*)),
          this, SLOT(onAcquiredInt16ImageAvailable(QxrdInt16ImageData*)));
  connect(m_Acquisition, SIGNAL(acquiredInt32ImageAvailable(QxrdInt32ImageData*)),
          this, SLOT(onAcquiredInt32ImageAvailable(QxrdInt32ImageData*)));

  connect(prop_SaveRawImages(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformDarkSubtraction(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformBadPixels(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformGainCorrection(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveSubtracted(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformIntegration(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_DisplayIntegratedData(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveIntegratedData(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformDarkSubtractionTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformBadPixelsTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformGainCorrectionTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveSubtractedTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformIntegrationTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_DisplayIntegratedDataTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveIntegratedDataTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(m_Acquisition -> prop_SummedExposures(), SIGNAL(changedValue(int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(m_Acquisition -> prop_Raw16SaveTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(m_Acquisition -> prop_Raw32SaveTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
}

void QxrdDataProcessor::setWindow(QxrdWindow *win)
{
  m_Window = win;
}

void QxrdDataProcessor::writeSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

  m_CenterFinder -> writeSettings(settings, section+"/centerfinder");
  m_Integrator   -> writeSettings(settings, section+"/integrator");
}

void QxrdDataProcessor::readSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  m_CenterFinder -> readSettings(settings, section+"/centerfinder");
  m_Integrator   -> readSettings(settings, section+"/integrator");

  newLogFile(get_LogFilePath());
}

void QxrdDataProcessor::onAcquiredInt16ImageAvailable(QxrdInt16ImageData *image)
{
  QMutexLocker lock(&m_Mutex);

  int navail = decrementAcquiredCount();

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      m_DarkUsage.lockForRead();
      m_Processing.lockForRead();
      processAcquiredInt16Image(image);
    } else {
      QWriteLocker wl(&m_DarkUsage);

      saveNamedImageData(image->get_FileName(), image);

      set_DarkImagePath(image->get_FileName());

      emit printMessage(tr("Saved dark image \"%1\"").arg(image->get_FileName()));

      newDarkImage(image);

      returnInt16ImageToPool(image);
    }
  }
}

void QxrdDataProcessor::onAcquiredInt32ImageAvailable(QxrdInt32ImageData *image)
{
  QMutexLocker lock(&m_Mutex);

  int navail = decrementAcquiredCount();

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      m_DarkUsage.lockForRead();
      m_Processing.lockForRead();
      processAcquiredInt32Image(image);
    } else {
      QWriteLocker wl(&m_DarkUsage);

      saveNamedImageData(image->get_FileName(), image);

      set_DarkImagePath(image->get_FileName());

      emit printMessage(tr("Saved dark image \"%1\"").arg(image->get_FileName()));

      newDarkImage(image);

      returnInt32ImageToPool(image);
    }
  }
}

QxrdDoubleImageData *QxrdDataProcessor::takeNextFreeImage()
{
  QMutexLocker lock(&m_Mutex);

  if (m_FreeImages.size() == 0) {
//    printf("Allocate new image\n");
    return new QxrdDoubleImageData(2048, 2048);
  } else {
    return m_FreeImages.dequeue();
  }
}

void QxrdDataProcessor::returnImageToPool(QxrdDoubleImageData *img)
{
  QMutexLocker lock(&m_Mutex);

  m_FreeImages.enqueue(img);
}

void QxrdDataProcessor::returnInt16ImageToPool(QxrdInt16ImageData *img)
{
  QMutexLocker lock(&m_Mutex);

  m_Acquisition -> returnImageToPool(img);
}

void QxrdDataProcessor::returnInt32ImageToPool(QxrdInt32ImageData *img)
{
  QMutexLocker lock(&m_Mutex);

  m_Acquisition -> returnImageToPool(img);
}

void QxrdDataProcessor::newData(QxrdDoubleImageData *image)
{
  QMutexLocker lock(&m_Mutex);

  if (m_Data != image) {
//    image -> copyMask(m_Data);

    if (m_Data) {
      returnImageToPool(m_Data);
    }

    m_Data = image;
  }

  set_FileName(image->get_FileName());

  incrementProcessedCount();

  m_Window -> newDataAvailable(m_Data);
}

void QxrdDataProcessor::newDarkImage(QxrdDoubleImageData *image)
{
  QMutexLocker lock(&m_Mutex);

  if (m_DarkFrame != image) {
    if (m_DarkFrame) {
      returnImageToPool(m_DarkFrame);
    }

    m_DarkFrame = image;
  }

  set_DarkImagePath(image->get_FileName());
}

void QxrdDataProcessor::newDarkImage(QxrdInt16ImageData *image)
{
  QMutexLocker lock(&m_Mutex);

  if (m_DarkFrame == NULL) {
    m_DarkFrame = new QxrdDoubleImageData();
  }

  image -> copyImage(m_DarkFrame);

  set_DarkImagePath(m_DarkFrame -> get_FileName());
}

void QxrdDataProcessor::newDarkImage(QxrdInt32ImageData *image)
{
  QMutexLocker lock(&m_Mutex);

  if (m_DarkFrame == NULL) {
    m_DarkFrame = new QxrdDoubleImageData();
  }

  image -> copyImage(m_DarkFrame);

  set_DarkImagePath(m_DarkFrame -> get_FileName());
}

void QxrdDataProcessor::newBadPixelsImage(QxrdDoubleImageData *image)
{
  QMutexLocker lock(&m_Mutex);

  if (m_BadPixels != image) {
    if (m_BadPixels) {
      returnImageToPool(m_BadPixels);
    }

    m_BadPixels = image;
  }

  set_BadPixelsPath(image->get_FileName());
}

void QxrdDataProcessor::newGainMapImage(QxrdDoubleImageData *image)
{
  QMutexLocker lock(&m_Mutex);

  if (m_GainMap != image) {
    if (m_GainMap) {
      returnImageToPool(m_GainMap);
    }

    m_GainMap = image;
  }

  set_GainMapPath(image->get_FileName());
}

void QxrdDataProcessor::newMask()
{
  m_Window -> newMaskAvailable(m_Mask);
}

void QxrdDataProcessor::loadDefaultImages()
{
  QMutexLocker lock(&m_Mutex);

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

void QxrdDataProcessor::loadData(QString name)
{
  QMutexLocker lock(&m_Mutex);

//  printf("QxrdDataProcessor::loadData(%s)\n", qPrintable(name));

  QxrdDoubleImageData* res = takeNextFreeImage();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();

    newData(res);

    set_DataPath(res -> get_FileName());
  } else {
    returnImageToPool(res);
  }
}

void QxrdDataProcessor::saveData(QString name, int canOverwrite)
{
  QMutexLocker lock(&m_Mutex);

  if (saveNamedImageData(name, m_Data, canOverwrite)) {
    set_DataPath(m_Data -> get_FileName());
  }
}

void QxrdDataProcessor::loadDark(QString name)
{
  QMutexLocker lock(&m_Mutex);

//  printf("QxrdDataProcessor::loadDark(%s)\n", qPrintable(name));

  QxrdDoubleImageData* res = takeNextFreeImage();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QxrdDoubleImageData::DarkData);

    newDarkImage(res);

    set_DarkImagePath(res -> get_FileName());
  } else {
    returnImageToPool(res);
  }
}

void QxrdDataProcessor::saveDark(QString name, int canOverwrite)
{
  QMutexLocker lock(&m_Mutex);

  if (saveNamedImageData(name, m_DarkFrame, canOverwrite)) {
    set_DarkImagePath(m_DarkFrame -> get_FileName());
  }
}

void QxrdDataProcessor::loadBadPixels(QString name)
{
  QMutexLocker lock(&m_Mutex);

//  printf("QxrdDataProcessor::loadDark(%s)\n", qPrintable(name));

  QxrdDoubleImageData* res = takeNextFreeImage();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QxrdDoubleImageData::BadPixelsData);

    newBadPixelsImage(res);

    set_BadPixelsPath(res -> get_FileName());
  } else {
    returnImageToPool(res);
  }
}

void QxrdDataProcessor::saveBadPixels(QString name, int canOverwrite)
{
  QMutexLocker lock(&m_Mutex);

  if (saveNamedImageData(name, m_BadPixels, canOverwrite)) {
    set_BadPixelsPath(m_BadPixels -> get_FileName());
  }
}

void QxrdDataProcessor::loadGainMap(QString name)
{
  QMutexLocker lock(&m_Mutex);

//  printf("QxrdDataProcessor::loadGainMap(%s)\n", qPrintable(name));

  QxrdDoubleImageData* res = takeNextFreeImage();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QxrdDoubleImageData::GainData);

    newGainMapImage(res);

    set_GainMapPath(res -> get_FileName());
  } else {
    returnImageToPool(res);
  }
}

void QxrdDataProcessor::saveGainMap(QString name, int canOverwrite)
{
  QMutexLocker lock(&m_Mutex);

  if (saveNamedImageData(name, m_GainMap, canOverwrite)) {
    set_GainMapPath(m_GainMap -> get_FileName());
  }
}

void QxrdDataProcessor::loadMask(QString name)
{
  QMutexLocker lock(&m_Mutex);

//  printf("QxrdDataProcessor::loadData(%s)\n", qPrintable(name));

  QxrdMaskData* res = new QxrdMaskData();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QxrdMaskData::MaskData);

    res -> copyMask(m_Mask);

    newMask();

    set_MaskPath(m_Mask -> get_FileName());
  }

  delete res;
}

void QxrdDataProcessor::saveMask(QString name, int canOverwrite)
{
  QMutexLocker lock(&m_Mutex);

  if (saveNamedMaskData(name, m_Mask, canOverwrite)) {
    set_MaskPath(m_Mask -> get_FileName());
  }
}

#define TIFFCHECK(a) if (res && ((a)==0)) { res = 0; }

bool QxrdDataProcessor::saveNamedImageData(QString name, QxrdDoubleImageData *image, int canOverwrite)
{
  QMutexLocker lock(&m_Mutex);

//  emit printMessage(tr("Saved \"%1\")").arg(name));

  QMutexLocker lockr(image->mutex());

  int nrows = image -> get_Height();
  int ncols = image -> get_Width();

  if (canOverwrite == NoOverwrite) {
    name = uniqueFileName(name);
  }

  TIFF* tif = TIFFOpen(qPrintable(name),"w");
  int res = 1;

  if (tif) {
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));

    if (get_CompressImages()) {
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW));
    } else {
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE));
    }

    TIFFCHECK(TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP));

    image -> setTiffMetaData(tif);

    QVector<float> buffvec(ncols);
    float* buffer = buffvec.data();

    for (int y=0; y<nrows; y++) {
      for (int x=0; x<ncols; x++) {
        buffer[x] = image->value(x,y);
      }

      TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
    }

    TIFFClose(tif);

    image -> set_FileName(name);

    image -> saveMetaData();
  }

  return res;
}

bool QxrdDataProcessor::saveNamedImageData(QString name, QxrdInt16ImageData *image, int canOverwrite)
{
  QMutexLocker lock(&m_Mutex);

  bool res = saveNamedRawImageData(name, image, canOverwrite);

  if (res) {
    image -> set_FileName(name);
  }

  return res;
}

bool QxrdDataProcessor::saveNamedRawImageData(QString name, QxrdInt16ImageData *image, int canOverwrite)
{
  QMutexLocker lock(&m_Mutex);

//  emit printMessage(tr("Saved \"%1\")").arg(name));

  QMutexLocker lockr(image->mutex());

  int nrows = image -> get_Height();
  int ncols = image -> get_Width();

  if (canOverwrite == NoOverwrite) {
    name = uniqueFileName(name);
  }

  TIFF* tif = TIFFOpen(qPrintable(name),"w");
  int res = 1;

  if (tif) {

    TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));

    if (get_CompressImages()) {
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW));
    } else {
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE));
    }

    TIFFCHECK(TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT));

    image -> setTiffMetaData(tif);

    QVector<quint16> buffvec(ncols);
    quint16* buffer = buffvec.data();

    for (int y=0; y<nrows; y++) {
      for (int x=0; x<ncols; x++) {
        buffer[x] = image->value(x,y);
      }

      TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
    }

    TIFFClose(tif);

    image -> saveMetaData(name);
  }

  return res;
}

bool QxrdDataProcessor::saveNamedImageData(QString name, QxrdInt32ImageData *image, int canOverwrite)
{
  QMutexLocker lock(&m_Mutex);

  bool res = saveNamedRawImageData(name, image, canOverwrite);

  if (res) {
    image -> set_FileName(name);
  }

  return res;
}

bool QxrdDataProcessor::saveNamedRawImageData(QString name, QxrdInt32ImageData *image, int canOverwrite)
{
  QMutexLocker lock(&m_Mutex);

//  emit printMessage(tr("Saved \"%1\")").arg(name));

  QMutexLocker lockr(image->mutex());

  int nrows = image -> get_Height();
  int ncols = image -> get_Width();

  if (canOverwrite == NoOverwrite) {
    name = uniqueFileName(name);
  }

  TIFF* tif = TIFFOpen(qPrintable(name),"w");
  int res = 1;

  if (tif) {

    TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));

    if (get_CompressImages()) {
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW));
    } else {
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE));
    }

    TIFFCHECK(TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT));

    image -> setTiffMetaData(tif);

    QVector<quint32> buffvec(ncols);
    quint32* buffer = buffvec.data();

    for (int y=0; y<nrows; y++) {
      for (int x=0; x<ncols; x++) {
        buffer[x] = image->value(x,y);
      }

      TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
    }

    TIFFClose(tif);

    image -> saveMetaData(name);
  }

  return res;
}

bool QxrdDataProcessor::saveNamedMaskData(QString name, QxrdMaskData *image, int canOverwrite)
{
  QMutexLocker lock(&m_Mutex);

//  emit printMessage(tr("Saved \"%1\")").arg(name));

  QMutexLocker lockr(image->mutex());

  int nrows = image -> get_Height();
  int ncols = image -> get_Width();

  if (canOverwrite == NoOverwrite) {
    name = uniqueFileName(name);
  }

  TIFF* tif = TIFFOpen(qPrintable(name),"w");
  int res = 1;

  if (tif) {

    TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8));
    TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT));

    image -> setTiffMetaData(tif);

    QVector<quint8> buffvec(ncols);
    quint8* buffer = buffvec.data();

    for (int y=0; y<nrows; y++) {
      for (int x=0; x<ncols; x++) {
        buffer[x] = image->value(x,y);
      }

      TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
    }

    TIFFClose(tif);

    image -> set_FileName(name);

    image -> saveMetaData();
  }

  return res;
}

QString QxrdDataProcessor::uniqueFileName(QString name)
{
  QMutexLocker lock(&m_Mutex);

  QFileInfo f(name);

  if (f.exists()) {
    QDir dir = f.dir();
    QString base = f.baseName();
    QString suff = f.completeSuffix();

    for (int i=1; ; i++) {
      QString newname = dir.filePath(base+QString().sprintf("-%0.5d.",i)+suff);
      QFileInfo f(newname);

      if (!f.exists()) {
        return newname;
      }
    }
  } else {
    return name;
  }
}

void QxrdDataProcessor::clearDark()
{
  QMutexLocker lock(&m_Mutex);

  newDarkImage((QxrdDoubleImageData*) NULL);

  set_DarkImagePath("");
}

void QxrdDataProcessor::clearBadPixels()
{
  QMutexLocker lock(&m_Mutex);

  newBadPixelsImage(NULL);

  set_BadPixelsPath("");
}

void QxrdDataProcessor::clearGainMap()
{
  QMutexLocker lock(&m_Mutex);

  newGainMapImage(NULL);

  set_GainMapPath("");
}

void QxrdDataProcessor::clearMask()
{
  QMutexLocker lock(&m_Mutex);

  m_Mask -> clear();

  newMask();

  set_MaskPath("");
}

void QxrdDataProcessor::processAcquiredInt16Image(QxrdInt16ImageData *img)
{
  QMutexLocker lock(&m_Mutex);

//  printf("QxrdDataProcessor::processAcquiredImage\n");
  emit printMessage(tr("processing acquired 16 bit image, %1 remaining").arg(getAcquiredCount()));

  if (img) {
    QTime tic;
    tic.start();

    if (get_SaveRawImages()) {
      saveNamedRawImageData(img->rawFileName(), img);

      updateEstimatedTime(m_Acquisition -> prop_Raw16SaveTime(), tic.elapsed());

      emit printMessage(tr("Saved raw data in file \"%1\" after %2 msec").
                        arg(img->rawFileName()).arg(tic.restart()));
    }

    QxrdDoubleImageData *dimg = takeNextFreeImage();

    img -> copyImage(dimg);

    returnInt16ImageToPool(img);

    dimg -> set_DateTime(QDateTime::currentDateTime());

    processAcquiredImage(dimg);
  }

  m_Processing.unlock();
}

void QxrdDataProcessor::processAcquiredInt32Image(QxrdInt32ImageData *img)
{
  QMutexLocker lock(&m_Mutex);

//  printf("QxrdDataProcessor::processAcquiredImage\n");
  emit printMessage(tr("processing acquired 32 bit image, %1 remaining").arg(getAcquiredCount()));

  if (img) {
    QTime tic;
    tic.start();

    if (get_SaveRawImages()) {
      saveNamedRawImageData(img->rawFileName(), img);

      updateEstimatedTime(m_Acquisition -> prop_Raw32SaveTime(), tic.elapsed());

      emit printMessage(tr("Saved raw data in file \"%1\" after %2 msec").
                        arg(img->rawFileName()).arg(tic.restart()));
    }

    QxrdDoubleImageData *dimg = takeNextFreeImage();

    img -> copyImage(dimg);

    returnInt32ImageToPool(img);

    dimg -> set_DateTime(QDateTime::currentDateTime());

    processAcquiredImage(dimg);
  }

  m_Processing.unlock();
}

void QxrdDataProcessor::processAcquiredImage(QxrdDoubleImageData *dimg)
{
  QMutexLocker lock(&m_Mutex);

  if (dimg) {
    QTime tic;
    tic.start();

    if (get_PerformDarkSubtraction()) {
      QxrdDoubleImageData *dark   = darkImage();

      subtractDarkImage(dimg, dark);

      updateEstimatedTime(prop_PerformDarkSubtractionTime(), tic.elapsed());

      emit printMessage(tr("Dark subtraction took %1 msec").arg(tic.restart()));
      m_DarkUsage.unlock();
    }

    if (get_PerformBadPixels()) {
      correctBadPixels(dimg);

      updateEstimatedTime(prop_PerformBadPixelsTime(), tic.restart());
    }

    if (get_PerformGainCorrection()) {
      correctImageGains(dimg);

      updateEstimatedTime(prop_PerformGainCorrectionTime(), tic.restart());
    }

    if (get_SaveSubtracted()) {
      saveNamedImageData(dimg->get_FileName(), dimg);

      updateEstimatedTime(prop_SaveSubtractedTime(), tic.elapsed());

      emit printMessage(tr("Saved processed image in file \"%1\" after %2 msec").arg(dimg->get_FileName()).arg(tic.restart()));
    }

    if (get_PerformIntegration()) {
      m_Integrator -> performIntegration();

      updateEstimatedTime(prop_PerformIntegrationTime(), tic.restart());
    }

    if (get_SaveIntegratedData()) {
      updateEstimatedTime(prop_SaveIntegratedDataTime(), tic.restart());
    }

    if (get_DisplayIntegratedData()) {
      updateEstimatedTime(prop_DisplayIntegratedDataTime(), tic.restart());
    }

//    m_ProcessedImages.enqueue(img);

    newData(dimg);

    emit printMessage(tr("Processing took %1 msec").arg(tic.restart()));
  }
}

void QxrdDataProcessor::updateEstimatedTime(QcepDoubleProperty *prop, int msec)
{
  QMutexLocker lock(&m_Mutex);

  double newVal = prop -> value() * (1.0 - get_AveragingRatio()) + ((double) msec)/1000.0* get_AveragingRatio();

  prop -> setValue(newVal);
}

void QxrdDataProcessor::subtractDarkImage(QxrdDoubleImageData *image, QxrdDoubleImageData *dark)
{
  QMutexLocker lock(&m_Mutex);

  if (get_PerformDarkSubtraction()) {
    if (dark && image) {
      if (dark->get_ReadoutMode() != image->get_ReadoutMode()) {
        emit printMessage("Readout modes of acquired data and dark image are different, skipping");
        return;
      }

      if (dark->get_ExposureTime() != image->get_ExposureTime()) {
        emit printMessage("Exposure times of acquired data and dark image are different, skipping");
        return;
      }

      if (dark->get_Width() != image->get_Width() ||
          dark->get_Height() != image->get_Height()) {
        emit printMessage("Dimensions of acquired data and dark image are different, skipping");
        return;
      }

      if (dark->get_CameraGain() != image->get_CameraGain()) {
        emit printMessage("Gains of acquired data and dark image are different, skipping");
        return;
      }

      if (!(image->get_DataType() == QxrdDoubleImageData::Raw16Data ||
            image->get_DataType() == QxrdDoubleImageData::Raw32Data)) {
        emit printMessage("Acquired data is not a raw image, skipping background subtraction");
        return;
      }

      QMutexLocker lock1(dark->mutex());
      QMutexLocker lock2(image->mutex());

      int height = image->get_Height();
      int width  = image->get_Width();
      int nres = image-> get_SummedExposures();
      int ndrk = dark -> get_SummedExposures();
      int npixels = width*height;

      if (nres <= 0) nres = 1;
      if (ndrk <= 0) ndrk = 1;

      double ratio = ((double) nres)/((double) ndrk);

//      printf("Dark subtraction nres=%d, ndrk=%d, npixels=%d, ratio=%g\n",
//             nres, ndrk, npixels, ratio);

      double *result = image->data();
      double *dk     = dark->data();

      for (int i=0; i<npixels; i++) {
        result[i] = result[i]-ratio*dk[i];
      }

      image -> set_DataType(QxrdDoubleImageData::SubtractedData);
    }
  }
}

void QxrdDataProcessor::correctBadPixels(QxrdDoubleImageData */*image*/)
{
  QMutexLocker lock(&m_Mutex);

}

void QxrdDataProcessor::correctImageGains(QxrdDoubleImageData */*image*/)
{
  QMutexLocker lock(&m_Mutex);

}

void QxrdDataProcessor::updateEstimatedProcessingTime()
{
  QMutexLocker lock(&m_Mutex);

  double estTime = 0;

  if (get_SaveRawImages()) {
    if (m_Acquisition -> get_SummedExposures() > 1) {
      estTime += m_Acquisition -> get_Raw32SaveTime();
    } else {
      estTime += m_Acquisition -> get_Raw16SaveTime();
    }
  }

  if (get_PerformDarkSubtraction()) {
    estTime += get_PerformDarkSubtractionTime();
  }

  if (get_PerformBadPixels()) {
    estTime += get_PerformBadPixelsTime();
  }

  if (get_PerformGainCorrection()) {
    estTime += get_PerformGainCorrectionTime();
  }

  if (get_SaveSubtracted()) {
    estTime += get_SaveSubtractedTime();
  }

  if (get_PerformIntegration()) {
    estTime += get_PerformIntegrationTime();
  }

  if (get_DisplayIntegratedData()) {
    estTime += get_DisplayIntegratedDataTime();
  }

  if (get_SaveIntegratedData()) {
    estTime += get_SaveIntegratedDataTime();
  }

  set_EstimatedProcessingTime(estTime);
}

void QxrdDataProcessor::showMaskRange(/*double min, double max*/)
{
  QMutexLocker lock(&m_Mutex);

  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  if (m_Data && m_Mask) {
//    printf ("clearMaskRange(%g,%g)\n", min, max);

    m_Mask -> showMaskRange(m_Data, min, max);

    newMask();
  }
}

void QxrdDataProcessor::hideMaskAll()
{
  QMutexLocker lock(&m_Mutex);

  if (m_Mask) {
//    printf ("setMaskRange(%g,%g)\n", min, max);

    m_Mask -> hideMaskAll();

    newMask();
  }
}

void QxrdDataProcessor::showMaskAll()
{
  QMutexLocker lock(&m_Mutex);

  if (m_Mask) {
//    printf ("clearMaskRange(%g,%g)\n", min, max);

    m_Mask -> showMaskAll();

    newMask();
  }
}

void QxrdDataProcessor::hideMaskRange(/*double min, double max*/)
{
  QMutexLocker lock(&m_Mutex);

  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  if (m_Data && m_Mask) {
//    printf ("setMaskRange(%g,%g)\n", min, max);

    m_Mask -> hideMaskRange(m_Data, min, max);

    newMask();
  }
}

void QxrdDataProcessor::invertMask()
{
  QMutexLocker lock(&m_Mutex);

  if (m_Mask) {
    m_Mask -> invertMask();

    newMask();
  }
}

void QxrdDataProcessor::maskCircle(QwtDoubleRect rect)
{ 
  QMutexLocker lock(&m_Mutex);

  if (m_Mask) {
    if ((rect.left() == rect.right()) && (rect.bottom() == rect.top())) {
      m_Mask -> maskCircle(rect.left(), rect.top(), get_MaskCircleRadius(), get_MaskSetPixels());
    } else {
      double cx = rect.center().x();
      double cy = rect.center().y();
      double rad = rect.width()/2;

      m_Mask -> maskCircle(cx, cy, rad, get_MaskSetPixels());
    }

    newMask();
  }
}

void QxrdDataProcessor::maskPolygon(QVector<QwtDoublePoint> poly)
{
  QMutexLocker lock(&m_Mutex);

  if (m_Mask) {
    //  printf("QxrdDataProcessor::maskPolygon(%d points ...)\n", poly.size());

    int nRows = m_Mask -> get_Height();
    int nCols = m_Mask -> get_Width();

    QImage polyImage(nCols, nRows, QImage::Format_RGB32);
    QPainter polyPainter(&polyImage);
    QPolygonF polygon;

    foreach(QwtDoublePoint pt, poly) {
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
          m_Mask -> setMaskValue(i, j, newval);
        }
      }
    }

    newMask();
  }
}

void QxrdDataProcessor::measurePolygon(QVector<QwtDoublePoint> poly)
{
  QMutexLocker lock(&m_Mutex);

  foreach(QwtDoublePoint pt, poly) {
    emit printMessage(tr("Measure pt (%1,%2) = %3").arg(pt.x()).arg(pt.y())
                      .arg(m_Data -> value(pt.x(),pt.y())));
  }

  summarizeMeasuredPolygon(poly);
}

void QxrdDataProcessor::printMeasuredPolygon(QVector<QwtDoublePoint> poly)
{
  QMutexLocker lock(&m_Mutex);

  foreach(QwtDoublePoint pt, poly) {
    emit printMessage(tr("Measure pt (%1,%2)").arg(pt.x()).arg(pt.y()));
  }

  summarizeMeasuredPolygon(poly);
}

void QxrdDataProcessor::summarizeMeasuredPolygon(QVector<QwtDoublePoint> poly)
{
  if (poly.size() >= 3) {
    double x0 = poly[0].x();
    double y0 = poly[0].y();
    double x1 = poly[1].x();
    double y1 = poly[1].y();
    double x2 = poly[2].x();
    double y2 = poly[2].y();
    double dx1 = x0-x1, dy1 = y0-y1, dx2 = x2-x1, dy2 = y2-y1;
    double a1 = atan2(dy1,dx1), a2 = atan2(dy2,dx2);

    emit statusMessage(tr("Angle: @ %1,%2, ang %3 deg").arg(x1).arg(y1).arg((a2-a1)/M_PI*180.0));
  } else if (poly.size() == 2) {
    double x0 = poly[0].x();
    double y0 = poly[0].y();
    double x1 = poly[1].x();
    double y1 = poly[1].y();
    double dx = x1-x0;
    double dy = y1-y0;
    double ang = atan2(dy,dx);
    double len = sqrt(dx*dx+dy*dy);

    emit statusMessage(tr("Line: %1,%2 - %3,%4 : D %5,%6 : L %7 : Ang %8").
                       arg(x0).arg(y0).arg(x1).arg(y1).
                       arg(dx).arg(dy).arg(len).arg(ang/M_PI*180.0));

  } else if (poly.size() == 1) {
    emit statusMessage(tr("Point: %1,%2").arg(poly[0].x()).arg(poly[0].y()));
  }
}

void QxrdDataProcessor::slicePolygon(QVector<QwtDoublePoint> poly)
{
  QMutexLocker lock(&m_Mutex);

  m_Integrator -> slicePolygon(poly,0);
}

QxrdDoubleImageData *QxrdDataProcessor::data() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Data;
}

QxrdDoubleImageData *QxrdDataProcessor::darkImage() const
{
  QMutexLocker lock(&m_Mutex);

  return m_DarkFrame;
}

QxrdMaskData *QxrdDataProcessor::mask() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Mask;
}

int QxrdDataProcessor::incrementAcquiredCount()
{
//  emit printMessage(tr("QxrdDataProcessor::incrementAcquiredCount m_AcquiredCount = %1").arg(m_AcquiredCount));

  return m_AcquiredCount.fetchAndAddOrdered(+1);
}

int QxrdDataProcessor::decrementAcquiredCount()
{
//  emit printMessage(tr("QxrdDataProcessor::decrementAcquiredCount m_AcquiredCount = %1").arg(m_AcquiredCount));

  return m_AcquiredCount.fetchAndAddOrdered(-1);
}

int QxrdDataProcessor::getAcquiredCount()
{
  return m_AcquiredCount.fetchAndAddOrdered(0);
}

int QxrdDataProcessor::status(double time)
{
  QMutex mutex;
  QMutexLocker lock(&mutex);

  if (m_ProcessWaiting.wait(&mutex, (int)(time*1000))) {
    return getProcessedCount();
  } else {
    return 0;
  }
}

int QxrdDataProcessor::incrementProcessedCount()
{
//  emit printMessage(tr("QxrdDataProcessor::incrementProcessedCount m_ProcessedCount = %1").arg(m_ProcessedCount));

  return m_ProcessedCount.fetchAndAddOrdered(+1);
}

int QxrdDataProcessor::decrementProcessedCount()
{
//  emit printMessage(tr("QxrdDataProcessor::decrementProcessedCount m_ProcessedCount = %1").arg(m_ProcessedCount));

  int res = m_ProcessedCount.fetchAndAddOrdered(-1);

  if (res == 0) {
    m_ProcessWaiting.wakeAll();
  }

  return res;
}

int QxrdDataProcessor::getProcessedCount()
{
  return m_ProcessedCount.fetchAndAddOrdered(0);
}

QxrdCenterFinder  *QxrdDataProcessor::centerFinder() const
{
  QMutexLocker  lock(&m_Mutex);

  if (m_CenterFinder == NULL) {
    emit printMessage("Problem QxrdDataProcessor::centerFinder == NULL");
  }

  return m_CenterFinder;
}

QxrdIntegrator    *QxrdDataProcessor::integrator() const
{
  QMutexLocker  lock(&m_Mutex);

  if (m_Integrator == NULL) {
    emit printMessage("Problem QxrdDataProcessor::integrator == NULL");
  }

  return m_Integrator;
}

void QxrdDataProcessor::newImage(int ncols, int nrows)
{
  QMutexLocker lock(&m_Mutex);

  m_Data -> resize(ncols, nrows);
  m_Data -> fill(0);

  newData(m_Data);
}

void QxrdDataProcessor::exponentialTail(double cx, double cy, double width, int oversample)
{
  QMutexLocker lock(&m_Mutex);

  int nr = m_Data -> get_Height();
  int nc = m_Data -> get_Width();

  for (int y=0; y<nr; y++) {
    for (int x=0; x<nc; x++) {
      double sum = 0;

      for (int iy = 0; iy < oversample; iy++) {
        double yy = (double) y + ((double) iy)/((double) oversample) - cy;
        double yy2 = yy*yy;
        for (int ix = 0; ix < oversample; ix++) {
          double xx = (double) x + ((double) ix)/((double) oversample) - cx;
          double xx2 = xx*xx;
          double r = sqrt(yy2 + xx2);
          sum += exp(-r/width);
        }
      }

      m_Data -> setValue(x, y, m_Data -> value(x,y) + sum/(oversample*oversample));
    }
  }

  newData(m_Data);
}

void QxrdDataProcessor::reciprocalTail(double cx, double cy, double strength, int oversample)
{
  QMutexLocker lock(&m_Mutex);

  int nr = m_Data -> get_Height();
  int nc = m_Data -> get_Width();

  for (int y=0; y<nr; y++) {
    for (int x=0; x<nc; x++) {
      double sum = 0;

      for (int iy = 0; iy < oversample; iy++) {
        double yy = (double) y + ((double) iy)/((double) oversample) - cy;
        double yy2 = yy*yy;
        for (int ix = 0; ix < oversample; ix++) {
          double xx = (double) x + ((double) ix)/((double) oversample) - cx;
          double xx2 = xx*xx;
          double r = sqrt(yy2 + xx2);
          sum += strength/r;
        }
      }

      m_Data -> setValue(x, y, m_Data -> value(x,y) + sum/(oversample*oversample));
    }
  }

  newData(m_Data);
}

void QxrdDataProcessor::powderRing(double cx, double cy, double radius, double width, double strength, int oversample)
{
  QMutexLocker lock(&m_Mutex);

  int nr = m_Data -> get_Height();
  int nc = m_Data -> get_Width();

  for (int y=0; y<nr; y++) {
    for (int x=0; x<nc; x++) {
      double sum = 0;

      for (int iy = 0; iy < oversample; iy++) {
        double yy = (double) y + ((double) iy)/((double) oversample) - cy;
        double yy2 = yy*yy;
        for (int ix = 0; ix < oversample; ix++) {
          double xx = (double) x + ((double) ix)/((double) oversample) - cx;
          double xx2 = xx*xx;
          double r = sqrt(yy2 + xx2);

          double ndr = (r - radius)/width;

          if (fabs(ndr) < 6) {
            double val = strength*exp(-2*ndr*ndr)/width*sqrt(2.0/M_PI);
            sum += val;
          }
        }
      }

      m_Data -> setValue(x, y, m_Data -> value(x,y) + sum/(oversample*oversample));
    }
  }

  newData(m_Data);
}

void QxrdDataProcessor::openLogFile()
{
  QMutexLocker lock(&m_LogFileMutex);

  if (m_LogFile == NULL) {
    m_LogFile = fopen(qPrintable(get_LogFilePath()), "a");

    if (m_LogFile) {
      writeLogHeader();
    }
  }
}

void QxrdDataProcessor::newLogFile(QString path)
{
  QMutexLocker lock(&m_LogFileMutex);

  if (m_LogFile) {
    fclose(m_LogFile);
    m_LogFile = NULL;
  }

  set_LogFilePath(path);

  openLogFile();
}

void QxrdDataProcessor::writeLogHeader()
{
  QMutexLocker lock(&m_LogFileMutex);

  if (m_LogFile) {
    fprintf(m_LogFile, "#F %s\n", qPrintable(get_LogFilePath()));
    fprintf(m_LogFile, "#E %d\n", QDateTime::currentDateTime().toTime_t());
    fprintf(m_LogFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")));
  }
}

void QxrdDataProcessor::logMessage(QString msg)
{
  QMutexLocker lock(&m_LogFileMutex);

  openLogFile();

  if (m_LogFile) {
    fprintf(m_LogFile, "#CX %s\n", qPrintable(msg));
  }
}

void QxrdDataProcessor::fileWriteTest(int dim, QString path)
{
  QMutexLocker lock(&m_Mutex);

  long sz = dim*dim;
  quint32 *buff = new quint32[sz];

  QTime tic;
  tic.start();
  int totalt = 0;

  for (int i=0; i<10; i++) {
    QString fileName = path+tr("%1.junk").arg(i,5,10,QChar('0'));

    FILE *f = fopen(qPrintable(fileName), "w");

    if (f) {
      fwrite(buff, sz, sizeof(quint32), f);
      fclose(f);

      int dt = tic.restart();
      totalt += dt;
      emit printMessage(tr("file %1 written in %2 msec").arg(fileName).arg(dt));
    }
  }

  emit printMessage(tr("average write speed %1 MB/sec").arg(((double) sz)*40.0*1000.0/(1e6*((double) totalt))));
  delete [] buff;
}

/******************************************************************
*
*  $Log: qxrddataprocessor.cpp,v $
*  Revision 1.56  2009/09/28 22:06:55  jennings
*  Added circular integration to processing steps
*
*  Revision 1.55  2009/09/26 04:55:46  jennings
*  Removed some commented-out sections
*  Removed QxrdDataProcessor::copyImage(src,dst), replaced with templatized image member fn
*
*  Revision 1.54  2009/09/25 22:42:48  jennings
*  Masking changes
*
*  Revision 1.53  2009/09/25 14:22:16  jennings
*  Simplified double-buffering for plotted data - there is now a separate copy of data and mask
*  in QxrdWindow
*
*  Revision 1.52  2009/09/22 18:19:00  jennings
*  Added slicing routines
*  Set title for traces in avg data graph
*
*  Revision 1.51  2009/09/21 19:40:46  jennings
*  Added version number to window title, added more measurement output
*
*  Revision 1.50  2009/09/21 16:27:58  jennings
*  Added user interface to log file path
*
*  Revision 1.49  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.48  2009/09/18 20:44:49  jennings
*  Add separate status functions for acquisition and processing, as well as an aggregated function
*  combining the status of the two.
*
*  Revision 1.47  2009/09/14 19:08:57  jennings
*  Added more checks for appropriate data type / exposure etc. before subtracting
*  backgrounds
*
*  Revision 1.46  2009/09/13 13:59:47  jennings
*  Added 'canOverwrite' argument to data saving routines and arrange
*  that saves via file dialogs can overwrite, programmatic saves use
*  unique file names
*
*  Revision 1.45  2009/09/12 14:44:20  jennings
*  Moved lock to base class, made into mutex
*
*  Revision 1.44  2009/09/12 13:44:37  jennings
*  Renamed convertImage to copyImage, added double->double version of copyImage,
*  added timer to copyImage
*
*  Revision 1.43  2009/09/12 13:36:19  jennings
*  Added more locks to QxrdDataProcessor
*
*  Revision 1.42  2009/09/10 21:33:30  jennings
*  Added TIFF error handling
*
*  Revision 1.41  2009/09/09 22:32:12  jennings
*  Started to add TIFF metadata support
*
*  Revision 1.40  2009/09/08 21:41:59  jennings
*  Update estimated processing time accumulators
*
*  Revision 1.39  2009/09/07 22:10:14  jennings
*  Allow NULL mask
*
*  Revision 1.38  2009/09/04 21:11:41  jennings
*  Support for file write timing tests
*
*  Revision 1.37  2009/09/04 20:04:31  jennings
*  Debugging pre-trigger acquisition
*
*  Revision 1.36  2009/09/04 15:15:42  jennings
*  Added log file routines
*  Removed newlines from any printMessage calls.
*
*  Revision 1.35  2009/09/03 21:16:24  jennings
*  Added properties and user interface elements for pre- and post- trigger counts
*  Added properties and user interface elements for fine-grained control of processing chain
*
*  Revision 1.34  2009/08/27 21:55:43  jennings
*  Added code to make sure file saving routines will not overwrite data
*
*  Revision 1.33  2009/08/27 21:02:17  jennings
*  Partial implementation of lazy plotting
*
*  Revision 1.32  2009/08/27 17:06:52  jennings
*  Added code to load/save dark and mask data
*
*  Revision 1.31  2009/08/26 20:57:13  jennings
*  Starting to implement separate Double Int16 and Int32 processing chains
*
*  Revision 1.30  2009/08/25 18:49:19  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.29  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.28  2009/08/09 15:40:32  jennings
*  Added measurer tool to all graphs
*
*  Revision 1.27  2009/08/08 20:14:55  jennings
*  Added powder ring calculation operations
*
*  Revision 1.26  2009/08/07 22:21:56  jennings
*  Added a number of sample data creation routines to QxrdDataProcessor
*  Added a parallelized integration routine to QxrdIntegrator
*
*  Revision 1.25  2009/08/04 20:42:53  jennings
*  Simple, initial, implementation of integration
*
*  Revision 1.24  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.23  2009/08/03 20:58:59  jennings
*  Minor fixups
*
*  Revision 1.22  2009/08/03 13:26:25  jennings
*  Added option to set/clear mask pixels
*
*  Revision 1.21  2009/08/02 21:14:16  jennings
*  Added masking dummy routines
*
*  Revision 1.20  2009/07/22 11:55:34  jennings
*  Center finder modifications
*
*  Revision 1.19  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.18  2009/07/20 00:34:49  jennings
*  Send data between acquisition and data processor via signal/slot args, rather
*  than image queues
*
*  Revision 1.17  2009/07/17 21:10:39  jennings
*  Modifications related to mask display
*
*  Revision 1.16  2009/07/17 20:41:20  jennings
*  Modifications related to mask display
*
*  Revision 1.15  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.14  2009/07/17 12:41:33  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.13  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.12  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.11  2009/07/08 19:06:27  jennings
*  Made centering parameters into Q_PROPERTYs
*  Saved centering, integrator and data processor settings
*
*  Revision 1.10  2009/06/30 21:35:05  jennings
*  Debugging meta data static init problems
*
*  Revision 1.9  2009/06/28 16:33:20  jennings
*  Eliminated compiler warnings
*
*  Revision 1.8  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

