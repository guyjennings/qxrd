/******************************************************************
*
*  $Id: qxrddataprocessor.cpp,v 1.10 2009/06/30 21:35:05 jennings Exp $
*
*******************************************************************/

#include "qxrddataprocessor.h"
#include <QtConcurrentRun>
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdimagedata.h"

#include <QTime>

QxrdDataProcessor::QxrdDataProcessor
    (QxrdWindow *win, QxrdAcquisition *acq, QObject *parent)
  : QObject(parent),
    m_Window(win),
    m_Acquisition(acq),
    m_DarkUsage(QReadWriteLock::Recursive),
    m_ProcessedImages("QxrdDataProcessor Processed Images"),
    m_DarkImages("QxrdDataProcessor Dark Images"),
    SOURCE_IDENT("$Id: qxrddataprocessor.cpp,v 1.10 2009/06/30 21:35:05 jennings Exp $")
{
  connect(m_Acquisition, SIGNAL(acquiredImageAvailable()), this, SLOT(on_acquired_image_available()));
}

void QxrdDataProcessor::on_acquired_image_available()
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

      m_Window -> saveImageData(image);

      m_DarkImages.enqueue(image);

      emit darkImageAvailable();
    }
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
      m_Acquisition -> returnImageToPool(res);
    }

    res = img;
  }

  return res;
}

QxrdImageData *QxrdDataProcessor::takeNextDarkImage()
{
  return m_DarkImages.dequeue();
}

void QxrdDataProcessor::processAcquiredImage(QxrdImageData *img)
{
//  printf("QxrdDataProcessor::processAcquiredImage\n");

  QxrdImageData *dark   = m_Window -> darkImage();

  if (img) {
    QTime tic;
    tic.start();

    subtractDarkImage(img, dark);
    emit printMessage(tr("Dark subtraction took %1 msec").arg(tic.elapsed()));
    m_DarkUsage.unlock();

    correctBadPixels(img);
    correctImageGains(img);

    emit printMessage(tr("Saving processed image in file \"%1\"").arg(img->get_FileName()));

    m_Window -> saveImageData(img);

    m_ProcessedImages.enqueue(img);

    emit processedImageAvailable();

    emit printMessage(tr("Processing took %1 msec").arg(tic.elapsed()));
  }

  m_Processing.unlock();
}

void QxrdDataProcessor::subtractDarkImage(QxrdImageData *image, QxrdImageData *dark)
{
  if (m_Window -> performDarkSubtraction()) {
    if (dark && m_Window -> saveRawImages()) {
      emit printMessage(tr("Saving raw data in file \"%1\"").arg(image->rawFileName()));

      m_Window -> saveRawData(image);
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

/******************************************************************
*
*  $Log: qxrddataprocessor.cpp,v $
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

