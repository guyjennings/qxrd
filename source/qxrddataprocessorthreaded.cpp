#include "qxrdmutexlocker.h"
#include "qxrddataprocessorthreaded.h"
#include "qxrdintegrator.h"
#include "qxrdcenterfinder.h"
#include "qxrdimagedata.h"
#include "qxrdallocator.h"
#include "qxrdfilesaverthread.h"
#include "qxrdapplication.h"
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
  QxrdDoubleImageDataPtr corrected = takeNextFreeImage(image->get_Width(), image->get_Height());

  m_CorrectedImages.enqueue(QtConcurrent::run(this,
                                              &QxrdDataProcessorThreaded::correctInt16Image,
                                              corrected, image, darkImage(), mask(), overflow));
}

void QxrdDataProcessorThreaded::acquiredInt32Image(QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow)
{
  QxrdDoubleImageDataPtr corrected = takeNextFreeImage(image->get_Width(), image->get_Height());

  m_CorrectedImages.enqueue(QtConcurrent::run(this,
                                              &QxrdDataProcessorThreaded::correctInt32Image,
                                              corrected, image, darkImage(), mask(), overflow));
}

void QxrdDataProcessorThreaded::acquiredDoubleImage(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{
  QxrdDoubleImageDataPtr corrected = takeNextFreeImage(image->get_Width(), image->get_Height());

  typedef QxrdDoubleImageDataPtr (QxrdDataProcessorThreaded::*MFType)(QxrdDoubleImageDataPtr, QxrdDoubleImageDataPtr, QxrdDoubleImageDataPtr, QxrdMaskDataPtr, QxrdMaskDataPtr);
  MFType p = &QxrdDataProcessorThreaded::correctDoubleImage;

  m_CorrectedImages.enqueue(QtConcurrent::run(this,
                                              p,
                                              corrected, image, darkImage(), mask(), overflow));
}

void QxrdDataProcessorThreaded::acquiredDoubleImage(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, QList<double> v)
{
  QxrdDoubleImageDataPtr corrected = takeNextFreeImage(image->get_Width(), image->get_Height());

  typedef QxrdDoubleImageDataPtr (QxrdDataProcessorThreaded::*MFType)(QxrdDoubleImageDataPtr, QxrdDoubleImageDataPtr, QxrdDoubleImageDataPtr, QxrdMaskDataPtr, QList<double>);
  MFType p = &QxrdDataProcessorThreaded::correctDoubleImage;

  m_CorrectedImages.enqueue(QtConcurrent::run(this,
                                              p,
                                              corrected, image, darkImage(), overflow, v));
}

QxrdDoubleImageDataPtr QxrdDataProcessorThreaded::correctInt16Image
    (QxrdDoubleImageDataPtr corrected, QxrdInt16ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  QThread::currentThread()->setObjectName("correctInt16Image");

  if (qcepDebug(DEBUG_PROCESS)) {
    g_Application->printMessage(tr("QxrdDataProcessorThreaded::correctInt16Image"));
  }

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      return processAcquiredInt16Image(corrected, image, dark, mask, overflow);
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
    (QxrdDoubleImageDataPtr corrected, QxrdInt32ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  QThread::currentThread()->setObjectName("correctInt32Image");

  if (qcepDebug(DEBUG_PROCESS)) {
    g_Application->printMessage(tr("QxrdDataProcessorThreaded::correctInt32Image"));
  }

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      return processAcquiredInt32Image(corrected, image, dark, mask, overflow);
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
    (QxrdDoubleImageDataPtr corrected, QxrdDoubleImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  QThread::currentThread()->setObjectName("correctDoubleImage");

  if (qcepDebug(DEBUG_PROCESS)) {
    g_Application->printMessage(tr("QxrdDataProcessorThreaded::correctDoubleImage"));
  }

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      return processAcquiredDoubleImage(corrected, image, dark, mask, overflow);
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
    (QxrdDoubleImageDataPtr corrected, QxrdDoubleImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr overflow, QcepDoubleList v)
{
  QThread::currentThread()->setObjectName("correctDoubleImage");

  if (qcepDebug(DEBUG_PROCESS)) {
    g_Application->printMessage(tr("QxrdDataProcessorThreaded::correctDoubleImage"));
  }

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      return processAcquiredDoubleImage(corrected, image, dark, image->mask(), overflow, v);
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
  QxrdIntegratedDataPtr integ = m_Allocator->newIntegratedData(QxrdAllocator::AlwaysAllocate, img);

  if (img) {
    m_IntegratedData.enqueue(QtConcurrent::run(this, &QxrdDataProcessorThreaded::integrateImage,
                                               integ,
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
    (QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask, double /*cx*/, double /*cy*/)
{
  QThread::currentThread()->setObjectName("integrateImage");

  if (qcepDebug(DEBUG_PROCESS)) {
    g_Application->printMessage(tr("QxrdDataProcessorThreaded::integrateImage"));
  }

  if (image && get_PerformIntegration()) {
    QTime tic;
    tic.start();

    m_Integrator -> performIntegration(integ, image, mask);

    updateEstimatedTime(prop_PerformIntegrationTime(), tic.restart());

    return integ;
  }

  return QxrdIntegratedDataPtr();
}

void QxrdDataProcessorThreaded::onIntegratedDataAvailable()
{
  if (qcepDebug(DEBUG_PROCESS)) {
    g_Application->printMessage(tr("QxrdDataProcessorThreaded::onIntegratedDataAvailable"));
  }

  QxrdIntegratedDataPtr integ = m_IntegratedData.dequeue();

  if (integ) {
    writeOutputScan(integ);
    displayIntegratedData(integ);
  }
}

QxrdROIDataPtr QxrdDataProcessorThreaded::calculateROI
    (QxrdDoubleImageDataPtr /*image*/, QxrdMaskDataPtr /*mask*/)
{
  QThread::currentThread()->setObjectName("calculateROI");

  if (qcepDebug(DEBUG_PROCESS)) {
    g_Application->printMessage(tr("QxrdDataProcessorThreaded::calculateROI"));
  }

  return QxrdROIDataPtr();
}

void QxrdDataProcessorThreaded::onROIDataAvailable()
{
  if (qcepDebug(DEBUG_PROCESS)) {
    g_Application->printMessage(tr("QxrdDataProcessorThreaded::onROIDataAvailable"));
  }

  QxrdROIDataPtr roiData = m_ROIData.dequeue();
}

QxrdHistogramDataPtr QxrdDataProcessorThreaded::calculateHistogram
    (QxrdDoubleImageDataPtr /*image*/, QxrdMaskDataPtr /*mask*/)
{
  QThread::currentThread()->setObjectName("calculateHistogram");

  if (qcepDebug(DEBUG_PROCESS)) {
    g_Application->printMessage(tr("QxrdDataProcessorThreaded::calculateHistogram"));
  }

  return QxrdHistogramDataPtr();
}

void QxrdDataProcessorThreaded::onHistogramDataAvailable()
{
  if (qcepDebug(DEBUG_PROCESS)) {
    g_Application->printMessage(tr("QxrdDataProcessorThreaded::onHistogramDataAvailable"));
  }

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
  QxrdDoubleImageDataPtr summed = takeNextFreeImage(0,0);
  int first = true;

  foreach(QString name, names) {
    QxrdDoubleImageDataPtr img = takeNextFreeImage(0,0);
    QString path = filePathInCurrentDirectory(name);

    if (img->readImage(path)) {
      g_Application->printMessage(tr("Load image from %1").arg(path));
      g_Application->statusMessage(tr("Load image from %1").arg(path));
      img -> loadMetaData();

      if (first) {
        summed->copyFrom(img);
        first = false;
      } else {
        summed->accumulateImage(img);
      }
    } else {
      g_Application->printMessage(tr("Couldn't load %1").arg(path));
      g_Application->statusMessage(tr("Couldn't load %1").arg(path));
    }
  }

  if (first) {
    g_Application->printMessage(tr("No images were loaded"));
    g_Application->statusMessage(tr("No images were loaded"));
  } else {
    acquiredDoubleImage(summed, QxrdMaskDataPtr());
  }
}

void QxrdDataProcessorThreaded::integrateData(QString name)
{
  QThread::currentThread()->setObjectName("integrateData");

  QxrdDoubleImageDataPtr img;
  QxrdIntegratedDataPtr  result;

  m_Allocator->newDoubleImageAndIntegratedData(QxrdAllocator::AlwaysAllocate, 0,0, img, result);

  QString path = filePathInCurrentDirectory(name);

  if (img -> readImage(path)) {
    g_Application->printMessage(tr("Load image from %1").arg(path));
    g_Application->statusMessage(tr("Load image from %1").arg(path));

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    img -> loadMetaData();

    m_IntegratedData.enqueue(QtConcurrent::run(this, &QxrdDataProcessorThreaded::integrateImage,
                                               result,
                                               img, mask(),
                                               centerFinder() -> get_CenterX(),
                                               centerFinder() -> get_CenterY()));
  } else {
    g_Application->printMessage(tr("Couldn't load %1").arg(path));
    g_Application->statusMessage(tr("Couldn't load %1").arg(path));
  }
}

void QxrdDataProcessorThreaded::processData(QString name)
{
  QxrdDoubleImageDataPtr res = takeNextFreeImage(0,0);

  QString path = filePathInCurrentDirectory(name);

  if (res -> readImage(path)) {
    g_Application->printMessage(tr("Load image from %1").arg(path));
    g_Application->statusMessage(tr("Load image from %1").arg(path));

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();

    acquiredDoubleImage(res, /*darkImage(), mask(),*/ QxrdMaskDataPtr());

    set_DataPath(res -> get_FileName());
  } else {
    g_Application->printMessage(tr("Couldn't load %1").arg(path));
    g_Application->statusMessage(tr("Couldn't load %1").arg(path));
  }
}

void QxrdDataProcessorThreaded::processDataSequence(QString path, QString filter)
{
  QDirIterator iter(path, QStringList(filter));

  while (iter.hasNext()) {
    QString path = iter.next();

    g_Application->printMessage(path);

    processData(path);
  }
}

void QxrdDataProcessorThreaded::processDataSequence(QStringList paths)
{
  QString path;

  foreach(path, paths) {
    g_Application->printMessage(path);

    processData(path);
  }
}

void QxrdDataProcessorThreaded::processDataSequence(QString path, QStringList filters)
{
  QDirIterator iter(path, filters);

  while (iter.hasNext()) {
    QString path = iter.next();

    g_Application->printMessage(path);

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
  QxrdDoubleImageDataPtr res = takeNextFreeImage(0,0);

  QString path = filePathInCurrentDirectory(name);

  if (res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> setMask(mask(), QxrdMaskDataPtr());

    acquiredDoubleImage(res, /*darkImage(), mask(),*/ QxrdMaskDataPtr(), v);

    set_DataPath(res -> get_FileName());
  } else {
    g_Application->printMessage(tr("Couldn't load %1").arg(path));
  }
}

void QxrdDataProcessorThreaded::slicePolygon(QVector<QwtDoublePoint> poly)
{
  QxrdIntegratedDataPtr integ = m_Allocator->newIntegratedData(QxrdAllocator::WaitTillAvailable, data());

  m_IntegratedData.enqueue(
      QtConcurrent::run(m_Integrator,
                        &QxrdIntegrator::slicePolygon,
                        integ,
                        m_Data, poly, 0));
}

void QxrdDataProcessorThreaded::integrateSaveAndDisplay()
{
  QxrdIntegratedDataPtr integ = m_Allocator->newIntegratedData(QxrdAllocator::WaitTillAvailable, data());

  m_IntegratedData.enqueue(
      QtConcurrent::run(m_Integrator,
                        &QxrdIntegrator::performIntegration,
                        integ,
                        data(), mask()));
}

void QxrdDataProcessorThreaded::fixupBadBackgroundSubtraction(QString imagePattern, int nImgExposures, QString darkPath, int nDarkExposures)
{
  QDirIterator imagePaths(currentDirectory(), QStringList(imagePattern));

  QxrdDoubleImageDataPtr dark = takeNextFreeImage(0,0);
  QString path = filePathInCurrentDirectory(darkPath);

  if (dark->readImage(path)) {
    dark->loadMetaData();

    int nFileDarkExposures = dark->get_SummedExposures();

    g_Application->printMessage(tr("Loaded Dark image from %1 (%2 summed)").arg(path).arg(nFileDarkExposures));

    while (imagePaths.hasNext()) {
      QString imagePath=imagePaths.next();

      QString path = filePathInCurrentDirectory(imagePath);
      QxrdDoubleImageDataPtr image = takeNextFreeImage(0,0);

      if (image->readImage(path)) {
        image->loadMetaData();

        int nFileImageExposures = image->get_SummedExposures();

        g_Application->printMessage(tr("Loaded image from %1 (%2 summed)").arg(path).arg(nFileImageExposures));

        image->correctBadBackgroundSubtraction(dark,nImgExposures,nDarkExposures);

        fileSaverThread()->saveData(path, image, QxrdMaskDataPtr(), NoOverwrite);
      } else {
        g_Application->printMessage(tr("Failed to load image from %1").arg(path));
      }
    }
  } else {
    g_Application->printMessage(tr("Failed to load Dark image from %1").arg(path));
  }
}
