/******************************************************************
*
*  $Id: qxrddataprocessorthreaded.cpp,v 1.3 2010/10/21 19:44:03 jennings Exp $
*
*******************************************************************/

#include "qxrdmutexlocker.h"
#include "qxrddataprocessorthreaded.h"
#include "qxrdintegrator.h"
#include "qxrdcenterfinder.h"

#include <QtConcurrentRun>

QxrdDataProcessorThreaded::QxrdDataProcessorThreaded(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent)
  : QxrdDataProcessorBase(acq, allocator, saver, parent),
    SOURCE_IDENT("$Id: qxrddataprocessorthreaded.cpp,v 1.3 2010/10/21 19:44:03 jennings Exp $")
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

/******************************************************************
*
*  $Log: qxrddataprocessorthreaded.cpp,v $
*  Revision 1.3  2010/10/21 19:44:03  jennings
*  Adding code to display overflow pixels, removed cuda and simple processors
*
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.16  2010/07/21 19:39:08  jennings
*  Improved calculation of estimated processing time
*
*  Revision 1.1.2.15  2010/07/20 20:30:25  jennings
*  Added memory usage display to status bar
*  Improved calculation of processing timings
*
*  Revision 1.1.2.14  2010/06/23 21:44:42  jennings
*  Tracked frame correction times and integration times allowing for multiple threads
*
*  Revision 1.1.2.13  2010/06/22 23:37:33  jennings
*  Made most acquisition and processing output conditional
*
*  Revision 1.1.2.12  2010/06/18 18:47:34  jennings
*  Only do subsequent processing steps if data is available - fixes crash when doing
*  dark measurements.
*
*  Revision 1.1.2.11  2010/06/17 19:19:59  jennings
*  Added 'save dark' and 'skip at start' options
*
*  Revision 1.1.2.10  2010/06/17 18:38:03  jennings
*  Implemented multi-threaded processing, with serialization of saves/display operation
*
*  Revision 1.1.2.9  2010/06/17 16:02:35  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*  Revision 1.1.2.8  2010/06/15 20:18:54  jennings
*  First steps to adding CUDA support
*
*  Revision 1.1.2.7  2010/06/11 21:23:14  jennings
*  When calling integrator, pass data, dark and mask images explicitly.
*
*  Revision 1.1.2.6  2010/06/09 19:20:29  jennings
*  Removed references to the QxrdFileSaver - all public accesses now through QxrdFileSaverThread
*
*  Revision 1.1.2.5  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.1.2.4  2010/05/20 20:16:58  jennings
*  Added simple threaded processor implementation using QtConcurrent::run
*
*  Revision 1.1.2.3  2010/04/26 20:53:26  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.1.2.2  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.1.2.1  2010/04/21 19:48:41  jennings
*  Added QxrdAcquisitionAreaDetector, QxrdAcquisitionPilatus, QxrdDataProcessorCuda, QxrdDataProcessorThreaded
*  and QxrdPreferencesDialog - they don't do much yet, though.
*
*
*******************************************************************/

