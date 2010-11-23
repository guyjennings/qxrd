#include "qxrddataprocessorbase.h"
#include "qxrddataprocessorthread.h"
#include <QtConcurrentRun>
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdimagedata.h"
#include "qxrddoubleimagedata.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrator.h"
#include "qxrdmutexlocker.h"
#include "qxrdallocator.h"
#include "qxrdfilesaverthread.h"
#include "qxrdgeneratetestimage.h"
#include "qxrddataprocessoroptionsdialog.h"

#include <QTime>
#include <QPainter>
#include <math.h>

QxrdDataProcessorBase::QxrdDataProcessorBase
    (QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent)
  : QxrdDataProcessor(acq, allocator, saver, parent),
    m_Mutex(QMutex::Recursive),
    m_LogFileMutex(QMutex::Recursive),
    m_Window(NULL),
    m_Allocator(allocator),
    m_FileSaverThread(saver),
    m_Acquisition(acq),
    m_AcquiredInt16Images("acquiredInt16Images"),
    m_AcquiredInt32Images("acquiredInt32Images"),
    m_Data(allocator -> newDoubleImage()),
    m_DarkFrame(NULL),
    m_BadPixels(NULL),
    m_GainMap(NULL),
    m_Mask(allocator -> newMask()),
    m_AcquiredCount(0),
    m_CenterFinder(NULL),
    m_Integrator(NULL),
    m_InitialRingSetFitParameters(NULL),
    m_RefinedRingSetFitParameters(NULL),
    m_InitialRingSetData(NULL),
    m_RefinedRingSetData(NULL),
    m_GenerateTestImage(NULL),
    m_LogFile(NULL)
{
  m_CenterFinder = QxrdCenterFinderPtr(new QxrdCenterFinder(this));
  m_Integrator   = QxrdIntegratorPtr(new QxrdIntegrator(QxrdDataProcessorPtr(this), m_Allocator, this));
  m_GenerateTestImage = QxrdGenerateTestImagePtr(new QxrdGenerateTestImage(this, m_Allocator, this));
  m_InitialRingSetFitParameters = QxrdRingSetFitParametersPtr(new QxrdRingSetFitParameters(this));
  m_RefinedRingSetFitParameters = QxrdRingSetFitParametersPtr(new QxrdRingSetFitParameters(this));
  m_InitialRingSetData = QxrdRingSetSampledDataPtr(new QxrdRingSetSampledData(/*m_InitialRingSetFitParameters,*/ this));
  m_RefinedRingSetData = QxrdRingSetSampledDataPtr(new QxrdRingSetSampledData(/*m_RefinedRingSetFitParameters,*/ this));
}

QxrdFileSaverThreadPtr QxrdDataProcessorBase::fileSaverThread() const
{
  return m_FileSaverThread;
}

QxrdDataProcessorBase::~QxrdDataProcessorBase()
{
  closeLogFile();
}

void QxrdDataProcessorBase::setAcquisition(QxrdAcquisitionPtr acq)
{
  m_Acquisition = acq;

  connect(prop_SaveRawImages(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformDarkSubtraction(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformBadPixels(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformGainCorrection(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveSubtracted(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveAsText(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformIntegration(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_DisplayIntegratedData(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveIntegratedData(), SIGNAL(changedValue(bool)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformDarkSubtractionTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformBadPixelsTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformGainCorrectionTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveSubtractedTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveAsTextTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformIntegrationTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_DisplayIntegratedDataTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveIntegratedDataTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(m_Acquisition -> prop_SummedExposures(), SIGNAL(changedValue(int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(m_Acquisition -> prop_Raw16SaveTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
  connect(m_Acquisition -> prop_Raw32SaveTime(), SIGNAL(changedValue(double)), this, SLOT(updateEstimatedProcessingTime()));
}

void QxrdDataProcessorBase::setWindow(QxrdWindowPtr win)
{
  m_Window = win;
  newData(m_Data, QxrdMaskDataPtr());
  newMask();
}

void QxrdDataProcessorBase::writeSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
  QcepProperty::writeSettings(this, staticMetaObject.superClass(), section, settings);

  m_CenterFinder -> writeSettings(settings, section+"/centerfinder");
  m_Integrator   -> writeSettings(settings, section+"/integrator");
  m_InitialRingSetFitParameters -> writeSettings(settings, section+"/initialFit");
  m_RefinedRingSetFitParameters -> writeSettings(settings, section+"/refinedFit");
  m_InitialRingSetData -> writeSettings(settings, section+"/initialData");
  m_RefinedRingSetData -> writeSettings(settings, section+"/refinedData");
}

void QxrdDataProcessorBase::readSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  m_CenterFinder -> readSettings(settings, section+"/centerfinder");
  m_Integrator   -> readSettings(settings, section+"/integrator");
  m_InitialRingSetFitParameters -> readSettings(settings, section+"/initialFit");
  m_RefinedRingSetFitParameters -> readSettings(settings, section+"/refinedFit");
  m_InitialRingSetData -> readSettings(settings, section+"/initialData");
  m_RefinedRingSetData -> readSettings(settings, section+"/refinedData");

  newLogFile(get_LogFilePath());
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::takeNextFreeImage()
{
  QxrdDoubleImageDataPtr res = m_Allocator -> newDoubleImage();

  while (res == NULL) {
    emit printMessage(QDateTime::currentDateTime(), tr("Waiting for double image"));

    QxrdDataProcessorThread::msleep(500);

    res = m_Allocator -> newDoubleImage();
  }

  return res;
}

void QxrdDataProcessorBase::newData(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{
  m_Data = image;
  m_Overflow = overflow;

  m_Window -> newDataAvailable(m_Data, overflow);
}

void QxrdDataProcessorBase::newDarkImage(QxrdDoubleImageDataPtr image)
{
  m_DarkFrame = image;

  set_DarkImagePath(image->get_FileName());

  int height = image->get_Height();
  int width  = image->get_Width();
  int ndrk = image -> get_SummedExposures();
  int npixels = width*height;
  if (ndrk <= 0) ndrk = 1;

  double *dk     = image->data();
  double avgdark = 0;

  for (int i=0; i<npixels; i++) {
    avgdark  += dk[i];
  }
  set_AverageDark(avgdark/npixels/ndrk);
}

void QxrdDataProcessorBase::newDarkImage(QxrdInt16ImageDataPtr image)
{
  if (m_DarkFrame == NULL) {
    m_DarkFrame = takeNextFreeImage();  /*QxrdDoubleImageDataPtr(new QxrdDoubleImageData())*/;
  }

  m_DarkFrame -> copyFrom(image);
  newDarkImage(m_DarkFrame);

//  set_DarkImagePath(m_DarkFrame -> get_FileName());
}

void QxrdDataProcessorBase::newDarkImage(QxrdInt32ImageDataPtr image)
{
  if (m_DarkFrame == NULL) {
    m_DarkFrame = takeNextFreeImage();  /*QxrdDoubleImageDataPtr(new QxrdDoubleImageData())*/;
  }

  m_DarkFrame -> copyFrom(image);
  newDarkImage(m_DarkFrame);

//  set_DarkImagePath(m_DarkFrame -> get_FileName());
}

void QxrdDataProcessorBase::newBadPixelsImage(QxrdDoubleImageDataPtr image)
{
  m_BadPixels = image;

  set_BadPixelsPath(image->get_FileName());
}

void QxrdDataProcessorBase::newGainMapImage(QxrdDoubleImageDataPtr image)
{
  m_GainMap = image;

  set_GainMapPath(image->get_FileName());
}

void QxrdDataProcessorBase::newMask()
{
  m_Window -> newMaskAvailable(m_Mask);
}

void QxrdDataProcessorBase::loadDefaultImages()
{
  QString fileName = get_MaskPath();
  QFileInfo fileInfo(fileName);

  if (fileInfo.exists() && fileInfo.isFile()) {
    loadMask(fileName);
  }

  fileName = get_DarkImagePath();
  fileInfo.setFile(fileName);

  if (fileInfo.exists() && fileInfo.isFile()) {
    loadDark(fileName);
  }

  fileName = get_BadPixelsPath();
  fileInfo.setFile(fileName);

  if (fileInfo.exists() && fileInfo.isFile()) {
    loadBadPixels(fileName);
  }

  fileName = get_GainMapPath();
  fileInfo.setFile(fileName);

  if (fileInfo.exists() && fileInfo.isFile()) {
    loadGainMap(fileName);
  }
}

void QxrdDataProcessorBase::loadData(QString name)
{
  QxrdDoubleImageDataPtr res = takeNextFreeImage();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();

    newData(res, QxrdMaskDataPtr());

    set_DataPath(res -> get_FileName());
  }
}

void QxrdDataProcessorBase::saveData(QString name, int canOverwrite)
{
  saveNamedImageData(name, m_Data, canOverwrite);

  set_DataPath(m_Data -> get_FileName());
}

void QxrdDataProcessorBase::loadDark(QString name)
{
  QCEP_DEBUG(DEBUG_FILES,
             printf("QxrdDataProcessorBase::loadDark(%s)\n", qPrintable(name));
  );

  QxrdDoubleImageDataPtr res = takeNextFreeImage();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QxrdDoubleImageData::DarkData);

    newDarkImage(res);

    set_DarkImagePath(res -> get_FileName());
  } else {
    emit printMessage(QDateTime::currentDateTime(),
                      tr("loadDark(%1) failed").arg(name));
  }
}

void QxrdDataProcessorBase::saveDark(QString name, int canOverwrite)
{
  saveNamedImageData(name, m_DarkFrame, canOverwrite);

  set_DarkImagePath(m_DarkFrame -> get_FileName());
}

void QxrdDataProcessorBase::loadBadPixels(QString name)
{
  QxrdDoubleImageDataPtr res = takeNextFreeImage();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QxrdDoubleImageData::BadPixelsData);

    newBadPixelsImage(res);

    set_BadPixelsPath(res -> get_FileName());
  }
}

void QxrdDataProcessorBase::saveBadPixels(QString name, int canOverwrite)
{
  saveNamedImageData(name, m_BadPixels, canOverwrite);

  set_BadPixelsPath(m_BadPixels -> get_FileName());
}

void QxrdDataProcessorBase::loadGainMap(QString name)
{
  QxrdDoubleImageDataPtr res = takeNextFreeImage();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QxrdDoubleImageData::GainData);

    newGainMapImage(res);

    set_GainMapPath(res -> get_FileName());
  }
}

void QxrdDataProcessorBase::saveGainMap(QString name, int canOverwrite)
{
  saveNamedImageData(name, m_GainMap, canOverwrite);

  set_GainMapPath(m_GainMap -> get_FileName());
}

void QxrdDataProcessorBase::loadMask(QString name)
{
  QxrdMaskDataPtr res = m_Allocator -> newMask();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QxrdMaskData::MaskData);

    res -> copyMask(m_Mask);

    newMask();

    set_MaskPath(m_Mask -> get_FileName());
  }
}

void QxrdDataProcessorBase::saveMask(QString name, int canOverwrite)
{
  saveNamedMaskData(name, m_Mask, canOverwrite);

  set_MaskPath(m_Mask -> get_FileName());
}

void QxrdDataProcessorBase::saveNamedImageData(QString name, QxrdDoubleImageDataPtr image, int canOverwrite)
{
  fileSaverThread() -> saveData(name, image, canOverwrite);
}

void QxrdDataProcessorBase::saveNamedImageData(QString name, QxrdInt16ImageDataPtr image, int canOverwrite)
{
  fileSaverThread() -> saveData(name, image, canOverwrite);
}

void QxrdDataProcessorBase::saveNamedRawImageData(QString name, QxrdInt16ImageDataPtr image, int canOverwrite)
{
  fileSaverThread() -> saveRawData(name, image, canOverwrite);
}

void QxrdDataProcessorBase::saveNamedImageData(QString name, QxrdInt32ImageDataPtr image, int canOverwrite)
{
  fileSaverThread() -> saveData(name, image, canOverwrite);
}

void QxrdDataProcessorBase::saveNamedRawImageData(QString name, QxrdInt32ImageDataPtr image, int canOverwrite)
{
  fileSaverThread() -> saveRawData(name, image, canOverwrite);
}

void QxrdDataProcessorBase::saveNamedMaskData(QString name, QxrdMaskDataPtr image, int canOverwrite)
{
  fileSaverThread() -> saveData(name, image, canOverwrite);
}

void QxrdDataProcessorBase::saveNamedImageDataAsText(QString name, QxrdDoubleImageDataPtr image, int canOverwrite)
{
  fileSaverThread() -> saveTextData(name, image, canOverwrite);
}

void QxrdDataProcessorBase::clearDark()
{
  newDarkImage(QxrdDoubleImageDataPtr(NULL));

  set_DarkImagePath("");
}

void QxrdDataProcessorBase::clearBadPixels()
{
  newBadPixelsImage(QxrdDoubleImageDataPtr(NULL));

  set_BadPixelsPath("");
}

void QxrdDataProcessorBase::clearGainMap()
{
  newGainMapImage(QxrdDoubleImageDataPtr(NULL));

  set_GainMapPath("");
}

void QxrdDataProcessorBase::clearMask()
{
  m_Mask -> clear();

  newMask();

  set_MaskPath("");
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::processAcquiredInt16Image
    (QxrdInt16ImageDataPtr img, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(QDateTime::currentDateTime(),
                               tr("processing acquired 16 bit image, %1 remaining").arg(getAcquiredCount()));
  );

  if (img) {
    if (get_SaveRawImages()) {
      if (img->get_ImageSaved()) {
        emit printMessage(QDateTime::currentDateTime(),
                          tr("Image \"%1\" is already saved").arg(img->rawFileName()));
      } else {
        saveNamedRawImageData(img->rawFileName(), img, QxrdDataProcessorBase::NoOverwrite);
      }
    }

    QxrdDoubleImageDataPtr dimg = takeNextFreeImage();

    dimg -> copyFrom(img);
    dimg -> set_DateTime(QDateTime::currentDateTime());

    processAcquiredImage(dimg, dark, mask, overflow);

    return dimg;
  } else {
    return QxrdDoubleImageDataPtr();
  }
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::processAcquiredInt32Image
    (QxrdInt32ImageDataPtr img, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  QCEP_DEBUG(DEBUG_PROCESS,
             emit printMessage(QDateTime::currentDateTime(),
                               tr("processing acquired 32 bit image, %1 remaining").arg(getAcquiredCount()));
  );

  if (img) {
    if (get_SaveRawImages()) {
      if (img->get_ImageSaved()) {
        emit printMessage(QDateTime::currentDateTime(),
                          tr("Image \"%1\" is already saved").arg(img->rawFileName()));
      } else {
        saveNamedRawImageData(img->rawFileName(), img, QxrdDataProcessorBase::NoOverwrite);
      }
    }

    QxrdDoubleImageDataPtr dimg = takeNextFreeImage();

    dimg -> copyFrom(img);
    dimg -> set_DateTime(QDateTime::currentDateTime());

    processAcquiredImage(dimg, dark, mask, overflow);

    return dimg;
  } else {
    return QxrdDoubleImageDataPtr();
  }
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::processAcquiredDoubleImage
    (QxrdDoubleImageDataPtr dimg, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  return processAcquiredImage(dimg, dark, mask, overflow);
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::processAcquiredImage
    (QxrdDoubleImageDataPtr dimg, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  if (dimg) {
    QTime tic;
    tic.start();

    QCEP_DEBUG(DEBUG_PROCESS,
               emit printMessage(QDateTime::currentDateTime(),
                                 tr("Processing Image \"%1\", count %2").arg(dimg->get_FileName()).arg(getAcquiredCount()));
    );

    if (get_PerformDarkSubtraction()) {
      subtractDarkImage(dimg, dark);
      dimg -> set_ImageSaved(false);

      int subTime = tic.restart();

      updateEstimatedTime(prop_PerformDarkSubtractionTime(), subTime);

      QCEP_DEBUG(DEBUG_PROCESS,
                 emit printMessage(QDateTime::currentDateTime(),
                                   tr("Dark subtraction took %1 msec").arg(subTime));
      );
    }

    if (get_PerformBadPixels()) {
      correctBadPixels(dimg);
      dimg -> set_ImageSaved(false);

      updateEstimatedTime(prop_PerformBadPixelsTime(), tic.restart());
    }

    if (get_PerformGainCorrection()) {
      correctImageGains(dimg);
      dimg -> set_ImageSaved(false);

      updateEstimatedTime(prop_PerformGainCorrectionTime(), tic.restart());
    }

    if (get_SaveSubtracted()) {
      if (dimg->get_ImageSaved()) {
        emit printMessage(QDateTime::currentDateTime(),
                          tr("Image \"%1\" is already saved").arg(dimg->rawFileName()));
      } else {
        saveNamedImageData(QDir(subtractedOutputDirectory()).filePath(dimg->get_FileBase()), dimg);
      }
    }

    if (get_SaveAsText()) {
      saveNamedImageDataAsText(dimg->get_FileName(), dimg);

      updateEstimatedTime(prop_SaveAsTextTime(), tic.elapsed());
    }

    newData(dimg, overflow);

    QCEP_DEBUG(DEBUG_PROCESS,
               emit printMessage(QDateTime::currentDateTime(),
                                 tr("Processing took %1 msec").arg(tic.restart()));
    );
  }

  return dimg;
}

void QxrdDataProcessorBase::updateEstimatedTime(QcepDoubleProperty *prop, int msec)
{
  double newVal = prop -> value() * (1.0 - get_AveragingRatio()) + ((double) msec)/1000.0* get_AveragingRatio();

  prop -> setValue(newVal);
}

void QxrdDataProcessorBase::subtractDarkImage(QxrdDoubleImageDataPtr image, QxrdDoubleImageDataPtr dark)
{
  if (get_PerformDarkSubtraction()) {
    if (dark && image) {
      if (dark->get_ExposureTime() != image->get_ExposureTime()) {
        emit printMessage(QDateTime::currentDateTime(),
                          "Exposure times of acquired data and dark image are different, skipping");
        return;
      }

      if (dark->get_Width() != image->get_Width() ||
          dark->get_Height() != image->get_Height()) {
        emit printMessage(QDateTime::currentDateTime(),
                          "Dimensions of acquired data and dark image are different, skipping");
        return;
      }

      if (dark->get_CameraGain() != image->get_CameraGain()) {
        emit printMessage(QDateTime::currentDateTime(),
                          "Gains of acquired data and dark image are different, skipping");
        return;
      }

      if (!(image->get_DataType() == QxrdDoubleImageData::Raw16Data ||
            image->get_DataType() == QxrdDoubleImageData::Raw32Data)) {
        emit printMessage(QDateTime::currentDateTime(),
                          "Acquired data is not a raw image, skipping background subtraction");
        return;
      }

      QxrdMutexLocker lock1(__FILE__, __LINE__, dark->mutex());
      QxrdMutexLocker lock2(__FILE__, __LINE__, image->mutex());

      int height = image->get_Height();
      int width  = image->get_Width();
      int nres = image-> get_SummedExposures();
      int ndrk = dark -> get_SummedExposures();
      int npixels = width*height;

      if (nres <= 0) nres = 1;

      double ratio = ((double) nres)/((double) ndrk);

//      printf("Dark subtraction nres=%d, ndrk=%d, npixels=%d, ratio=%g\n",
//             nres, ndrk, npixels, ratio);

      double *result = image->data();
      double *dk     = dark->data();
      double avgraw  = 0;
//      double avgdark = 0;

      for (int i=0; i<npixels; i++) {
//        avgdark  += dk[i];
        avgraw   += result[i];
        result[i] = result[i]-ratio*dk[i];
      }

//      set_AverageDark(avgdark/npixels/ndrk);
      set_AverageRaw(avgraw/npixels/nres);
      set_Average(get_AverageRaw() - get_AverageDark());

      image -> set_DataType(QxrdDoubleImageData::SubtractedData);
    }
  }
}

void QxrdDataProcessorBase::correctBadPixels(QxrdDoubleImageDataPtr /*image*/)
{
}

void QxrdDataProcessorBase::correctImageGains(QxrdDoubleImageDataPtr /*image*/)
{
}

void QxrdDataProcessorBase::updateEstimatedProcessingTime()
{
  double estSerialTime = 0, estParallelTime = 0;

  if (get_SaveRawImages()) {
    if (m_Acquisition -> get_SummedExposures() > 1) {
      estSerialTime += m_Acquisition -> get_Raw32SaveTime();
    } else {
      estSerialTime += m_Acquisition -> get_Raw16SaveTime();
    }
  }

  if (get_PerformDarkSubtraction()) {
    estParallelTime += get_PerformDarkSubtractionTime();
  }

  if (get_PerformBadPixels()) {
    estParallelTime += get_PerformBadPixelsTime();
  }

  if (get_PerformGainCorrection()) {
    estParallelTime += get_PerformGainCorrectionTime();
  }

  if (get_SaveSubtracted()) {
    estSerialTime += get_SaveSubtractedTime();
  }

  if (get_SaveAsText()) {
    estSerialTime += get_SaveAsTextTime();
  }

  if (get_PerformIntegration()) {
    estParallelTime += get_PerformIntegrationTime();
  }

  if (get_DisplayIntegratedData()) {
    estSerialTime += get_DisplayIntegratedDataTime();
  }

  if (get_SaveIntegratedData()) {
    estSerialTime += get_SaveIntegratedDataTime();
  }

  set_EstimatedProcessingTime(estimatedProcessingTime(estSerialTime, estParallelTime));
}

double QxrdDataProcessorBase::estimatedProcessingTime(double estSerialTime, double estParallelTime)
{
  return estSerialTime + estParallelTime;
}

void QxrdDataProcessorBase::showMaskRange(/*double min, double max*/)
{
  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  if (m_Data && m_Mask) {
    m_Mask -> showMaskRange(QSharedPointer< QcepImageData<double> >(m_Data), min, max);

    newMask();
  }
}

void QxrdDataProcessorBase::hideMaskAll()
{
  if (m_Mask) {
    m_Mask -> hideMaskAll();

    newMask();
  }
}

void QxrdDataProcessorBase::showMaskAll()
{
  if (m_Mask) {
    m_Mask -> showMaskAll();

    newMask();
  }
}

void QxrdDataProcessorBase::hideMaskRange(/*double min, double max*/)
{
  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  if (m_Data && m_Mask) {
    m_Mask -> hideMaskRange(QSharedPointer< QcepImageData<double> >(m_Data), min, max);

    newMask();
  }
}

void QxrdDataProcessorBase::invertMask()
{
  if (m_Mask) {
    m_Mask -> invertMask();

    newMask();
  }
}

void QxrdDataProcessorBase::maskCircle(QwtDoubleRect rect)
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

    newMask();
  }
}

void QxrdDataProcessorBase::maskPolygon(QVector<QwtDoublePoint> poly)
{
  if (m_Mask) {
    //  printf("QxrdDataProcessorBase::maskPolygon(%d points ...)\n", poly.size());

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

    newMask();
  }
}

void QxrdDataProcessorBase::measurePolygon(QVector<QwtDoublePoint> poly)
{
  foreach(QwtDoublePoint pt, poly) {
    emit printMessage(QDateTime::currentDateTime(),
                      tr("Measure pt (%1,%2) = %3").arg(pt.x()).arg(pt.y())
                      .arg(m_Data -> value(pt.x(),pt.y())));
  }

  summarizeMeasuredPolygon(poly);
}

void QxrdDataProcessorBase::printMeasuredPolygon(QVector<QwtDoublePoint> poly)
{
  foreach(QwtDoublePoint pt, poly) {
    emit printMessage(QDateTime::currentDateTime(),
                      tr("Measure pt (%1,%2)").arg(pt.x()).arg(pt.y()));
  }

  summarizeMeasuredPolygon(poly);
}

void QxrdDataProcessorBase::summarizeMeasuredPolygon(QVector<QwtDoublePoint> poly)
{
  if (poly.size() >= 3) {
    double x0 = poly[0].x();
    double y0 = poly[0].y();
    double x1 = poly[1].x();
    double y1 = poly[1].y();
    double x2 = poly[2].x();
    double y2 = poly[2].y();
    double dx1 = x0-x1, dy1 = y0-y1, dx2 = x2-x1, dy2 = y2-y1;
    double a1 = atan2(dy1,dx1), a2 = atan2(dy2,dx2);

    emit statusMessage(QDateTime::currentDateTime(),
                       tr("Angle: @ %1,%2, ang %3 deg").arg(x1).arg(y1).arg((a2-a1)/M_PI*180.0));
  } else if (poly.size() == 2) {
    double x0 = poly[0].x();
    double y0 = poly[0].y();
    double x1 = poly[1].x();
    double y1 = poly[1].y();
    double dx = x1-x0;
    double dy = y1-y0;
    double ang = atan2(dy,dx);
    double len = sqrt(dx*dx+dy*dy);

    emit statusMessage(QDateTime::currentDateTime(),
                       tr("Line: %1,%2 - %3,%4 : D %5,%6 : L %7 : Ang %8").
                       arg(x0).arg(y0).arg(x1).arg(y1).
                       arg(dx).arg(dy).arg(len).arg(ang/M_PI*180.0));

  } else if (poly.size() == 1) {
    emit statusMessage(QDateTime::currentDateTime(),
                       tr("Point: %1,%2").arg(poly[0].x()).arg(poly[0].y()));
  }
}

void QxrdDataProcessorBase::slicePolygon(QVector<QwtDoublePoint> poly)
{
  m_Integrator -> slicePolygon(m_Data, poly, 0);
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::data() const
{
  return m_Data;
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::darkImage() const
{
  return m_DarkFrame;
}

QxrdMaskDataPtr QxrdDataProcessorBase::mask() const
{
  return m_Mask;
}

QxrdMaskDataPtr QxrdDataProcessorBase::overflow() const
{
  return m_Overflow;
}

int QxrdDataProcessorBase::incrementAcquiredCount()
{
  return m_AcquiredCount.fetchAndAddOrdered(+1);
}

int QxrdDataProcessorBase::decrementAcquiredCount()
{
  int res = m_AcquiredCount.fetchAndAddOrdered(-1);

  if (res == 0) {
    m_ProcessWaiting.wakeAll();
  }

  return res;
}

int QxrdDataProcessorBase::getAcquiredCount()
{
  return m_AcquiredCount.fetchAndAddOrdered(0);
}

int QxrdDataProcessorBase::status(double time)
{
  QMutex mutex;
  QxrdMutexLocker lock(__FILE__, __LINE__, &mutex);

  if (getAcquiredCount() == 0) {
    return 1;
  }

  if (m_ProcessWaiting.wait(&mutex, (int)(time*1000))) {
    return getAcquiredCount()==0;
  } else {
    return 0;
  }
}

QxrdCenterFinderPtr QxrdDataProcessorBase::centerFinder() const
{
  if (m_CenterFinder == NULL) {
    emit printMessage(QDateTime::currentDateTime(),
                      "Problem QxrdDataProcessorBase::centerFinder == NULL");
  }

  return m_CenterFinder;
}

QxrdIntegratorPtr QxrdDataProcessorBase::integrator() const
{
  if (m_Integrator == NULL) {
    emit printMessage(QDateTime::currentDateTime(),
                      "Problem QxrdDataProcessorBase::integrator == NULL");
  }

  return m_Integrator;
}

QxrdRingSetFitParametersPtr QxrdDataProcessorBase::initialRingSetFitParameters() const
{
  if (m_InitialRingSetFitParameters == NULL) {
    emit printMessage(QDateTime::currentDateTime(),
                      "Problem QxrdDataProcessorBase::initialRingSetFitParameters == NULL");
  }

  return m_InitialRingSetFitParameters;
}

QxrdRingSetSampledDataPtr QxrdDataProcessorBase::initialRingSetData() const
{
  if (m_InitialRingSetData == NULL) {
    emit printMessage(QDateTime::currentDateTime(),
                      "Problem QxrdDataProcessorBase::initialRingSetData == NULL");
  }

  return m_InitialRingSetData;
}

QxrdRingSetFitParametersPtr QxrdDataProcessorBase::refinedRingSetFitParameters() const
{
  if (m_RefinedRingSetFitParameters == NULL) {
    emit printMessage(QDateTime::currentDateTime(),
                      "Problem QxrdDataProcessorBase::refinedRingSetFitParameters == NULL");
  }

  return m_RefinedRingSetFitParameters;
}

QxrdRingSetSampledDataPtr QxrdDataProcessorBase::refinedRingSetData() const
{
  if (m_RefinedRingSetData == NULL) {
    emit printMessage(QDateTime::currentDateTime(),
                      "Problem QxrdDataProcessorBase::refinedRingSetData == NULL");
  }

  return m_RefinedRingSetData;
}

void QxrdDataProcessorBase::newImage(int ncols, int nrows)
{
  m_Data -> resize(ncols, nrows);
  m_Data -> fill(0);

  newData(m_Data, QxrdMaskDataPtr());
}

void QxrdDataProcessorBase::exponentialTail(double cx, double cy, double width, int oversample)
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

  newData(m_Data, QxrdMaskDataPtr());
}

void QxrdDataProcessorBase::reciprocalTail(double cx, double cy, double strength, int oversample)
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

  newData(m_Data, QxrdMaskDataPtr());
}

void QxrdDataProcessorBase::powderRing(double cx, double cy, double radius, double width, double strength, int oversample)
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

  newData(m_Data, QxrdMaskDataPtr());
}

void QxrdDataProcessorBase::ellipse(double cx, double cy, double a, double e, double ang, double width, double strength, int oversample)
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
          double th = atan2(yy,xx);
          double elr = a*(1-e*e)/(1 - e*cos(th - ang));
          double ndr = (elr - r)/width;

          if (fabs(ndr) < 6) {
            double val = strength*exp(-2*ndr*ndr)/width*sqrt(2.0/M_PI);
            sum += val;
          }
        }
      }

      m_Data -> setValue(x, y, m_Data -> value(x,y) + sum/(oversample*oversample));
    }
  }

  newData(m_Data, QxrdMaskDataPtr());
}

void QxrdDataProcessorBase::openLogFile()
{
  if (m_LogFile == NULL) {
    m_LogFile = fopen(qPrintable(get_LogFilePath()), "a");

    if (m_LogFile) {
      writeLogHeader();
    }
  }
}

void QxrdDataProcessorBase::newLogFile(QString path)
{
  if (m_LogFile) {
    fclose(m_LogFile);
    m_LogFile = NULL;
  }

  set_LogFilePath(path);

  openLogFile();
}

void QxrdDataProcessorBase::writeLogHeader()
{
  if (m_LogFile) {
    fprintf(m_LogFile, "#F %s\n", qPrintable(get_LogFilePath()));
    fprintf(m_LogFile, "#E %d\n", QDateTime::currentDateTime().toTime_t());
    fprintf(m_LogFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")));
    fflush(m_LogFile);
  }
}

void QxrdDataProcessorBase::logMessage(QString msg)
{
  openLogFile();

  if (m_LogFile) {
    fprintf(m_LogFile, "#CX %s\n", qPrintable(msg));
    fflush(m_LogFile);
  }
}

void QxrdDataProcessorBase::closeLogFile()
{
  if (m_LogFile) {
    logMessage(tr("%1 ------- shutdown --------").
               arg(QDateTime::currentDateTime().toString("yyyy.MM.dd : hh:mm:ss.zzz ")));
    fclose(m_LogFile);
    m_LogFile = NULL;
  }
}

void QxrdDataProcessorBase::writeOutputScan(QxrdIntegratedDataPtr data)
{
  if (this->get_SaveIntegratedData()) {
    fileSaverThread()->writeOutputScan(m_LogFile, data);
  }

  if (this->get_SaveIntegratedInSeparateFiles()) {
    fileSaverThread()->writeOutputScan(integratedOutputDirectory(), data);
  }
}

void QxrdDataProcessorBase::displayIntegratedData(QxrdIntegratedDataPtr data)
{
  emit newIntegrationAvailable(data);
}

void QxrdDataProcessorBase::fileWriteTest(int dim, QString path)
{
  long sz = dim*dim;
  quint32 *buff = new quint32[sz];

  QTime tic;
  tic.start();
  int totalt = 0;

  for (int i=0; i<10; i++) {
    QString fileName = path+tr("%1.junk").arg(i,5,10,QChar('0'));

    FILE *f = fopen(qPrintable(fileName), "w");

    if (f) {
      fwrite(buff, sz, sizeof(quint32), f);
      fclose(f);

      int dt = tic.restart();
      totalt += dt;
      emit printMessage(QDateTime::currentDateTime(),
                        tr("file %1 written in %2 msec").arg(fileName).arg(dt));
    }
  }

  emit printMessage(QDateTime::currentDateTime(),
                    tr("average write speed %1 MB/sec")
                    .arg(((double) sz)*40.0*1000.0/(1e6*((double) totalt))));
  delete [] buff;
}

void QxrdDataProcessorBase::calculateROI()
{
  QTime tic;
  tic.start();

  if (m_Data && m_Mask) {
    double *res = new double[65536];
    long npixels = (m_Data -> get_Height()) * (m_Data -> get_Width());
    double *data = m_Data -> data();
    short  *mask = m_Mask -> data();

    for (int i=0; i<65536; i++) {
      res[i] = 0;
    }

    for (long i=0; i<npixels; i++) {
      double v = data[i];
      short bin = mask[i];
      bin &= 255;
      res[bin] += v;
    }

    int dt = tic.restart();

    emit printMessage(QDateTime::currentDateTime(),
                      tr("ROI calculated in %1 msec").arg(dt));

    delete [] res;
  }
}

void QxrdDataProcessorBase::calculateHistogram()
{
  QTime tic;
  tic.start();

  if (m_Data && m_Mask) {
    double *res = new double[65536];
    long npixels = m_Data -> get_Height() * m_Data -> get_Width();
    double *data = m_Data -> data();
    short  *mask = m_Mask -> data();

    double minVal = *data;
    double maxVal = minVal;

    for (long i=0; i<npixels; i++) {
      double v = data[i];
      if (v < minVal) minVal = v;
      if (v > maxVal) maxVal = v;
    }

    for (int i=0; i<65536; i++) {
      res[i] = 0;
    }

    for (long i=0; i<npixels; i++) {
      double v = data[i];
      short bin = mask[i];
      res[bin] += v;
    }

    int dt = tic.restart();

    emit printMessage(QDateTime::currentDateTime(),
                      tr("Histogram calculated in %1 msec").arg(dt));

    delete [] res;
  }
}

QxrdGenerateTestImagePtr QxrdDataProcessorBase::generateTestImage() const
{
  return m_GenerateTestImage;
}

void QxrdDataProcessorBase::processorOptionsDialog()
{
  QxrdDataProcessorOptionsDialog options(this);

  options.exec();
}
