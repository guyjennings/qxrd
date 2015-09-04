#include "qxrddebug.h"
#include "qcepmutexlocker.h"
#include "qxrddataprocessorthreaded.h"
#include "qxrdintegrator.h"
#include "qxrdcenterfinder.h"
#include "qcepimagedata.h"
#include "qcepallocator.h"
#include "qxrdfilesaverthread.h"
#include "qxrdfilesaver.h"
#include "qxrdapplication.h"
#include "qxrdroidata.h"
#include "qcepintegrateddata.h"
#include "qxrdhistogramdata.h"
#include "qxrdexperiment.h"
#include <QtConcurrentRun>
#include <QDirIterator>

QxrdDataProcessorThreaded::QxrdDataProcessorThreaded(QcepSettingsSaverWPtr saver,
    QxrdExperimentWPtr    doc,
    QxrdAcquisitionWPtr   acq,
    QxrdFileSaverWPtr filesaver)
  : QxrdDataProcessorBase(saver, doc, acq, filesaver),
    m_CorrectedImages(prop_CorrectionQueueLength(), this),
    m_IntegratedData(prop_IntegrationQueueLength(), this),
    m_ROIData(NULL, this),
    m_HistogramData(NULL, this)
{
  connect(&m_CorrectedImages, &QxrdResultSerializerBase::resultAvailable, this, &QxrdDataProcessorThreaded::onCorrectedImageAvailable);
  connect(&m_IntegratedData,  &QxrdResultSerializerBase::resultAvailable, this, &QxrdDataProcessorThreaded::onIntegratedDataAvailable);
  connect(&m_ROIData,         &QxrdResultSerializerBase::resultAvailable, this, &QxrdDataProcessorThreaded::onROIDataAvailable);
  connect(&m_HistogramData,   &QxrdResultSerializerBase::resultAvailable, this, &QxrdDataProcessorThreaded::onHistogramDataAvailable);

//  prop_CorrectionQueueLength()->setDebug(true);
//  prop_IntegrationQueueLength()->setDebug(true);
}

QxrdDataProcessorThreaded::~QxrdDataProcessorThreaded()
{
}

void QxrdDataProcessorThreaded::beginAcquisition(int /*isDark*/)
{
}

void QxrdDataProcessorThreaded::idleInt16Image(QcepInt16ImageDataPtr image, bool liveView)
{
  QcepMutexLocker lock(__FILE__, __LINE__, image->mutex());
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

    QcepDoubleImageDataPtr corrected = takeNextFreeImage(image->get_Width(), image->get_Height());
    QcepDoubleImageDataPtr dark      = darkImage();

    corrected->copyFrom(image);
    subtractDarkImage(corrected, dark);

    newData(corrected, QcepMaskDataPtr());

    m_LiveData = corrected;
  }
}

void QxrdDataProcessorThreaded::acquiredInt16Image(QcepInt16ImageDataPtr image, QcepMaskDataPtr overflow)
{
  QcepDoubleImageDataPtr corrected = takeNextFreeImage(image->get_Width(), image->get_Height());

  m_CorrectedImages.enqueue(QtConcurrent::run(this,
                                              &QxrdDataProcessorThreaded::correctInt16Image,
                                              corrected, image, darkImage(), mask(), overflow));
}

void QxrdDataProcessorThreaded::acquiredInt32Image(QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow)
{
  QcepDoubleImageDataPtr corrected = takeNextFreeImage(image->get_Width(), image->get_Height());

  m_CorrectedImages.enqueue(QtConcurrent::run(this,
                                              &QxrdDataProcessorThreaded::correctInt32Image,
                                              corrected, image, darkImage(), mask(), overflow));
}

void QxrdDataProcessorThreaded::acquiredDoubleImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow)
{
  QcepDoubleImageDataPtr corrected = takeNextFreeImage(image->get_Width(), image->get_Height());

  typedef QcepDoubleImageDataPtr (QxrdDataProcessorThreaded::*MFType)(QcepDoubleImageDataPtr, QcepDoubleImageDataPtr, QcepDoubleImageDataPtr, QcepMaskDataPtr, QcepMaskDataPtr);
  MFType p = &QxrdDataProcessorThreaded::correctDoubleImage;

  m_CorrectedImages.enqueue(QtConcurrent::run(this,
                                              p,
                                              corrected, image, darkImage(), mask(), overflow));
}

void QxrdDataProcessorThreaded::acquiredDoubleImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, QList<double> v)
{
  QcepDoubleImageDataPtr corrected = takeNextFreeImage(image->get_Width(), image->get_Height());

  typedef QcepDoubleImageDataPtr (QxrdDataProcessorThreaded::*MFType)(QcepDoubleImageDataPtr, QcepDoubleImageDataPtr, QcepDoubleImageDataPtr, QcepMaskDataPtr, QList<double>);
  MFType p = &QxrdDataProcessorThreaded::correctDoubleImage;

  m_CorrectedImages.enqueue(QtConcurrent::run(this,
                                              p,
                                              corrected, image, darkImage(), overflow, v));
}

QcepDoubleImageDataPtr QxrdDataProcessorThreaded::correctInt16Image
    (QcepDoubleImageDataPtr corrected, QcepInt16ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow)
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

  return QcepDoubleImageDataPtr();
}

QcepDoubleImageDataPtr QxrdDataProcessorThreaded::correctInt32Image
    (QcepDoubleImageDataPtr corrected, QcepInt32ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow)
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

  return QcepDoubleImageDataPtr();
}

QcepDoubleImageDataPtr QxrdDataProcessorThreaded::correctDoubleImage
    (QcepDoubleImageDataPtr corrected, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow)
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

  return QcepDoubleImageDataPtr();
}

QcepDoubleImageDataPtr QxrdDataProcessorThreaded::correctDoubleImage
    (QcepDoubleImageDataPtr corrected, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr overflow, QcepDoubleList v)
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

  return QcepDoubleImageDataPtr();
}

void QxrdDataProcessorThreaded::onCorrectedImageAvailable()
{
  QcepDoubleImageDataPtr img = m_CorrectedImages.dequeue();
  QcepMaskDataPtr mask = (img ? img->mask() : QcepMaskDataPtr());
  QcepIntegratedDataPtr integ = QcepAllocator::newIntegratedData(QcepAllocator::AlwaysAllocate, img, this);

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

QcepIntegratedDataPtr QxrdDataProcessorThreaded::integrateImage
    (QcepIntegratedDataPtr integ, QcepDoubleImageDataPtr image, QcepMaskDataPtr mask, double /*cx*/, double /*cy*/)
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

  return QcepIntegratedDataPtr();
}

void QxrdDataProcessorThreaded::onIntegratedDataAvailable()
{
  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("QxrdDataProcessorThreaded::onIntegratedDataAvailable"));
  }

  QcepIntegratedDataPtr integ = m_IntegratedData.dequeue();

  if (integ) {
    writeOutputScan(integ);
    displayIntegratedData(integ);

    if (get_AccumulateIntegrated2D()) {
      m_Integrator -> appendIntegration(get_AccumulateIntegratedName(), integ);
    }
  }
}

QxrdROIDataPtr QxrdDataProcessorThreaded::calculateROI
    (QcepDoubleImageDataPtr /*image*/, QcepMaskDataPtr /*mask*/)
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
    (QcepDoubleImageDataPtr /*image*/, QcepMaskDataPtr /*mask*/)
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

void QxrdDataProcessorThreaded::sumImages(QStringList names)
{
  QcepDoubleImageDataPtr summed = takeNextFreeImage(0,0);
  int first = true;

  foreach(QString name, names) {
    QcepDoubleImageDataPtr img = takeNextFreeImage(0,0);
    QString path = filePathInDataDirectory(name);

    if (img->readImage(path)) {
      printMessage(tr("Load image from %1").arg(path));
      statusMessage(tr("Load image from %1").arg(path));

      img -> loadMetaData();

      int typ = img->get_DataType();

      if ((typ == QcepDoubleImageData::Raw16Data) ||
          (typ == QcepDoubleImageData::Raw32Data))
      {
        subtractDarkImage(img, darkImage());
      }

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
    newData(summed, QcepMaskDataPtr());
  }
}

void QxrdDataProcessorThreaded::addImages(QStringList names)
{
  QcepDoubleImageDataPtr summed = data();

  foreach(QString name, names) {
    QcepDoubleImageDataPtr img = takeNextFreeImage(0,0);
    QString path = filePathInDataDirectory(name);

    if (img->readImage(path)) {
      printMessage(tr("Load image from %1").arg(path));
      statusMessage(tr("Load image from %1").arg(path));

      img -> loadMetaData();

      int typ = img->get_DataType();

      if ((typ == QcepDoubleImageData::Raw16Data) ||
          (typ == QcepDoubleImageData::Raw32Data))
      {
        subtractDarkImage(img, darkImage());
      }

      summed->add(img);
    } else {
      printMessage(tr("Couldn't load %1").arg(path));
      statusMessage(tr("Couldn't load %1").arg(path));
    }
  }

  newData(summed, QcepMaskDataPtr());
}

void QxrdDataProcessorThreaded::subtractImages(QStringList names)
{
  QcepDoubleImageDataPtr summed = data();

  foreach(QString name, names) {
    QcepDoubleImageDataPtr img = takeNextFreeImage(0,0);
    QString path = filePathInDataDirectory(name);

    if (img->readImage(path)) {
      printMessage(tr("Load image from %1").arg(path));
      statusMessage(tr("Load image from %1").arg(path));

      img -> loadMetaData();

      int typ = img->get_DataType();

      if ((typ == QcepDoubleImageData::Raw16Data) ||
          (typ == QcepDoubleImageData::Raw32Data))
      {
        subtractDarkImage(img, darkImage());
      }

      summed->subtract(img);
    } else {
      printMessage(tr("Couldn't load %1").arg(path));
      statusMessage(tr("Couldn't load %1").arg(path));
    }
  }

  newData(summed, QcepMaskDataPtr());
}

void QxrdDataProcessorThreaded::clearAccumulator()
{
  m_Integrator -> clearAccumulator(get_AccumulateIntegratedName());
}

void QxrdDataProcessorThreaded::integrateAndAccumulate(QStringList names)
{
  int nImages   = names.count();
  m_Integrator -> prepareAccumulator(get_AccumulateIntegratedName(), nImages);

  foreach(QString name, names) {
    QcepDoubleImageDataPtr img = takeNextFreeImage(0,0);
    QString path = filePathInDataDirectory(name);

    if (img->readImage(path)) {
      printMessage(tr("Load image from %1").arg(path));
      statusMessage(tr("Load image from %1").arg(path));

      img -> loadMetaData();

      m_Integrator -> appendIntegration(get_AccumulateIntegratedName(), img, mask());
    } else {
      printMessage(tr("Couldn't load %1").arg(path));
      statusMessage(tr("Couldn't load %1").arg(path));
    }
  }

  m_Integrator -> completeAccumulator(get_AccumulateIntegratedName());
}

void QxrdDataProcessorThreaded::saveAccumulator(QString &path, QString filter)
{
  m_Integrator -> saveAccumulator(get_AccumulateIntegratedName(), path, filter);
}

void QxrdDataProcessorThreaded::reflectHorizontally()
{
  QcepDoubleImageDataPtr image = data();

  if (image) {
    int wid = image->get_Width();
    int hgt = image->get_Height();

    for (int y=0; y<hgt; y++) {
      for (int x=0; x<wid/2; x++) {
        double val1 = image->getImageData(x,y);
        double val2 = image->getImageData(wid-x-1, y);

        image->setValue(wid-x-1, y, val1);
        image->setValue(x,y, val2);
      }
    }
  }

  newData(image, QcepMaskDataPtr());
}

void QxrdDataProcessorThreaded::reflectVertically()
{
  QcepDoubleImageDataPtr image = data();

  if (image) {
    int wid = image->get_Width();
    int hgt = image->get_Height();

    for (int x=0; x<wid; x++) {
      for (int y=0; y<hgt/2; y++) {
        double val1 = image->getImageData(x,y);
        double val2 = image->getImageData(x, hgt-y-1);

        image->setValue(x, hgt-y-1, val1);
        image->setValue(x,y, val2);
      }
    }
  }

  newData(image, QcepMaskDataPtr());
}

void QxrdDataProcessorThreaded::projectImages(QStringList names, int px, int py, int pz)
{
  QcepDoubleImageDataPtr sumx, sumy, sumz;

  int nx = 0;
  int ny = 0;
  int nz = names.count();
  int first = true;

  if (px) {
    sumx = takeNextFreeImage(0,0);
    printMessage(tr("Projecting %1 images onto X").arg(nz));
  }

  if (py) {
    sumy = takeNextFreeImage(0,0);
    printMessage(tr("Projecting %1 images onto Y").arg(nz));
  }

  if (pz) {
    sumz = takeNextFreeImage(0,0);
    printMessage(tr("Projecting %1 images onto Z").arg(nz));
  }

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    expt->commenceWork(nz);
  }

  for (int i=0; i<nz; i++) {
    QcepDoubleImageDataPtr img = takeNextFreeImage(0,0);
    QString path = filePathInDataDirectory(names[i]);

    if (img->readImage(path)) {
      printMessage(tr("Load image from %1").arg(path));
      statusMessage(tr("Load image from %1").arg(path));

      img->loadMetaData();
      int typ = img->get_DataType();

      if ((typ == QcepDoubleImageData::Raw16Data) ||
          (typ == QcepDoubleImageData::Raw32Data))
      {
        subtractDarkImage(img, darkImage());
      }

      if (first) {
        nx = img->get_Width();
        ny = img->get_Height();

        if (px) {
          sumx->copyPropertiesFrom(img);
          sumx->resize(nz,ny);
          sumx->clear();
          sumx->set_SummedExposures(0);
        }

        if (py) {
          sumy->copyPropertiesFrom(img);
          sumy->resize(nz,nx);
          sumy->clear();
          sumy->set_SummedExposures(0);
        }

        if (pz) {
          sumz->copyPropertiesFrom(img);
          sumz->resize(nx,ny);
          sumz->clear();
          sumz->set_SummedExposures(0);
        }

        first = false;
      }

      if (px) {
        sumx->prop_SummedExposures()->incValue(1);

        for (int y=0; y<ny; y++) {
          double sum=0;

          for (int x=0; x<nx; x++) {
            sum += img->getImageData(x,y);
          }

          sumx->addValue(i,y, sum);
        }
      }

      if (py) {
        sumy->prop_SummedExposures()->incValue(1);

        for (int x=0; x<nx; x++) {
          double sum=0;

          for (int y=0; y<ny; y++) {
            sum += img->getImageData(x,y);
          }

          sumy->addValue(i,x, sum);
        }
      }

      if (pz) {
        sumz->prop_SummedExposures()->incValue(1);

        for (int x=0; x<nx; x++) {
          for (int y=0; y<ny; y++) {
            sumz->addValue(x,y, img->getImageData(x,y));
          }
        }
      }
    }

    if (expt) {
      expt->completeWork(1);
    }
  }

  if (px) {
    newData(sumx, QcepMaskDataPtr());
  }

  if (py) {
    newData(sumy, QcepMaskDataPtr());
  }

  if (pz) {
    newData(sumz, QcepMaskDataPtr());
  }

  if (expt) {
    expt->finishedWork(nz);
  }
}

void QxrdDataProcessorThreaded::correlateImages(QStringList names)
{
  QcepDoubleImageDataPtr imga = data();

  foreach(QString name, names) {
    QcepDoubleImageDataPtr imgb = takeNextFreeImage(0,0);
    QString path = filePathInDataDirectory(name);

    if (imgb->readImage(path)) {
      printMessage(tr("Load image from %1").arg(path));
      statusMessage(tr("Load image from %1").arg(path));

      imgb -> loadMetaData();

      int typ = imgb->get_DataType();

      if ((typ == QcepDoubleImageData::Raw16Data) ||
          (typ == QcepDoubleImageData::Raw32Data))
      {
        subtractDarkImage(imgb, darkImage());
      }

      printMessage("dx\tdy\tcorr");

      for (int dy = -10; dy<=10; dy++) {
        for (int dx = -10; dx<=10; dx++) {
          double corr = imga->correlate(imgb, dx, dy, 10, 10);

          printMessage(tr("%1\t%2\t%3").arg(dx).arg(dy).arg(corr));
        }
      }
    } else {
      printMessage(tr("Couldn't load %1").arg(path));
      statusMessage(tr("Couldn't load %1").arg(path));
    }
  }
}

void QxrdDataProcessorThreaded::shiftImage(int dx, int dy)
{
  QcepDoubleImageDataPtr img = data();

  if (img) {
    QcepDoubleImageDataPtr shft = takeNextFreeImage(img->get_Width(), img->get_Height());

    shft->shiftImage(img, dx, dy);

    newData(shft, QcepMaskDataPtr());
  }
}

void QxrdDataProcessorThreaded::integrateData(QString name)
{
  QThread::currentThread()->setObjectName("integrateData");

  QcepDoubleImageDataPtr img;
  QcepIntegratedDataPtr  result;

  QcepAllocator::newDoubleImageAndIntegratedData(QcepAllocator::AlwaysAllocate, 0,0, this, img, result);

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
  QcepDoubleImageDataPtr res = takeNextFreeImage(0,0);

  QString path = filePathInDataDirectory(name);

  if (res -> readImage(path)) {
    printMessage(tr("Load image from %1").arg(path));
    statusMessage(tr("Load image from %1").arg(path));

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();

    acquiredDoubleImage(res, /*darkImage(), mask(),*/ QcepMaskDataPtr());

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
  QcepDoubleImageDataPtr res = takeNextFreeImage(0,0);

  QString path = filePathInDataDirectory(name);

  if (res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> setMask(mask(), QcepMaskDataPtr());

    acquiredDoubleImage(res, /*darkImage(), mask(),*/ QcepMaskDataPtr(), v);

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
  QcepDoubleImageDataPtr res = takeNextFreeImage(0,0);

  QString path = filePathInDataDirectory(name);

  if (res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    QxrdExperimentPtr exp(m_Experiment);

    res -> loadMetaData();
    res -> setMask(mask(), QcepMaskDataPtr());
    res -> set_Normalization(v);
    res -> saveMetaData(name);
  } else {
    printMessage(tr("Couldn't load %1").arg(path));
  }
}

void QxrdDataProcessorThreaded::slicePolygon(QVector<QPointF> poly)
{
  QcepIntegratedDataPtr integ = QcepAllocator::newIntegratedData(QcepAllocator::WaitTillAvailable, data(), this);

  m_IntegratedData.enqueue(
      QtConcurrent::run(m_Integrator.data(),
                        &QxrdIntegrator::slicePolygon,
                        integ,
                        m_Data, poly, 0));
}

void QxrdDataProcessorThreaded::integrateSaveAndDisplay()
{
  if (qcepDebug(DEBUG_INTEGRATOR)) {
    printMessage(tr("processor.integrateSaveAndDisplay: %1, %2, %3")
                 .arg(data()->get_FileName()).arg(m_CenterFinder->get_CenterX()).arg(m_CenterFinder->get_CenterY()));
  }

  QcepIntegratedDataPtr integ = QcepAllocator::newIntegratedData(QcepAllocator::WaitTillAvailable, data(), this);

  m_IntegratedData.enqueue(
      QtConcurrent::run(m_Integrator.data(),
                        &QxrdIntegrator::performIntegration,
                        integ,
                        data(), mask()));
}

void QxrdDataProcessorThreaded::fixupBadBackgroundSubtraction(QString imagePattern, int nImgExposures, QString darkPath, int nDarkExposures)
{
  QDirIterator imagePaths(dataDirectory(), QStringList(imagePattern));

  QcepDoubleImageDataPtr dark = takeNextFreeImage(0,0);
  QString path = filePathInDataDirectory(darkPath);

  if (dark->readImage(path)) {
    dark->loadMetaData();

    int nFileDarkExposures = dark->get_SummedExposures();

    printMessage(tr("Loaded Dark image from %1 (%2 summed)").arg(path).arg(nFileDarkExposures));

    while (imagePaths.hasNext()) {
      QString imagePath=imagePaths.next();

      QString path = filePathInDataDirectory(imagePath);
      QcepDoubleImageDataPtr image = takeNextFreeImage(0,0);

      if (image->readImage(path)) {
        image->loadMetaData();

        int nFileImageExposures = image->get_SummedExposures();

        printMessage(tr("Loaded image from %1 (%2 summed)").arg(path).arg(nFileImageExposures));

        image->correctBadBackgroundSubtraction(dark,nImgExposures,nDarkExposures);

        QxrdFileSaverPtr saver(m_FileSaver);

        if (saver) {
          saver->saveDoubleData(path, image, QcepMaskDataPtr(), NoOverwrite);
        }
      } else {
        printMessage(tr("Failed to load image from %1").arg(path));
      }
    }
  } else {
    printMessage(tr("Failed to load Dark image from %1").arg(path));
  }
}

void QxrdDataProcessorThreaded::fitPeakNear(double x, double y)
{
  QxrdCenterFinderPtr cf(centerFinder());

  if (cf) {
    cf->fitPeakNear(x,y);
  }
}
