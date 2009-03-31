#include "qxrddataprocessor.h"
#include <QtConcurrentRun>
#include "qxrdwindow.h"
#include "qxrdacquisitionthread.h"

#include <QTime>

QxrdDataProcessor::QxrdDataProcessor
    (QxrdWindow *win, QxrdAcquisitionThread *acq, QObject *parent)
  : QObject(parent),
    m_Window(win),
    m_AcquisitionThread(acq),
    m_DarkUsage(QReadWriteLock::Recursive),
    m_ProcessedImages("QxrdDataProcessor Processed Images"),
    m_DarkImages("QxrdDataProcessor Dark Images")
{
  connect(m_AcquisitionThread, SIGNAL(acquiredImageAvailable()), this, SLOT(on_acquired_image_available()));
}

void QxrdDataProcessor::on_acquired_image_available()
{
  printf("QxrdDataProcessor::on_acquired_image_available()\n");

  QxrdImageData *image = m_AcquisitionThread -> takeNextAcquiredImage();

  if (image) {
    printf("Frame Number %d\n", image -> frameNumber());

    if ((image -> frameNumber()) >= 0) {
      m_DarkUsage.lockForRead();
      m_Processing.lockForRead();
      QtConcurrent::run(this, &QxrdDataProcessor::processAcquiredImage, image);
    } else {
      QWriteLocker wl(&m_DarkUsage);

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
      m_AcquisitionThread -> returnImageToPool(res);
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
  printf("QxrdDataProcessor::processAcquiredImage\n");

  QxrdImageData *dark   = m_Window -> darkImage();

  if (img) {
    subtractDarkImage(img, dark);
    m_DarkUsage.unlock();

    correctBadPixels(img);
    correctImageGains(img);

    m_Window -> saveImageData(img);

    m_ProcessedImages.enqueue(img);

    emit processedImageAvailable();
  }

  m_Processing.unlock();
}

void QxrdDataProcessor::subtractDarkImage(QxrdImageData *image, QxrdImageData *dark)
{
  if (m_Window -> performDarkSubtraction()) {
    if (m_Window -> saveRawImages()) {
      m_Window -> saveRawData(image);
    }

    if (dark && image) {
      if (dark->integrationMode() != image->integrationMode()) {
        printf("Integration times of acquired data and dark image are different, skipping\n");
        return;
      }

      if (dark->width() != image->width() ||
          dark->height() != image->height()) {
        printf("Dimensions of acquired data and dark image are different, skipping\n");
        return;
      }

      QReadLocker lock1(dark->rwLock());
      QWriteLocker lock2(image->rwLock());

      int height = image->height();
      int width  = image->width();
      int nres = image->nSummed();
      int ndrk = dark -> nSummed();
      int npixels = width*height;

      double ratio = ((double) nres)/((double) ndrk);

      printf("Dark subtraction nres=%d, ndrk=%d, npixels=%d, ratio=%g\n",
             nres, ndrk, npixels, ratio);

      QTime tic;
      tic.start();

      double *result = image->data();
      double *dk     = dark->data();

      for (int i=0; i<npixels; i++) {
        result[i] = result[i]-ratio*dk[i];
      }

      printf("Dark subtraction took %d msec\n", tic.elapsed());
    }
  }
}

void QxrdDataProcessor::correctBadPixels(QxrdImageData *image)
{
}

void QxrdDataProcessor::correctImageGains(QxrdImageData *image)
{
}
