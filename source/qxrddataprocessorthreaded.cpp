#include "qxrddebug.h"
#include "qxrdmutexlocker.h"
#include "qxrddataprocessorthreaded.h"
#include "qxrdintegrator.h"
#include "qxrdcenterfinder.h"
#include "qxrdimagedata.h"
#include "qxrdallocator.h"
#include "qxrdfilesaverthread.h"
#include "qxrdfilesaver.h"
#include "qxrdapplication.h"
#include "qxrdroidata.h"
#include "qxrdintegrateddata.h"
#include "qxrdhistogramdata.h"
#include "qxrdexperiment.h"
#include <QtConcurrentRun>
#include <QDirIterator>

QxrdDataProcessorThreaded::QxrdDataProcessorThreaded(
    QxrdSettingsSaverWPtr saver,
    QxrdExperimentWPtr    doc,
    QxrdAcquisitionWPtr   acq,
    QxrdAllocatorWPtr allocator,
    QxrdFileSaverWPtr filesaver)
  : QxrdDataProcessorBase(saver, doc, acq, allocator, filesaver),
    m_CorrectedImages(prop_CorrectionQueueLength()),
    m_IntegratedData(prop_IntegrationQueueLength())
{
  connect(&m_CorrectedImages, SIGNAL(resultAvailable()), this, SLOT(onCorrectedImageAvailable()));
  connect(&m_IntegratedData,  SIGNAL(resultAvailable()), this, SLOT(onIntegratedDataAvailable()));
  connect(&m_ROIData,         SIGNAL(resultAvailable()), this, SLOT(onROIDataAvailable()));
  connect(&m_HistogramData,   SIGNAL(resultAvailable()), this, SLOT(onHistogramDataAvailable()));

//  prop_CorrectionQueueLength()->setDebug(true);
//  prop_IntegrationQueueLength()->setDebug(true);
}

QxrdDataProcessorThreaded::~QxrdDataProcessorThreaded()
{
}

void QxrdDataProcessorThreaded::beginAcquisition(int /*isDark*/)
{
}

void QxrdDataProcessorThreaded::idleInt16Image(QxrdInt16ImageDataPtr image, bool liveView)
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

  if (liveView) {
    if (qcepDebug(DEBUG_PROCESS)) {
      printMessage("Image Live View");
    }

    QxrdDoubleImageDataPtr corrected = takeNextFreeImage(image->get_Width(), image->get_Height());
    QxrdDoubleImageDataPtr dark      = darkImage();

    corrected->copyFrom(image);
    subtractDarkImage(corrected, dark);

    newData(corrected, QxrdMaskDataPtr());

    m_LiveData = corrected;
  }
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
    printMessage(tr("QxrdDataProcessorThreaded::correctInt16Image"));
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
    printMessage(tr("QxrdDataProcessorThreaded::correctInt32Image"));
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
    printMessage(tr("QxrdDataProcessorThreaded::correctDoubleImage"));
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
    printMessage(tr("QxrdDataProcessorThreaded::correctDoubleImage"));
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
  QxrdIntegratedDataPtr integ = QxrdAllocator::newIntegratedData(m_Allocator, QxrdAllocator::AlwaysAllocate, img);

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
    printMessage(tr("QxrdDataProcessorThreaded::integrateImage"));
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
    printMessage(tr("QxrdDataProcessorThreaded::onIntegratedDataAvailable"));
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
    printMessage(tr("QxrdDataProcessorThreaded::calculateROI"));
  }

  return QxrdROIDataPtr();
}

void QxrdDataProcessorThreaded::onROIDataAvailable()
{
  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("QxrdDataProcessorThreaded::onROIDataAvailable"));
  }

  QxrdROIDataPtr roiData = m_ROIData.dequeue();
}

QxrdHistogramDataPtr QxrdDataProcessorThreaded::calculateHistogram
    (QxrdDoubleImageDataPtr /*image*/, QxrdMaskDataPtr /*mask*/)
{
  QThread::currentThread()->setObjectName("calculateHistogram");

  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("QxrdDataProcessorThreaded::calculateHistogram"));
  }

  return QxrdHistogramDataPtr();
}

void QxrdDataProcessorThreaded::onHistogramDataAvailable()
{
  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("QxrdDataProcessorThreaded::onHistogramDataAvailable"));
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
    QString path = filePathInDataDirectory(name);

    if (img->readImage(path)) {
      printMessage(tr("Load image from %1").arg(path));
      statusMessage(tr("Load image from %1").arg(path));

      img -> loadMetaData();

      if (first) {
        summed->copyFrom(img);
        first = false;
      } else {
        summed->accumulateImage(img);
      }
    } else {
      printMessage(tr("Couldn't load %1").arg(path));
      statusMessage(tr("Couldn't load %1").arg(path));
    }
  }

  if (first) {
    printMessage(tr("No images were loaded"));
    statusMessage(tr("No images were loaded"));
  } else {
    acquiredDoubleImage(summed, QxrdMaskDataPtr());
  }
}

void QxrdDataProcessorThreaded::integrateData(QString name)
{
  QThread::currentThread()->setObjectName("integrateData");

  QxrdDoubleImageDataPtr img;
  QxrdIntegratedDataPtr  result;

  QxrdAllocator::newDoubleImageAndIntegratedData(m_Allocator, QxrdAllocator::AlwaysAllocate, 0,0, img, result);

  QString path = filePathInDataDirectory(name);

  if (img -> readImage(path)) {
    printMessage(tr("Load image from %1").arg(path));
    statusMessage(tr("Load image from %1").arg(path));

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    img -> loadMetaData();

    m_IntegratedData.enqueue(QtConcurrent::run(this, &QxrdDataProcessorThreaded::integrateImage,
                                               result,
                                               img, mask(),
                                               centerFinder() -> get_CenterX(),
                                               centerFinder() -> get_CenterY()));
  } else {
    printMessage(tr("Couldn't load %1").arg(path));
    statusMessage(tr("Couldn't load %1").arg(path));
  }
}

void QxrdDataProcessorThreaded::processData(QString name)
{
  QxrdDoubleImageDataPtr res = takeNextFreeImage(0,0);

  QString path = filePathInDataDirectory(name);

  if (res -> readImage(path)) {
    printMessage(tr("Load image from %1").arg(path));
    statusMessage(tr("Load image from %1").arg(path));

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();

    acquiredDoubleImage(res, /*darkImage(), mask(),*/ QxrdMaskDataPtr());

    set_DataPath(res -> get_FileName());
  } else {
    printMessage(tr("Couldn't load %1").arg(path));
    statusMessage(tr("Couldn't load %1").arg(path));
  }
}

void QxrdDataProcessorThreaded::processDataSequence(QString path, QString filter)
{
  QDirIterator iter(path, QStringList(filter));

  while (iter.hasNext()) {
    QString path = iter.next();

    printMessage(path);

    processData(path);
  }
}

void QxrdDataProcessorThreaded::processDataSequence(QStringList paths)
{
  QString path;

  foreach(path, paths) {
    printMessage(path);

    processData(path);
  }
}

void QxrdDataProcessorThreaded::processDataSequence(QString path, QStringList filters)
{
  QDirIterator iter(path, filters);

  while (iter.hasNext()) {
    QString path = iter.next();

    printMessage(path);

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

  QString path = filePathInDataDirectory(name);

  if (res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> setMask(mask(), QxrdMaskDataPtr());

    acquiredDoubleImage(res, /*darkImage(), mask(),*/ QxrdMaskDataPtr(), v);

    set_DataPath(res -> get_FileName());
  } else {
    printMessage(tr("Couldn't load %1").arg(path));
  }
}

void QxrdDataProcessorThreaded::setFileNormalization(QString path, double v1)
{
  QList<double> v;
  v << v1;

  setFileNormalization(path, v);
}

void QxrdDataProcessorThreaded::setFileNormalization(QString path, double v1, double v2)
{
  QList<double> v;
  v << v1 << v2;

  setFileNormalization(path, v);
}

void QxrdDataProcessorThreaded::setFileNormalization(QString name, QList<double> v)
{
  QxrdDoubleImageDataPtr res = takeNextFreeImage(0,0);

  QString path = filePathInDataDirectory(name);

  if (res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    QxrdExperimentPtr exp(m_Experiment);

    res -> loadMetaData();
    res -> setMask(mask(), QxrdMaskDataPtr());
    res -> set_Normalization(v);
    res -> saveMetaData(name, exp);
  } else {
    printMessage(tr("Couldn't load %1").arg(path));
  }
}

void QxrdDataProcessorThreaded::slicePolygon(QVector<QPointF> poly)
{
  QxrdIntegratedDataPtr integ = QxrdAllocator::newIntegratedData(m_Allocator, QxrdAllocator::WaitTillAvailable, data());

  m_IntegratedData.enqueue(
      QtConcurrent::run(m_Integrator.data(),
                        &QxrdIntegrator::slicePolygon,
                        integ,
                        m_Data, poly, 0));
}

void QxrdDataProcessorThreaded::integrateSaveAndDisplay()
{
  QxrdIntegratedDataPtr integ = QxrdAllocator::newIntegratedData(m_Allocator, QxrdAllocator::WaitTillAvailable, data());

  m_IntegratedData.enqueue(
      QtConcurrent::run(m_Integrator.data(),
                        &QxrdIntegrator::performIntegration,
                        integ,
                        data(), mask()));
}

void QxrdDataProcessorThreaded::fixupBadBackgroundSubtraction(QString imagePattern, int nImgExposures, QString darkPath, int nDarkExposures)
{
  QDirIterator imagePaths(dataDirectory(), QStringList(imagePattern));

  QxrdDoubleImageDataPtr dark = takeNextFreeImage(0,0);
  QString path = filePathInDataDirectory(darkPath);

  if (dark->readImage(path)) {
    dark->loadMetaData();

    int nFileDarkExposures = dark->get_SummedExposures();

    printMessage(tr("Loaded Dark image from %1 (%2 summed)").arg(path).arg(nFileDarkExposures));

    while (imagePaths.hasNext()) {
      QString imagePath=imagePaths.next();

      QString path = filePathInDataDirectory(imagePath);
      QxrdDoubleImageDataPtr image = takeNextFreeImage(0,0);

      if (image->readImage(path)) {
        image->loadMetaData();

        int nFileImageExposures = image->get_SummedExposures();

        printMessage(tr("Loaded image from %1 (%2 summed)").arg(path).arg(nFileImageExposures));

        image->correctBadBackgroundSubtraction(dark,nImgExposures,nDarkExposures);

        QxrdFileSaverPtr saver(m_FileSaver);

        if (saver) {
          saver->saveData(path, image, QxrdMaskDataPtr(), NoOverwrite);
        }
      } else {
        printMessage(tr("Failed to load image from %1").arg(path));
      }
    }
  } else {
    printMessage(tr("Failed to load Dark image from %1").arg(path));
  }
}
