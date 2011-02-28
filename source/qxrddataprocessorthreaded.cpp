#include "qxrdmutexlocker.h"
#include "qxrddataprocessorthreaded.h"
#include "qxrdintegrator.h"
#include "qxrdcenterfinder.h"
#include "qxrdimagedata.h"

#include <QtConcurrentRun>
#include <QDirIterator>

QxrdDataProcessorThreaded::QxrdDataProcessorThreaded(QxrdAcquisition *acq, QxrdAllocator *allocator, QxrdFileSaverThread *saver, QObject *parent)
  : QxrdDataProcessorBase(acq, allocator, saver, parent)
{
  connect(&m_CorrectedImages, SIGNAL(resultAvailable()), this, SLOT(onCorrectedImageAvailable()));
  connect(&m_IntegratedData,  SIGNAL(resultAvailable()), this, SLOT(onIntegratedDataAvailable()));
  connect(&m_ROIData,         SIGNAL(resultAvailable()), this, SLOT(onROIDataAvailable()));
  connect(&m_HistogramData,   SIGNAL(resultAvailable()), this, SLOT(onHistogramDataAvailable()));
}

void QxrdDataProcessorThreaded::beginAcquisition(int /*isDark*/)
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

void QxrdDataProcessorThreaded::acquiredDoubleImage(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{
  typedef QxrdDoubleImageDataPtr (QxrdDataProcessorThreaded::*MFType)(QxrdDoubleImageDataPtr, QxrdDoubleImageDataPtr, QxrdMaskDataPtr, QxrdMaskDataPtr);
  MFType p = &QxrdDataProcessorThreaded::correctDoubleImage;
  m_CorrectedImages.enqueue(QtConcurrent::run(this, p, image, darkImage(), mask(), overflow));
}

void QxrdDataProcessorThreaded::acquiredDoubleImage(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, QList<double> v)
{
  typedef QxrdDoubleImageDataPtr (QxrdDataProcessorThreaded::*MFType)(QxrdDoubleImageDataPtr, QxrdDoubleImageDataPtr, QxrdMaskDataPtr, QxrdMaskDataPtr, QList<double>);
  MFType p = &QxrdDataProcessorThreaded::correctDoubleImage;
  m_CorrectedImages.enqueue(QtConcurrent::run(this, p, image, darkImage(), mask(), overflow, v));
}

QxrdDoubleImageDataPtr QxrdDataProcessorThreaded::correctInt16Image
    (QxrdInt16ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(QDateTime::currentDateTime(),
                               tr("QxrdDataProcessorThreaded::correctInt16Image"));
  );

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      return processAcquiredInt16Image(image, dark, mask, overflow);
    } else {
      if (get_SaveDarkImages()) {
        saveNamedImageData(image->get_FileName(), image, overflow);
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
             emit printMessage(QDateTime::currentDateTime(),
                               tr("QxrdDataProcessorThreaded::correctInt32Image"));
  );

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      return processAcquiredInt32Image(image, dark, mask, overflow);
    } else {
      if (get_SaveDarkImages()) {
        saveNamedImageData(image->get_FileName(), image, overflow);

        set_DarkImagePath(image->get_FileName());
      }

      newDarkImage(image);
    }
  }

  return QxrdDoubleImageDataPtr();
}

QxrdDoubleImageDataPtr QxrdDataProcessorThreaded::correctDoubleImage
    (QxrdDoubleImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(QDateTime::currentDateTime(),
                               tr("QxrdDataProcessorThreaded::correctDoubleImage"));
  );

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      return processAcquiredDoubleImage(image, dark, mask, overflow);
    } else {
      if (get_SaveDarkImages()) {
        saveNamedImageData(image->get_FileName(), image, overflow);

        set_DarkImagePath(image->get_FileName());
      }

      newDarkImage(image);
    }
  }

  return QxrdDoubleImageDataPtr();
}

QxrdDoubleImageDataPtr QxrdDataProcessorThreaded::correctDoubleImage
    (QxrdDoubleImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow, QcepDoubleList v)
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(QDateTime::currentDateTime(),
                               tr("QxrdDataProcessorThreaded::correctDoubleImage"));
  );

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      return processAcquiredDoubleImage(image, dark, mask, overflow, v);
    } else {
      if (get_SaveDarkImages()) {
        saveNamedImageData(image->get_FileName(), image, overflow);

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
    (QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask, double /*cx*/, double /*cy*/)
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(QDateTime::currentDateTime(),
                               tr("QxrdDataProcessorThreaded::integrateImage"));
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
             emit printMessage(QDateTime::currentDateTime(),
                               tr("QxrdDataProcessorThreaded::onIntegratedDataAvailable"));
  );

  QxrdIntegratedDataPtr integ = m_IntegratedData.dequeue();

  if (integ) {
    writeOutputScan(integ);
    displayIntegratedData(integ);
  }
}

QxrdROIDataPtr QxrdDataProcessorThreaded::calculateROI
    (QxrdDoubleImageDataPtr /*image*/, QxrdMaskDataPtr /*mask*/)
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(QDateTime::currentDateTime(),
                               tr("QxrdDataProcessorThreaded::calculateROI"));
  );

  return QxrdROIDataPtr();
}

void QxrdDataProcessorThreaded::onROIDataAvailable()
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(QDateTime::currentDateTime(),
                               tr("QxrdDataProcessorThreaded::onROIDataAvailable"));
  );

  QxrdROIDataPtr roiData = m_ROIData.dequeue();
}

QxrdHistogramDataPtr QxrdDataProcessorThreaded::calculateHistogram
    (QxrdDoubleImageDataPtr /*image*/, QxrdMaskDataPtr /*mask*/)
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(QDateTime::currentDateTime(),
                               tr("QxrdDataProcessorThreaded::calculateHistogram"));
  );

  return QxrdHistogramDataPtr();
}

void QxrdDataProcessorThreaded::onHistogramDataAvailable()
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(QDateTime::currentDateTime(),
                               tr("QxrdDataProcessorThreaded::onHistogramDataAvailable"));
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

void QxrdDataProcessorThreaded::accumulateImages(QStringList names)
{
  QxrdDoubleImageDataPtr summed = takeNextFreeImage();
  int first = true;

  foreach(QString name, names) {
    QxrdDoubleImageDataPtr img = takeNextFreeImage();
    QString path = filePathInCurrentDirectory(name);

    if (img->readImage(path)) {
      emit printMessage(QDateTime::currentDateTime(),
                        tr("Load image from %1").arg(path));
      emit statusMessage(QDateTime::currentDateTime(),
                        tr("Load image from %1").arg(path));
      img -> loadMetaData();

      if (first) {
        summed->copyFrom(img);
        first = false;
      } else {
        summed->accumulateImage(img);
      }
    } else {
      emit printMessage(QDateTime::currentDateTime(),
                        tr("Couldn't load %1").arg(path));
      emit statusMessage(QDateTime::currentDateTime(),
                        tr("Couldn't load %1").arg(path));
    }
  }

  if (first) {
    emit printMessage(QDateTime::currentDateTime(),
                      tr("No images were loaded"));
    emit statusMessage(QDateTime::currentDateTime(),
                      tr("No images were loaded"));
  } else {
    acquiredDoubleImage(summed, QxrdMaskDataPtr());
  }
}

void QxrdDataProcessorThreaded::integrateData(QString name)
{
  QxrdDoubleImageDataPtr res = takeNextFreeImage();

  QString path = filePathInCurrentDirectory(name);

  if (res -> readImage(path)) {
    emit printMessage(QDateTime::currentDateTime(),
                      tr("Load image from %1").arg(path));
    emit statusMessage(QDateTime::currentDateTime(),
                      tr("Load image from %1").arg(path));

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();

    m_IntegratedData.enqueue(QtConcurrent::run(this, &QxrdDataProcessorThreaded::integrateImage,
                                               res, mask(),
                                               centerFinder() -> get_CenterX(),
                                               centerFinder() -> get_CenterY()));
  } else {
    emit printMessage(QDateTime::currentDateTime(),
                      tr("Couldn't load %1").arg(path));
    emit statusMessage(QDateTime::currentDateTime(),
                      tr("Couldn't load %1").arg(path));
  }
}

void QxrdDataProcessorThreaded::processData(QString name)
{
  QxrdDoubleImageDataPtr res = takeNextFreeImage();

  QString path = filePathInCurrentDirectory(name);

  if (res -> readImage(path)) {
    emit printMessage(QDateTime::currentDateTime(),
                      tr("Load image from %1").arg(path));
    emit statusMessage(QDateTime::currentDateTime(),
                      tr("Load image from %1").arg(path));

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();

    acquiredDoubleImage(res, /*darkImage(), mask(),*/ QxrdMaskDataPtr());

    set_DataPath(res -> get_FileName());
  } else {
    emit printMessage(QDateTime::currentDateTime(),
                      tr("Couldn't load %1").arg(path));
    emit statusMessage(QDateTime::currentDateTime(),
                      tr("Couldn't load %1").arg(path));
  }
}

void QxrdDataProcessorThreaded::processDataSequence(QString path, QString filter)
{
  QDirIterator iter(path, QStringList(filter));

  while (iter.hasNext()) {
    QString path = iter.next();

    emit printMessage(QDateTime::currentDateTime(), path);

    processData(path);
  }
}

void QxrdDataProcessorThreaded::processDataSequence(QStringList paths)
{
  QString path;

  foreach(path, paths) {
    emit printMessage(QDateTime::currentDateTime(), path);

    processData(path);
  }
}

void QxrdDataProcessorThreaded::processDataSequence(QString path, QStringList filters)
{
  QDirIterator iter(path, filters);

  while (iter.hasNext()) {
    QString path = iter.next();

    emit printMessage(QDateTime::currentDateTime(), path);

    processData(path);
  }
}

void QxrdDataProcessorThreaded::processNormalizedFile(QString path, double v1)
{
  QList<double> v;
  v << v1;

  processNormalizedFile(path, v);
}

void QxrdDataProcessorThreaded::processNormalizedFile(QString path, double v1, double v2)
{
  QList<double> v;
  v << v1 << v2;

  processNormalizedFile(path, v);
}

void QxrdDataProcessorThreaded::processNormalizedFile(QString name, QList<double> v)
{
  QxrdDoubleImageDataPtr res = takeNextFreeImage();

  QString path = filePathInCurrentDirectory(name);

  if (res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();

    acquiredDoubleImage(res, /*darkImage(), mask(),*/ QxrdMaskDataPtr(), v);

    set_DataPath(res -> get_FileName());
  } else {
    emit printMessage(QDateTime::currentDateTime(),
                      tr("Couldn't load %1").arg(path));
  }
}

void QxrdDataProcessorThreaded::slicePolygon(QVector<QwtDoublePoint> poly)
{
  m_IntegratedData.enqueue(
      QtConcurrent::run(m_Integrator,
                        &QxrdIntegrator::slicePolygon,
                        m_Data, poly, 0));
}

void QxrdDataProcessorThreaded::integrateSaveAndDisplay()
{
  m_IntegratedData.enqueue(
      QtConcurrent::run(m_Integrator,
                        &QxrdIntegrator::performIntegration,
                        data(), mask()));
}

