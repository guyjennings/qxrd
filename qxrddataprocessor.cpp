/******************************************************************
*
*  $Id: qxrddataprocessor.cpp,v 1.41 2009/09/09 22:32:12 jennings Exp $
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
    m_Window(win),
    m_Acquisition(acq),
    m_DarkUsage(QReadWriteLock::Recursive),
//    m_ProcessedImages("QxrdDataProcessor Processed Images"),
//    m_DarkImages("QxrdDataProcessor Dark Images"),
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
    SOURCE_IDENT("$Id: qxrddataprocessor.cpp,v 1.41 2009/09/09 22:32:12 jennings Exp $")
{
  m_CenterFinder = new QxrdCenterFinder(this);
  m_Integrator   = new QxrdIntegrator(this, this);

  m_DarkImagePath.setDebug(10);
}

void QxrdDataProcessor::setAcquisition(QxrdAcquisition*acq)
{
  m_Acquisition = acq;

  connect(m_Acquisition, SIGNAL(acquiredImageAvailable(QxrdDoubleImageData*)),
          this, SLOT(onAcquiredImageAvailable(QxrdDoubleImageData*)));
  connect(m_Acquisition, SIGNAL(acquiredInt16ImageAvailable(QxrdInt16ImageData*)),
          this, SLOT(onAcquiredInt16ImageAvailable(QxrdInt16ImageData*)));
  connect(m_Acquisition, SIGNAL(acquiredInt32ImageAvailable(QxrdInt32ImageData*)),
          this, SLOT(onAcquiredInt32ImageAvailable(QxrdInt32ImageData*)));
/*  connect(m_Acquisition, SIGNAL(darkImageAvailable(QxrdDoubleImageData*)),
          this, SLOT(onDarkImageAvailable(QxrdDoubleImageData*))); */

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

//void QxrdDataProcessor::onAcquiredImageAvailable(QxrdDoubleImageData *image)
//{
//  int navail = decrementAcquiredCount();
//
//  printf("QxrdDataProcessor::on_acquired_image_available(), navail = %d\n", navail);
//
////  QxrdImageData *image = m_Acquisition -> takeNextAcquiredImage();
//
//  if (image) {
////    printf("Image Number %d\n", image -> imageNumber());
//
//    if ((image -> get_ImageNumber()) >= 0) {
//      m_DarkUsage.lockForRead();
//      m_Processing.lockForRead();
//      QtConcurrent::run(this, &QxrdDataProcessor::processAcquiredImage, image);
//    } else {
//      QWriteLocker wl(&m_DarkUsage);
//
//      emit printMessage(tr("Saving dark image \"%1\"").arg(image->get_FileName()));
//
//      saveImageData(image);
//
////      m_DarkImages.enqueue(image);
//
//      newDarkImage(image);
//    }
//  }
//}

void QxrdDataProcessor::onAcquiredInt16ImageAvailable(QxrdInt16ImageData *image)
{
  int navail = decrementAcquiredCount();

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      m_DarkUsage.lockForRead();
      m_Processing.lockForRead();
//      QtConcurrent::run(this, &QxrdDataProcessor::processAcquiredInt16Image, image);
      processAcquiredInt16Image(image);
    } else {
      QWriteLocker wl(&m_DarkUsage);

      saveNamedImageData(image->get_FileName(), image);

      set_DarkImagePath(image->get_FileName());

      emit printMessage(tr("Saved dark image \"%1\"").arg(image->get_FileName()));

//      m_DarkImages.enqueue(image);

      newDarkImage(image);

      returnInt16ImageToPool(image);
    }
  }
}

void QxrdDataProcessor::onAcquiredInt32ImageAvailable(QxrdInt32ImageData *image)
{
  int navail = decrementAcquiredCount();

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      m_DarkUsage.lockForRead();
      m_Processing.lockForRead();
//      QtConcurrent::run(this, &QxrdDataProcessor::processAcquiredInt32Image, image);
      processAcquiredInt32Image(image);
    } else {
      QWriteLocker wl(&m_DarkUsage);

      saveNamedImageData(image->get_FileName(), image);

      set_DarkImagePath(image->get_FileName());

      emit printMessage(tr("Saved dark image \"%1\"").arg(image->get_FileName()));

//      m_DarkImages.enqueue(image);

      newDarkImage(image);

      returnInt32ImageToPool(image);
    }
  }
}

QxrdDoubleImageData *QxrdDataProcessor::takeNextFreeImage()
{
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
  m_Acquisition -> returnImageToPool(img);
}

void QxrdDataProcessor::returnInt32ImageToPool(QxrdInt32ImageData *img)
{
  m_Acquisition -> returnImageToPool(img);
}

void QxrdDataProcessor::newData(QxrdDoubleImageData *image)
{

  if (m_Data != image) {
//    image -> copyMask(m_Data);

    if (m_Data) {
      returnImageToPool(m_Data);
    }

    m_Data = image;
  }

  set_FileName(image->get_FileName());

  incrementProcessedCount();

  m_Data = m_Window -> newDataAvailable(m_Data);
}

void QxrdDataProcessor::newDarkImage(QxrdDoubleImageData *image)
{
  if (m_DarkFrame != image) {
    if (m_DarkFrame) {
      returnImageToPool(m_DarkFrame);
    }

    m_DarkFrame = image;
  }

  set_DarkImagePath(image->get_FileName());

  emit newDarkImageAvailable(m_DarkFrame);
}

void QxrdDataProcessor::newDarkImage(QxrdInt16ImageData *image)
{
  if (m_DarkFrame == NULL) {
    m_DarkFrame = new QxrdDoubleImageData();
  }

  convertImage(image, m_DarkFrame);

  set_DarkImagePath(m_DarkFrame -> get_FileName());

  emit newDarkImageAvailable(m_DarkFrame);
}

void QxrdDataProcessor::newDarkImage(QxrdInt32ImageData *image)
{
  if (m_DarkFrame == NULL) {
    m_DarkFrame = new QxrdDoubleImageData();
  }

  convertImage(image, m_DarkFrame);

  set_DarkImagePath(m_DarkFrame -> get_FileName());

  emit newDarkImageAvailable(m_DarkFrame);
}

void QxrdDataProcessor::newBadPixelsImage(QxrdDoubleImageData *image)
{
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
  if (m_GainMap != image) {
    if (m_GainMap) {
      returnImageToPool(m_GainMap);
    }

    m_GainMap = image;
  }

  set_GainMapPath(image->get_FileName());
}

void QxrdDataProcessor::newMask(QxrdMaskData *mask)
{
  if (m_Mask != mask) {
    delete m_Mask;

    m_Mask = mask;
  }

  emit newMaskAvailable(m_Data, m_Mask);
}

void QxrdDataProcessor::loadDefaultImages()
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

void QxrdDataProcessor::loadData(QString name)
{
//  printf("QxrdDataProcessor::loadData(%s)\n", qPrintable(name));

  QxrdDoubleImageData* res = takeNextFreeImage();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    newData(res);

    set_DataPath(res -> get_FileName());
  } else {
    delete res;
  }
}

void QxrdDataProcessor::saveData(QString name)
{
  QMutexLocker lock(&m_Mutex);

  if (saveNamedImageData(name, m_Data)) {
    set_DataPath(m_Data -> get_FileName());
  }
}

void QxrdDataProcessor::loadDark(QString name)
{
//  printf("QxrdDataProcessor::loadDark(%s)\n", qPrintable(name));

  QxrdDoubleImageData* res = takeNextFreeImage();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    newDarkImage(res);

    set_DarkImagePath(res -> get_FileName());
  } else {
    delete res;
  }
}

void QxrdDataProcessor::saveDark(QString name)
{
  QMutexLocker lock(&m_Mutex);

  if (saveNamedImageData(name, m_DarkFrame)) {
    set_DarkImagePath(m_DarkFrame -> get_FileName());
  }
}

void QxrdDataProcessor::loadBadPixels(QString name)
{
//  printf("QxrdDataProcessor::loadDark(%s)\n", qPrintable(name));

  QxrdDoubleImageData* res = takeNextFreeImage();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    newBadPixelsImage(res);

    set_BadPixelsPath(res -> get_FileName());
  } else {
    delete res;
  }
}

void QxrdDataProcessor::saveBadPixels(QString name)
{
  QMutexLocker lock(&m_Mutex);

  if (saveNamedImageData(name, m_BadPixels)) {
    set_BadPixelsPath(m_BadPixels -> get_FileName());
  }
}

void QxrdDataProcessor::loadGainMap(QString name)
{
//  printf("QxrdDataProcessor::loadGainMap(%s)\n", qPrintable(name));

  QxrdDoubleImageData* res = takeNextFreeImage();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    newGainMapImage(res);

    set_GainMapPath(res -> get_FileName());
  } else {
    delete res;
  }
}

void QxrdDataProcessor::saveGainMap(QString name)
{
  QMutexLocker lock(&m_Mutex);

  if (saveNamedImageData(name, m_GainMap)) {
    set_GainMapPath(m_GainMap -> get_FileName());
  }
}

void QxrdDataProcessor::loadMask(QString name)
{
//  printf("QxrdDataProcessor::loadData(%s)\n", qPrintable(name));

  QxrdMaskData* res = new QxrdMaskData();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    newMask(res);

    set_MaskPath(res -> get_FileName());
  } else {
    delete res;
  }
}

void QxrdDataProcessor::saveMask(QString name)
{
  QMutexLocker lock(&m_Mutex);

  if (saveNamedMaskData(name, m_Mask)) {
    set_MaskPath(m_Mask -> get_FileName());
  }
}

//void QxrdDataProcessor::loadBadPixels(QString name)
//{
//  QxrdDoubleImageData* res = takeNextFreeImage();
//
//  if (res -> readImage(name)) {
//    newBadPixelsImage(res);
//
//    set_BadPixelsPath(res -> get_FileName());
//  } else {
//    delete res;
//  }
//}
//
//void QxrdDataProcessor::loadGainMap(QString name)
//{
//  QxrdDoubleImageData* res = takeNextFreeImage();
//
//  res -> readImage(name);
//
//  newGainMapImage(res);
//}
//
//void QxrdDataProcessor::saveImageData(QxrdDoubleImageData *image)
//{
//  saveNamedImageData(image->get_FileName(), image);
//}
//
//void QxrdDataProcessor::saveImageData(QxrdInt16ImageData *image)
//{
//  saveNamedImageData(image->get_FileName(), image);
//}
//
//void QxrdDataProcessor::saveImageData(QxrdInt32ImageData *image)
//{
//  saveNamedImageData(image->get_FileName(), image);
//}
//
//void QxrdDataProcessor::saveRawData(QxrdInt16ImageData *image)
//{
//  saveNamedImageData(image->rawFileName(), image);
//}
//
//void QxrdDataProcessor::saveRawData(QxrdInt32ImageData *image)
//{
//  saveNamedImageData(image->rawFileName(), image);
//}
//
bool QxrdDataProcessor::saveNamedImageData(QString name, QxrdDoubleImageData *image)
{
//  emit printMessage(tr("Saved \"%1\")").arg(name));

  QReadLocker lock(image->rwLock());

  int nrows = image -> get_Height();
  int ncols = image -> get_Width();

  name = uniqueFileName(name);

  TIFF* tif = TIFFOpen(qPrintable(name),"w");

  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);

  if (get_CompressImages()) {
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
  } else {
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  }

  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32);
  TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);

  image -> setTiffMetaData(tif);

  QVector<float> buffvec(ncols);
  float* buffer = buffvec.data();

  for (int y=0; y<nrows; y++) {
    for (int x=0; x<ncols; x++) {
      buffer[x] = image->value(x,y);
    }

    TIFFWriteScanline(tif, buffer, y, 0);
  }

  TIFFClose(tif);

  image -> set_FileName(name);

  return true;
}

bool QxrdDataProcessor::saveNamedImageData(QString name, QxrdInt16ImageData *image)
{
  bool res = saveNamedRawImageData(name, image);

  if (res) {
    image -> set_FileName(name);
  }

  return res;
}

bool QxrdDataProcessor::saveNamedRawImageData(QString name, QxrdInt16ImageData *image)
{
//  emit printMessage(tr("Saved \"%1\")").arg(name));

  QReadLocker lock(image->rwLock());

  int nrows = image -> get_Height();
  int ncols = image -> get_Width();

  name = uniqueFileName(name);

  TIFF* tif = TIFFOpen(qPrintable(name),"w");

  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);

  if (get_CompressImages()) {
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
  } else {
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  }

  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16);
  TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);

  image -> setTiffMetaData(tif);

  QVector<quint16> buffvec(ncols);
  quint16* buffer = buffvec.data();

  for (int y=0; y<nrows; y++) {
    for (int x=0; x<ncols; x++) {
      buffer[x] = image->value(x,y);
    }

    TIFFWriteScanline(tif, buffer, y, 0);
  }

  TIFFClose(tif);

  return true;
}

bool QxrdDataProcessor::saveNamedImageData(QString name, QxrdInt32ImageData *image)
{
  bool res = saveNamedRawImageData(name, image);

  if (res) {
    image -> set_FileName(name);
  }

  return res;
}

bool QxrdDataProcessor::saveNamedRawImageData(QString name, QxrdInt32ImageData *image)
{
//  emit printMessage(tr("Saved \"%1\")").arg(name));

  QReadLocker lock(image->rwLock());

  int nrows = image -> get_Height();
  int ncols = image -> get_Width();

  name = uniqueFileName(name);

  TIFF* tif = TIFFOpen(qPrintable(name),"w");

  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);

  if (get_CompressImages()) {
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
  } else {
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  }

  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32);
  TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);

  image -> setTiffMetaData(tif);

  QVector<quint32> buffvec(ncols);
  quint32* buffer = buffvec.data();

  for (int y=0; y<nrows; y++) {
    for (int x=0; x<ncols; x++) {
      buffer[x] = image->value(x,y);
    }

    TIFFWriteScanline(tif, buffer, y, 0);
  }

  TIFFClose(tif);

  return true;
}

bool QxrdDataProcessor::saveNamedMaskData(QString name, QxrdMaskData *image)
{
//  emit printMessage(tr("Saved \"%1\")").arg(name));

  QReadLocker lock(image->rwLock());

  int nrows = image -> get_Height();
  int ncols = image -> get_Width();

  name = uniqueFileName(name);

  TIFF* tif = TIFFOpen(qPrintable(name),"w");

  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);

  image -> setTiffMetaData(tif);

  QVector<quint8> buffvec(ncols);
  quint8* buffer = buffvec.data();

  for (int y=0; y<nrows; y++) {
    for (int x=0; x<ncols; x++) {
      buffer[x] = image->value(x,y);
    }

    TIFFWriteScanline(tif, buffer, y, 0);
  }

  TIFFClose(tif);

  image -> set_FileName(name);

  return true;
}

QString QxrdDataProcessor::uniqueFileName(QString name)
{
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
  newDarkImage((QxrdDoubleImageData*) NULL);

  set_DarkImagePath("");
}

void QxrdDataProcessor::clearBadPixels()
{
  newBadPixelsImage(NULL);

  set_BadPixelsPath("");
}

void QxrdDataProcessor::clearGainMap()
{
  newGainMapImage(NULL);

  set_GainMapPath("");
}

void QxrdDataProcessor::clearMask()
{
  newMask(NULL);

  set_MaskPath("");
}

//void QxrdDataProcessor::loadDarkImage(QString name)
//{
//  QxrdDoubleImageData* img = takeNextFreeImage();
//
//  img -> readImage(name);
//
//  newDarkImage(img);
//}
//
void QxrdDataProcessor::convertImage(QxrdInt16ImageData *src, QxrdDoubleImageData *dest)
{
  if (src && dest) {
    int ncols = src -> get_Width();
    int nrows = src -> get_Height();
    int npix = ncols*nrows;

    dest -> resize(ncols, nrows);

    src -> copyProperties(dest);

    quint16 *srcp = src -> data();
    double  *destp = dest -> data();

    for (int i=0; i<npix; i++) {
      *destp++ = *srcp++;
    }
  }
}

void QxrdDataProcessor::convertImage(QxrdInt32ImageData *src, QxrdDoubleImageData *dest)
{
  if (src && dest) {
    int ncols = src -> get_Width();
    int nrows = src -> get_Height();
    int npix = ncols*nrows;

    dest -> resize(ncols, nrows);

    src -> copyProperties(dest);

    quint32 *srcp = src -> data();
    double  *destp = dest -> data();

    for (int i=0; i<npix; i++) {
      *destp++ = *srcp++;
    }
  }
}

//void QxrdDataProcessor::processAcquiredImage(QxrdDoubleImageData *img)
//{
////  printf("QxrdDataProcessor::processAcquiredImage\n");
//
//  QxrdDoubleImageData *dark   = darkImage();
//
//  if (img) {
//    QTime tic;
//    tic.start();
//
//    if (get_PerformDarkSubtraction()) {
//      if (dark && get_SaveRawImages()) {
//        emit printMessage(tr("Saving raw data in file \"%1\"").arg(img->rawFileName()));
//
//        saveRawData(img);
//      }
//    }
//
//    subtractDarkImage(img, dark);
//    emit printMessage(tr("Dark subtraction took %1 msec").arg(tic.elapsed()));
//    m_DarkUsage.unlock();
//
//    correctBadPixels(img);
//    correctImageGains(img);
//
//    emit printMessage(tr("Saving processed image in file \"%1\"").arg(img->get_FileName()));
//
//    saveImageData(img);
//
////    m_ProcessedImages.enqueue(img);
//
//    newData(img);
//
//    emit printMessage(tr("Processing took %1 msec").arg(tic.elapsed()));
//  }
//
//  m_Processing.unlock();
//}

void QxrdDataProcessor::processAcquiredInt16Image(QxrdInt16ImageData *img)
{
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

    convertImage(img, dimg);

    returnInt16ImageToPool(img);

    dimg -> set_DateTime(QDateTime::currentDateTime());

    processAcquiredImage(dimg);
  }

  m_Processing.unlock();
}

void QxrdDataProcessor::processAcquiredInt32Image(QxrdInt32ImageData *img)
{
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

    convertImage(img, dimg);

    returnInt32ImageToPool(img);

    dimg -> set_DateTime(QDateTime::currentDateTime());

    processAcquiredImage(dimg);
  }

  m_Processing.unlock();
}

void QxrdDataProcessor::processAcquiredImage(QxrdDoubleImageData *dimg)
{
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
  double newVal = prop -> value() * (1.0 - get_AveragingRatio()) + ((double) msec)/1000.0* get_AveragingRatio();

  prop -> setValue(newVal);
}

void QxrdDataProcessor::subtractDarkImage(QxrdDoubleImageData *image, QxrdDoubleImageData *dark)
{
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

      QReadLocker lock1(dark->rwLock());
      QWriteLocker lock2(image->rwLock());

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
    }
  }
}

void QxrdDataProcessor::correctBadPixels(QxrdDoubleImageData */*image*/)
{
}

void QxrdDataProcessor::correctImageGains(QxrdDoubleImageData */*image*/)
{
}

void QxrdDataProcessor::updateEstimatedProcessingTime()
{
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
  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  if (m_Data && m_Mask) {
//    printf ("clearMaskRange(%g,%g)\n", min, max);

    m_Mask -> showMaskRange(m_Data, min, max);

    newMask(m_Mask);
  }
}

void QxrdDataProcessor::hideMaskAll()
{
  if (m_Mask) {
//    printf ("setMaskRange(%g,%g)\n", min, max);

    m_Mask -> hideMaskAll();

    newMask(m_Mask);
  }
}

void QxrdDataProcessor::showMaskAll()
{
  if (m_Mask) {
//    printf ("clearMaskRange(%g,%g)\n", min, max);

    m_Mask -> showMaskAll();

    newMask(m_Mask);
  }
}

void QxrdDataProcessor::hideMaskRange(/*double min, double max*/)
{
  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  if (m_Data && m_Mask) {
//    printf ("setMaskRange(%g,%g)\n", min, max);

    m_Mask -> hideMaskRange(m_Data, min, max);

    newMask(m_Mask);
  }
}

void QxrdDataProcessor::invertMask()
{
  if (m_Mask) {
    m_Mask -> invertMask();

    newMask(m_Mask);
  }
}

void QxrdDataProcessor::maskCircle(QwtDoubleRect rect)
{ 
  if (m_Mask) {
    if ((rect.left() == rect.right()) && (rect.bottom() == rect.top())) {
      m_Mask -> maskCircle(rect.left(), rect.top(), get_MaskCircleRadius(), get_MaskSetPixels());
    } else {
      double cx = rect.center().x();
      double cy = rect.center().y();
      double rad = rect.width()/2;

      m_Mask -> maskCircle(cx, cy, rad, get_MaskSetPixels());
    }

    newMask(m_Mask);
  }
}

void QxrdDataProcessor::maskPolygon(QVector<QwtDoublePoint> poly)
{
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

    newMask(m_Mask);
  }
}

void QxrdDataProcessor::measurePolygon(QVector<QwtDoublePoint> poly)
{
  foreach(QwtDoublePoint pt, poly) {
    emit printMessage(tr("Measure pt (%1,%2) = %3").arg(pt.x()).arg(pt.y())
                      .arg(m_Data -> value(pt.x(),pt.y())));
  }
}

void QxrdDataProcessor::printMeasuredPolygon(QVector<QwtDoublePoint> poly)
{
  foreach(QwtDoublePoint pt, poly) {
    emit printMessage(tr("Measure pt (%1,%2)").arg(pt.x()).arg(pt.y()));
  }
}

void QxrdDataProcessor::slicePolygon(QVector<QwtDoublePoint> poly)
{
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

int QxrdDataProcessor::incrementProcessedCount()
{
//  emit printMessage(tr("QxrdDataProcessor::incrementProcessedCount m_ProcessedCount = %1").arg(m_ProcessedCount));

  return m_ProcessedCount.fetchAndAddOrdered(+1);
}

int QxrdDataProcessor::decrementProcessedCount()
{
//  emit printMessage(tr("QxrdDataProcessor::decrementProcessedCount m_ProcessedCount = %1").arg(m_ProcessedCount));

  return m_ProcessedCount.fetchAndAddOrdered(-1);
}

int QxrdDataProcessor::getProcessedCount()
{
  return m_ProcessedCount.fetchAndAddOrdered(0);
}

QxrdCenterFinder  *QxrdDataProcessor::centerFinder() const
{
  QMutexLocker  lock(&m_Mutex);

  if (m_CenterFinder == NULL) {
    printf("Problem QxrdDataProcessor::centerFinder == NULL\n");
  }

  return m_CenterFinder;
}

QxrdIntegrator    *QxrdDataProcessor::integrator() const
{
  QMutexLocker  lock(&m_Mutex);

  if (m_Integrator == NULL) {
    printf("Problem QxrdDataProcessor::integrator == NULL\n");
  }

  return m_Integrator;
}

void QxrdDataProcessor::newImage(int ncols, int nrows)
{
  m_Data -> resize(ncols, nrows);
  m_Data -> fill(0);

  newData(m_Data);
}

void QxrdDataProcessor::exponentialTail(double cx, double cy, double width, int oversample)
{
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
  if (m_LogFile == NULL) {
    m_LogFile = fopen(qPrintable(get_LogFilePath()), "a");

    if (m_LogFile) {
      writeLogHeader();
    }
  }
}

void QxrdDataProcessor::newLogFile(QString path)
{
  if (m_LogFile) {
    fclose(m_LogFile);
    m_LogFile = NULL;
  }

  set_LogFilePath(path);

  openLogFile();
}

void QxrdDataProcessor::writeLogHeader()
{
  if (m_LogFile) {
    fprintf(m_LogFile, "#F %s\n", qPrintable(get_LogFilePath()));
    fprintf(m_LogFile, "#E %d\n", QDateTime::currentDateTime().toTime_t());
    fprintf(m_LogFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")));
  }
}

void QxrdDataProcessor::logMessage(QString msg)
{
  openLogFile();

  if (m_LogFile) {
    fprintf(m_LogFile, "#CX %s\n", qPrintable(msg));
  }
}

void QxrdDataProcessor::fileWriteTest(int dim, QString path)
{
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

