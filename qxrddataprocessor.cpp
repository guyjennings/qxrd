/******************************************************************
*
*  $Id: qxrddataprocessor.cpp,v 1.27 2009/08/08 20:14:55 jennings Exp $
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
    m_MaskCircleRadius(this, "maskCircleRadius", 10),
    m_MaskSetPixels(this, "maskSetPixels", true),
    m_Mutex(QMutex::Recursive),
    m_Acquisition(acq),
    m_DarkUsage(QReadWriteLock::Recursive),
//    m_ProcessedImages("QxrdDataProcessor Processed Images"),
//    m_DarkImages("QxrdDataProcessor Dark Images"),
    m_Data(new QxrdImageData(2048,2048)),
    m_DarkFrame(NULL),
    m_BadPixels(NULL),
    m_GainFrame(NULL),
    m_Mask(new QxrdMaskData(2048, 2048)),
    m_AcquiredCount(0),
    m_ProcessedCount(0),
    m_CenterFinder(NULL),
    m_Integrator(NULL),
    SOURCE_IDENT("$Id: qxrddataprocessor.cpp,v 1.27 2009/08/08 20:14:55 jennings Exp $")
{
  m_CenterFinder = new QxrdCenterFinder(this);
  m_Integrator   = new QxrdIntegrator(this, this);
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

  m_CenterFinder -> writeSettings(settings, section+"/centerfinder");
  m_Integrator   -> writeSettings(settings, section+"/integrator");
}

void QxrdDataProcessor::readSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  m_CenterFinder -> readSettings(settings, section+"/centerfinder");
  m_Integrator   -> readSettings(settings, section+"/integrator");
}

void QxrdDataProcessor::onAcquiredImageAvailable(QxrdImageData *image)
{
  int navail = decrementAcquiredCount();

  printf("QxrdDataProcessor::on_acquired_image_available(), navail = %d\n", navail);

//  QxrdImageData *image = m_Acquisition -> takeNextAcquiredImage();

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

//      m_DarkImages.enqueue(image);

      newDarkImage(image);
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

void QxrdDataProcessor::returnImageToPool(QxrdImageData *img)
{
  QMutexLocker lock(&m_Mutex);

  m_FreeImages.enqueue(img);
}

void QxrdDataProcessor::newData(QxrdImageData *image)
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

void QxrdDataProcessor::newMask(QxrdMaskData *mask)
{
  if (m_Mask != mask) {
    delete m_Mask;

    m_Mask = mask;
  }

  emit newMaskAvailable(m_Data, m_Mask);
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

//    m_ProcessedImages.enqueue(img);

    newData(img);

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

QxrdMaskData *QxrdDataProcessor::mask() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Mask;
}

int QxrdDataProcessor::incrementAcquiredCount()
{
  emit printMessage(tr("QxrdDataProcessor::incrementAcquiredCount m_AcquiredCount = %1").arg(m_AcquiredCount));

  return m_AcquiredCount.fetchAndAddOrdered(+1);
}

int QxrdDataProcessor::decrementAcquiredCount()
{
  emit printMessage(tr("QxrdDataProcessor::decrementAcquiredCount m_AcquiredCount = %1").arg(m_AcquiredCount));

  return m_AcquiredCount.fetchAndAddOrdered(-1);
}

int QxrdDataProcessor::getAcquiredCount()
{
  return m_AcquiredCount.fetchAndAddOrdered(0);
}

int QxrdDataProcessor::incrementProcessedCount()
{
  emit printMessage(tr("QxrdDataProcessor::incrementProcessedCount m_ProcessedCount = %1").arg(m_ProcessedCount));

  return m_ProcessedCount.fetchAndAddOrdered(+1);
}

int QxrdDataProcessor::decrementProcessedCount()
{
  emit printMessage(tr("QxrdDataProcessor::decrementProcessedCount m_ProcessedCount = %1").arg(m_ProcessedCount));

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

/******************************************************************
*
*  $Log: qxrddataprocessor.cpp,v $
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

