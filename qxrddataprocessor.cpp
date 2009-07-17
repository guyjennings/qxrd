/******************************************************************
*
*  $Id: qxrddataprocessor.cpp,v 1.16 2009/07/17 20:41:20 jennings Exp $
*
*******************************************************************/

#include "qxrddataprocessor.h"
#include <QtConcurrentRun>
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdimagedata.h"

#include "tiffio.h"

#include <QTime>

QxrdDataProcessor::QxrdDataProcessor
    (QxrdAcquisition *acq, QObject *parent)
  : QObject(parent),
    m_OutputDirectory(this,"outputDirectory", ""),
    m_DarkImagePath(this, "darkImagePath", ""),
    m_BadPixelsPath(this, "badPixelsPath", ""),
    m_GainMapPath(this, "gainMapPath", ""),
    m_PerformDarkSubtraction(this, "performDarkSubtraction", true),
    m_SaveRawImages(this, "saveRawImages", true),
    m_PerformBadPixels(this, "performBadPixels", true),
    m_PerformGainCorrection(this, "performGainCorrection", true),
    m_FileName(this,"fileName",""),
    m_MaskMinimumValue(this, "maskMinimumValue", 0),
    m_MaskMaximumValue(this, "maskMaximumValue", 20000),
    m_Acquisition(acq),
    m_DarkUsage(QReadWriteLock::Recursive),
    m_ProcessedImages("QxrdDataProcessor Processed Images"),
    m_DarkImages("QxrdDataProcessor Dark Images"),
    m_Data(new QxrdImageData(2048,2048)),
    m_DarkFrame(NULL),
    m_BadPixels(NULL),
    m_GainFrame(NULL),
    SOURCE_IDENT("$Id: qxrddataprocessor.cpp,v 1.16 2009/07/17 20:41:20 jennings Exp $")
{
}

void QxrdDataProcessor::setAcquisition(QxrdAcquisition*acq)
{
  m_Acquisition = acq;

  connect(m_Acquisition, SIGNAL(acquiredImageAvailable(QxrdImageData*)), this, SLOT(onAcquiredImageAvailable(QxrdImageData*)));
//  connect(m_Acquisition, SIGNAL(darkImageAvailable(QxrdImageData*)), this, SLOT(onDarkImageAvailable(QxrdImageData*)));
}

void QxrdDataProcessor::writeSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdDataProcessor::readSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdDataProcessor::onAcquiredImageAvailable(QxrdImageData *img)
{
//  printf("QxrdDataProcessor::on_acquired_image_available()\n");

  QxrdImageData *image = m_Acquisition -> takeNextAcquiredImage();

  if (image) {
//    printf("Image Number %d\n", image -> imageNumber());

    if ((image -> get_ImageNumber()) >= 0) {
      m_DarkUsage.lockForRead();
      m_Processing.lockForRead();
      QtConcurrent::run(this, &QxrdDataProcessor::processAcquiredImage, image);
    } else {
      QWriteLocker wl(&m_DarkUsage);

      emit printMessage(tr("Saving dark image \"%1\"").arg(image->get_FileName()));

      saveImageData(image);

      m_DarkImages.enqueue(image);

      emit darkImageAvailable(image);
    }
  }
}

QxrdImageData *QxrdDataProcessor::takeNextFreeImage()
{
  if (m_FreeImages.size() == 0) {
//    printf("Allocate new image\n");
    return new QxrdImageData(2048, 2048);
  } else {
    return m_FreeImages.dequeue();
  }
}

QxrdImageData *QxrdDataProcessor::takeNextProcessedImage()
{
  return m_ProcessedImages.dequeue();
}

QxrdImageData *QxrdDataProcessor::takeLatestProcessedImage()
{
  int n = m_ProcessedImages.size();

  QxrdImageData *res = NULL;

  for (int i=0; i<n; i++) {
    QxrdImageData *img = m_ProcessedImages.dequeue();

    if (res) {
      returnImageToPool(res);
    }

    res = img;
  }

  return res;
}

QxrdImageData *QxrdDataProcessor::takeNextDarkImage()
{
  return m_DarkImages.dequeue();
}

void QxrdDataProcessor::returnImageToPool(QxrdImageData *img)
{
  QMutexLocker lock(&m_Mutex);

  m_FreeImages.enqueue(img);
}

void QxrdDataProcessor::newData(QxrdImageData *image)
{
  if (m_Data != image) {
    image -> copyMask(m_Data);

    if (m_Data) {
      returnImageToPool(m_Data);
    }

    m_Data = image;
  }
//
//  QxrdRasterData     data(m_Data, interpolatePixels());
//  QxrdMaskRasterData mask(m_Data, interpolatePixels());
//
//  m_Plot -> setImage(data);
//  m_Plot -> setMask(mask);
//  m_Plot -> setTitle(m_Data -> get_Title());
//  m_Plot -> replot();

  set_FileName(image->get_FileName());

  emit newDataAvailable(m_Data);
}

void QxrdDataProcessor::newDarkImage(QxrdImageData *image)
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

void QxrdDataProcessor::newBadPixelsImage(QxrdImageData *image)
{
  if (m_BadPixels != image) {
    if (m_BadPixels) {
      returnImageToPool(m_BadPixels);
    }

    m_BadPixels = image;
  }

  set_BadPixelsPath(image->get_FileName());
}

void QxrdDataProcessor::newGainMapImage(QxrdImageData *image)
{
  if (m_GainFrame != image) {
    if (m_GainFrame) {
      returnImageToPool(m_GainFrame);
    }

    m_GainFrame = image;
  }

  set_GainMapPath(image->get_FileName());
}


void QxrdDataProcessor::onProcessedImageAvailable(QxrdImageData *image)
{
//  printf("onProcessedImageAvailable()\n");

  QxrdImageData* img = takeLatestProcessedImage();

  if (img) {
    newData(img);
  }
}

void QxrdDataProcessor::onDarkImageAvailable(QxrdImageData *image)
{
//  printf("onDarkImageAvailable()\n");

  QxrdImageData* img = takeNextDarkImage();

  if (img) {
    newDarkImage(img);
  }
}

void QxrdDataProcessor::loadData(QString name)
{
//  printf("QxrdDataProcessor::loadData(%s)\n", qPrintable(name));

  QxrdImageData* res = takeNextFreeImage();

  res -> readImage(name);

//  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

  newData(res);
}

void QxrdDataProcessor::saveData(QString name)
{
  QMutexLocker lock(&m_Mutex);

  m_Data -> set_FileName(name);

  saveImageData(m_Data);
}

void QxrdDataProcessor::saveImageData(QxrdImageData *image)
{
  saveNamedImageData(image->get_FileName(), image);
}

void QxrdDataProcessor::saveRawData(QxrdImageData *image)
{
  saveNamedImageData(image->rawFileName(), image);
}

void QxrdDataProcessor::saveNamedImageData(QString name, QxrdImageData *image)
{
//  emit printMessage(tr("Saved \"%1\")").arg(name));

  QReadLocker lock(image->rwLock());

  int nrows = image -> get_Height();
  int ncols = image -> get_Width();

  TIFF* tif = TIFFOpen(qPrintable(name),"w");

  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32);
  TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);

  QVector<float> buffvec(4096);
  float* buffer = buffvec.data();

  for (int y=0; y<nrows; y++) {
    for (int x=0; x<ncols; x++) {
      buffer[x] = image->value(x,y);
    }

    TIFFWriteScanline(tif, buffer, y, 0);
  }

  TIFFClose(tif);
}

void QxrdDataProcessor::loadDarkImage(QString name)
{
  QxrdImageData* img = takeNextFreeImage();

  img -> readImage(name);

  newDarkImage(img);
}

void QxrdDataProcessor::loadBadPixels(QString name)
{
  QxrdImageData* res = takeNextFreeImage();

  res -> readImage(name);

  newBadPixelsImage(res);
}

void QxrdDataProcessor::loadGainMap(QString name)
{
  QxrdImageData* res = takeNextFreeImage();

  res -> readImage(name);

  newGainMapImage(res);
}

void QxrdDataProcessor::processAcquiredImage(QxrdImageData *img)
{
//  printf("QxrdDataProcessor::processAcquiredImage\n");

  QxrdImageData *dark   = darkImage();

  if (img) {
    QTime tic;
    tic.start();

    subtractDarkImage(img, dark);
    emit printMessage(tr("Dark subtraction took %1 msec").arg(tic.elapsed()));
    m_DarkUsage.unlock();

    correctBadPixels(img);
    correctImageGains(img);

    emit printMessage(tr("Saving processed image in file \"%1\"").arg(img->get_FileName()));

    saveImageData(img);

    m_ProcessedImages.enqueue(img);

    emit processedImageAvailable(img);

    emit printMessage(tr("Processing took %1 msec").arg(tic.elapsed()));
  }

  m_Processing.unlock();
}

void QxrdDataProcessor::subtractDarkImage(QxrdImageData *image, QxrdImageData *dark)
{
  if (get_PerformDarkSubtraction()) {
    if (dark && get_SaveRawImages()) {
      emit printMessage(tr("Saving raw data in file \"%1\"").arg(image->rawFileName()));

      saveRawData(image);
    }

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

void QxrdDataProcessor::correctBadPixels(QxrdImageData */*image*/)
{
}

void QxrdDataProcessor::correctImageGains(QxrdImageData */*image*/)
{
}

void QxrdDataProcessor::showMaskRange(/*double min, double max*/)
{
  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  if (m_Data) {
//    printf ("clearMaskRange(%g,%g)\n", min, max);

    m_Data -> showMaskRange(min, max);

    newData(m_Data);
  }
}

void QxrdDataProcessor::hideMaskAll()
{
  if (m_Data) {
//    printf ("setMaskRange(%g,%g)\n", min, max);

    m_Data -> hideMaskAll();

    newData(m_Data);
  }
}

void QxrdDataProcessor::showMaskAll()
{
  if (m_Data) {
//    printf ("clearMaskRange(%g,%g)\n", min, max);

    m_Data -> showMaskAll();

    newData(m_Data);
  }
}

void QxrdDataProcessor::hideMaskRange(/*double min, double max*/)
{
  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  if (m_Data) {
//    printf ("setMaskRange(%g,%g)\n", min, max);

    m_Data -> hideMaskRange(min, max);

    newData(m_Data);
  }
}

QxrdImageData *QxrdDataProcessor::data() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Data;
}

QxrdImageData *QxrdDataProcessor::darkImage() const
{
  QMutexLocker lock(&m_Mutex);

  return m_DarkFrame;
}

/******************************************************************
*
*  $Log: qxrddataprocessor.cpp,v $
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

