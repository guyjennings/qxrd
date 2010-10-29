#include "qxrdmutexlocker.h"
#include "qxrddataprocessorthreaded.h"
#include "qxrdintegrator.h"
#include "qxrdcenterfinder.h"

#include <QtConcurrentRun>

QxrdDataProcessorThreaded::QxrdDataProcessorThreaded(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent)
  : QxrdDataProcessorBase(acq, allocator, saver, parent)
{
  connect(&m_CorrectedImages, SIGNAL(resultAvailable()), this, SLOT(onCorrectedImageAvailable()));
  connect(&m_IntegratedData,  SIGNAL(resultAvailable()), this, SLOT(onIntegratedDataAvailable()));
  connect(&m_ROIData,         SIGNAL(resultAvailable()), this, SLOT(onROIDataAvailable()));
  connect(&m_HistogramData,   SIGNAL(resultAvailable()), this, SLOT(onHistogramDataAvailable()));
}

void QxrdDataProcessorThreaded::beginAcquisition(int isDark)
{
}

void QxrdDataProcessorThreaded::idleInt16Image(QxrdInt16ImageDataPtr image)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, image->mutex());
  int height = image->get_Height();
  int width  = image->get_Width();
  int nres = image-> get_SummedExposures();
  int npixels = width*height;
  if (nres <= 0) nres = 1;
  double avgraw = 0;
  quint16 *img = image->data();

  for (int i=0; i<npixels; i++) {
    avgraw += *img++;
  }

  set_AverageRaw(avgraw/npixels/nres);
  set_Average(get_AverageRaw() - get_AverageDark());
}

void QxrdDataProcessorThreaded::acquiredInt16Image(QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow)
{
  typedef QxrdDoubleImageDataPtr (QxrdDataProcessorThreaded::*MFType)(QxrdInt16ImageDataPtr, QxrdDoubleImageDataPtr, QxrdMaskDataPtr, QxrdMaskDataPtr);
  MFType p = &QxrdDataProcessorThreaded::correctInt16Image;
  m_CorrectedImages.enqueue(QtConcurrent::run(this, p, image, darkImage(), mask(), overflow));
}

void QxrdDataProcessorThreaded::acquiredInt32Image(QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow)
{
  typedef QxrdDoubleImageDataPtr (QxrdDataProcessorThreaded::*MFType)(QxrdInt32ImageDataPtr, QxrdDoubleImageDataPtr, QxrdMaskDataPtr, QxrdMaskDataPtr);
  MFType p = &QxrdDataProcessorThreaded::correctInt32Image;
  m_CorrectedImages.enqueue(QtConcurrent::run(this, p, image, darkImage(), mask(), overflow));
}

QxrdDoubleImageDataPtr QxrdDataProcessorThreaded::correctInt16Image
    (QxrdInt16ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(tr("QxrdDataProcessorThreaded::correctInt16Image"));
  );

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      return processAcquiredInt16Image(image, dark, mask, overflow);
    } else {
      if (get_SaveDarkImages()) {
        saveNamedImageData(image->get_FileName(), image);
        set_DarkImagePath(image->get_FileName());
      }

      newDarkImage(image);
    }
  }

  return QxrdDoubleImageDataPtr();
}

QxrdDoubleImageDataPtr QxrdDataProcessorThreaded::correctInt32Image
    (QxrdInt32ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(tr("QxrdDataProcessorThreaded::correctInt32Image"));
  );

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      return processAcquiredInt32Image(image, dark, mask, overflow);
    } else {
      if (get_SaveDarkImages()) {
        saveNamedImageData(image->get_FileName(), image);

        set_DarkImagePath(image->get_FileName());
      }

      newDarkImage(image);
    }
  }

  return QxrdDoubleImageDataPtr();
}

void QxrdDataProcessorThreaded::onCorrectedImageAvailable()
{
  QxrdDoubleImageDataPtr img = m_CorrectedImages.dequeue();
  QxrdMaskDataPtr mask = (img ? img->mask() : QxrdMaskDataPtr());

  if (img) {
    m_IntegratedData.enqueue(QtConcurrent::run(this, &QxrdDataProcessorThreaded::integrateImage,
                                               img, mask,
                                               centerFinder() -> get_CenterX(),
                                               centerFinder() -> get_CenterY()));

    m_ROIData.enqueue(QtConcurrent::run(this, &QxrdDataProcessorThreaded::calculateROI,
                                        img, mask));

    m_HistogramData.enqueue(QtConcurrent::run(this, &QxrdDataProcessorThreaded::calculateHistogram,
                                              img, mask));
  }
}

QxrdIntegratedDataPtr QxrdDataProcessorThreaded::integrateImage
    (QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask, double cx, double cy)
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(tr("QxrdDataProcessorThreaded::integrateImage"));
  );

  if (image && get_PerformIntegration()) {
    QTime tic;
    tic.start();

    QxrdIntegratedDataPtr integ = m_Integrator -> performIntegration(image, mask);

    updateEstimatedTime(prop_PerformIntegrationTime(), tic.restart());

    return integ;
  }

  return QxrdIntegratedDataPtr();
}

void QxrdDataProcessorThreaded::onIntegratedDataAvailable()
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(tr("QxrdDataProcessorThreaded::onIntegratedDataAvailable"));
  );

  QxrdIntegratedDataPtr integ = m_IntegratedData.dequeue();

  if (integ) {
    if (get_SaveIntegratedData()) {
      m_Integrator -> saveIntegratedData(integ);
    }

    if (get_DisplayIntegratedData()) {
      m_Integrator -> displayIntegratedData(integ);
    }
  }
}

QxrdROIDataPtr QxrdDataProcessorThreaded::calculateROI
    (QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask)
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(tr("QxrdDataProcessorThreaded::calculateROI"));
  );

  return QxrdROIDataPtr();
}

void QxrdDataProcessorThreaded::onROIDataAvailable()
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(tr("QxrdDataProcessorThreaded::onROIDataAvailable"));
  );

  QxrdROIDataPtr roiData = m_ROIData.dequeue();
}

QxrdHistogramDataPtr QxrdDataProcessorThreaded::calculateHistogram
    (QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask)
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(tr("QxrdDataProcessorThreaded::calculateHistogram"));
  );

  return QxrdHistogramDataPtr();
}

void QxrdDataProcessorThreaded::onHistogramDataAvailable()
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(tr("QxrdDataProcessorThreaded::onHistogramDataAvailable"));
  );

  QxrdHistogramDataPtr histData = m_HistogramData.dequeue();
}

double QxrdDataProcessorThreaded::estimatedProcessingTime(double estSerialTime, double estParallelTime)
{
  int nThreads = QThreadPool::globalInstance()->maxThreadCount();

  if (nThreads >= 2) {
    return qMax(estSerialTime, estParallelTime/((double)nThreads));
  } else {
    return estSerialTime + estParallelTime;
  }
}
