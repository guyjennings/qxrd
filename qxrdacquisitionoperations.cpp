/******************************************************************
*
*  $Id: qxrdacquisitionoperations.cpp,v 1.6 2009/07/13 23:19:37 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionoperations.h"
#include "qxrdimagedata.h"
#include "qxrddataprocessor.h"

#include "tiffio.h"

QxrdAcquisitionOperations::QxrdAcquisitionOperations(/*QxrdAcquisitionThread *thread,*/ QxrdDataProcessor *proc)
  : QxrdAcquisitionScripting(proc),
    m_DataProcessor(proc),
    m_FreeImages("Free Image Pool"),
    m_AcquiredImages("Acquired Images"),
    m_Data(new QxrdImageData(2048,2048)),
    m_DarkFrame(NULL),
    m_BadPixels(NULL),
    m_GainFrame(NULL),
    SOURCE_IDENT("$Id: qxrdacquisitionoperations.cpp,v 1.6 2009/07/13 23:19:37 jennings Exp $")
{
}

QxrdImageData *QxrdAcquisitionOperations::takeNextFreeImage()
{
  QMutexLocker lock(&m_Mutex);

  if (m_FreeImages.size() == 0) {
//    printf("Allocate new image\n");
    return new QxrdImageData(2048, 2048);
  } else {
    return m_FreeImages.dequeue();
  }
}

QxrdImageData *QxrdAcquisitionOperations::takeNextAcquiredImage()
{
  QMutexLocker lock(&m_Mutex);

  return m_AcquiredImages.dequeue();
}

void QxrdAcquisitionOperations::newAcquiredImage(QxrdImageData *img)
{
  QMutexLocker lock(&m_Mutex);

  m_AcquiredImages.enqueue(img);

  emit acquiredImageAvailable();
}

void QxrdAcquisitionOperations::returnImageToPool(QxrdImageData *img)
{
  QMutexLocker lock(&m_Mutex);

  m_FreeImages.enqueue(img);
}

void QxrdAcquisitionOperations::loadData(QString name)
{
  QxrdImageData* res = takeNextFreeImage();

  res -> readImage(name);

  newData(res);
}

void QxrdAcquisitionOperations::saveData(QString name)
{
  QMutexLocker lock(&m_Mutex);

  m_Data -> set_FileName(name);

  saveImageData(m_Data);
}

void QxrdAcquisitionOperations::saveImageData(QxrdImageData *image)
{
  saveNamedImageData(image->get_FileName(), image);
}

void QxrdAcquisitionOperations::saveRawData(QxrdImageData *image)
{
  saveNamedImageData(image->rawFileName(), image);
}

void QxrdAcquisitionOperations::saveNamedImageData(QString name, QxrdImageData *image)
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

void QxrdAcquisitionOperations::loadDarkImage(QString name)
{
  QxrdImageData* img = takeNextFreeImage();

  img -> readImage(name);

  newDarkImage(img);
}

void QxrdAcquisitionOperations::loadBadPixels(QString name)
{
  QxrdImageData* res = takeNextFreeImage();

  res -> readImage(name);

  newBadPixelsImage(res);
}

void QxrdAcquisitionOperations::loadGainMap(QString name)
{
  QxrdImageData* res = takeNextFreeImage();

  res -> readImage(name);

  newGainMapImage(res);
}

void QxrdAcquisitionOperations::newData(QxrdImageData *image)
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
}

void QxrdAcquisitionOperations::newDarkImage(QxrdImageData *image)
{
  if (m_DarkFrame != image) {
    if (m_DarkFrame) {
      returnImageToPool(m_DarkFrame);
    }

    m_DarkFrame = image;
  }

  set_DarkImagePath(image->get_FileName());
}

void QxrdAcquisitionOperations::newBadPixelsImage(QxrdImageData *image)
{
  if (m_BadPixels != image) {
    if (m_BadPixels) {
      returnImageToPool(m_BadPixels);
    }

    m_BadPixels = image;
  }

  set_BadPixelsPath(image->get_FileName());
}

void QxrdAcquisitionOperations::newGainMapImage(QxrdImageData *image)
{
  if (m_GainFrame != image) {
    if (m_GainFrame) {
      returnImageToPool(m_GainFrame);
    }

    m_GainFrame = image;
  }

  set_GainMapPath(image->get_FileName());
}

void QxrdAcquisitionOperations::onProcessedImageAvailable()
{
//  printf("onProcessedImageAvailable()\n");

  QxrdImageData* img = m_DataProcessor -> takeLatestProcessedImage();

  if (img) {
    newData(img);
  }
}

void QxrdAcquisitionOperations::onDarkImageAvailable()
{
//  printf("onDarkImageAvailable()\n");

  QxrdImageData* img = m_DataProcessor -> takeNextDarkImage();

  if (img) {
    newDarkImage(img);
  }
}

void QxrdAcquisitionOperations::showMaskRange(double min, double max)
{
//  double min = m_MaskMinimum -> value();
//  double max = m_MaskMaximum -> value();

  if (m_Data) {
//    printf ("clearMaskRange(%g,%g)\n", min, max);

    m_Data -> showMaskRange(min, max);

    newData(m_Data);
  }
}

void QxrdAcquisitionOperations::hideMaskAll()
{
  if (m_Data) {
//    printf ("setMaskRange(%g,%g)\n", min, max);

    m_Data -> hideMaskAll();

    newData(m_Data);
  }
}

void QxrdAcquisitionOperations::showMaskAll()
{
  if (m_Data) {
//    printf ("clearMaskRange(%g,%g)\n", min, max);

    m_Data -> showMaskAll();

    newData(m_Data);
  }
}

void QxrdAcquisitionOperations::hideMaskRange(double min, double max)
{
//  double min = m_MaskMinimum -> value();
//  double max = m_MaskMaximum -> value();

  if (m_Data) {
//    printf ("setMaskRange(%g,%g)\n", min, max);

    m_Data -> hideMaskRange(min, max);

    newData(m_Data);
  }
}

QxrdImageData *QxrdAcquisitionOperations::data() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Data;
}

QxrdImageData *QxrdAcquisitionOperations::darkImage() const
{
  QMutexLocker lock(&m_Mutex);

  return m_DarkFrame;
}

/******************************************************************
*
*  $Log: qxrdacquisitionoperations.cpp,v $
*  Revision 1.6  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.5  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.4  2009/07/09 01:15:09  jennings
*  Added some locks
*
*  Revision 1.3  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

