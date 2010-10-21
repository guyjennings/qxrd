/******************************************************************
*
*  $Id: qxrddataprocessorbase.cpp,v 1.4 2010/10/21 19:44:03 jennings Exp $
*
*******************************************************************/

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

#include <QTime>
#include <QPainter>
#include <QDirIterator>
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
    m_DarkUsage(QReadWriteLock::Recursive),
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
    m_LogFile(NULL),
    SOURCE_IDENT("$Id: qxrddataprocessorbase.cpp,v 1.4 2010/10/21 19:44:03 jennings Exp $")
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
    emit printMessage(tr("Waiting for double image"));

    QxrdDataProcessorThread::msleep(500);

    res = m_Allocator -> newDoubleImage();
  }

  return res;
}

void QxrdDataProcessorBase::newData(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{
  m_Data = image;

  m_Data -> setMask(m_Mask, overflow);

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

void QxrdDataProcessorBase::processData(QString name)
{
  QxrdDoubleImageDataPtr res = takeNextFreeImage();

  if (res -> readImage(name)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();

    processAcquiredImage(res, darkImage(), mask(), QxrdMaskDataPtr());

    set_DataPath(res -> get_FileName());
  }
}

void QxrdDataProcessorBase::processDataSequence(QString path, QString filter)
{
  QDirIterator iter(path, QStringList(filter));

  while (iter.hasNext()) {
    QString path = iter.next();

    emit printMessage(path);

    processData(path);
  }
}

void QxrdDataProcessorBase::processDataSequence(QStringList paths)
{
  QString path;

  foreach(path, paths) {
    emit printMessage(path);

    processData(path);
  }
}

void QxrdDataProcessorBase::processDataSequence(QString path, QStringList filters)
{
  QDirIterator iter(path, filters);

  while (iter.hasNext()) {
    QString path = iter.next();

    emit printMessage(path);

    processData(path);
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
    emit printMessage(tr("loadDark(%1) failed").arg(name));
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
             emit printMessage(tr("processing acquired 16 bit image, %1 remaining").arg(getAcquiredCount()));
  );

  if (img) {
    if (get_SaveRawImages()) {
      if (img->get_ImageSaved()) {
        emit printMessage(tr("Image \"%1\" is already saved").arg(img->rawFileName()));
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
             emit printMessage(tr("processing acquired 32 bit image, %1 remaining").arg(getAcquiredCount()));
  );

  if (img) {
    if (get_SaveRawImages()) {
      if (img->get_ImageSaved()) {
        emit printMessage(tr("Image \"%1\" is already saved").arg(img->rawFileName()));
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

QxrdDoubleImageDataPtr QxrdDataProcessorBase::processAcquiredImage
    (QxrdDoubleImageDataPtr dimg, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  if (dimg) {
    QTime tic;
    tic.start();

    QCEP_DEBUG(DEBUG_PROCESS,
               emit printMessage(tr("Processing Image \"%1\", count %2").arg(dimg->get_FileName()).arg(getAcquiredCount()));
    );

    if (get_PerformDarkSubtraction()) {
      QReadLocker rl(&m_DarkUsage);

      subtractDarkImage(dimg, dark);
      dimg -> set_ImageSaved(false);

      int subTime = tic.restart();

      updateEstimatedTime(prop_PerformDarkSubtractionTime(), subTime);

      QCEP_DEBUG(DEBUG_PROCESS,
                 emit printMessage(tr("Dark subtraction took %1 msec").arg(subTime));
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
        emit printMessage(tr("Image \"%1\" is already saved").arg(dimg->rawFileName()));
      } else {
        saveNamedImageData(dimg->get_FileName(), dimg);
      }
    }

    if (get_SaveAsText()) {
      saveNamedImageDataAsText(dimg->get_FileName(), dimg);

      updateEstimatedTime(prop_SaveAsTextTime(), tic.elapsed());
    }

    newData(dimg, overflow);

    QCEP_DEBUG(DEBUG_PROCESS,
               emit printMessage(tr("Processing took %1 msec").arg(tic.restart()));
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
        emit printMessage("Exposure times of acquired data and dark image are different, skipping");
        return;
      }

      if (dark->get_Width() != image->get_Width() ||
          dark->get_Height() != image->get_Height()) {
        emit printMessage("Dimensions of acquired data and dark image are different, skipping");
        return;
      }

      if (dark->get_CameraGain() != image->get_CameraGain()) {
        emit printMessage("Gains of acquired data and dark image are different, skipping");
        return;
      }

      if (!(image->get_DataType() == QxrdDoubleImageData::Raw16Data ||
            image->get_DataType() == QxrdDoubleImageData::Raw32Data)) {
        emit printMessage("Acquired data is not a raw image, skipping background subtraction");
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
    emit printMessage(tr("Measure pt (%1,%2) = %3").arg(pt.x()).arg(pt.y())
                      .arg(m_Data -> value(pt.x(),pt.y())));
  }

  summarizeMeasuredPolygon(poly);
}

void QxrdDataProcessorBase::printMeasuredPolygon(QVector<QwtDoublePoint> poly)
{
  foreach(QwtDoublePoint pt, poly) {
    emit printMessage(tr("Measure pt (%1,%2)").arg(pt.x()).arg(pt.y()));
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

    emit statusMessage(tr("Angle: @ %1,%2, ang %3 deg").arg(x1).arg(y1).arg((a2-a1)/M_PI*180.0));
  } else if (poly.size() == 2) {
    double x0 = poly[0].x();
    double y0 = poly[0].y();
    double x1 = poly[1].x();
    double y1 = poly[1].y();
    double dx = x1-x0;
    double dy = y1-y0;
    double ang = atan2(dy,dx);
    double len = sqrt(dx*dx+dy*dy);

    emit statusMessage(tr("Line: %1,%2 - %3,%4 : D %5,%6 : L %7 : Ang %8").
                       arg(x0).arg(y0).arg(x1).arg(y1).
                       arg(dx).arg(dy).arg(len).arg(ang/M_PI*180.0));

  } else if (poly.size() == 1) {
    emit statusMessage(tr("Point: %1,%2").arg(poly[0].x()).arg(poly[0].y()));
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
    emit printMessage("Problem QxrdDataProcessorBase::centerFinder == NULL");
  }

  return m_CenterFinder;
}

QxrdIntegratorPtr QxrdDataProcessorBase::integrator() const
{
  if (m_Integrator == NULL) {
    emit printMessage("Problem QxrdDataProcessorBase::integrator == NULL");
  }

  return m_Integrator;
}

QxrdRingSetFitParametersPtr QxrdDataProcessorBase::initialRingSetFitParameters() const
{
  if (m_InitialRingSetFitParameters == NULL) {
    emit printMessage("Problem QxrdDataProcessorBase::initialRingSetFitParameters == NULL");
  }

  return m_InitialRingSetFitParameters;
}

QxrdRingSetSampledDataPtr QxrdDataProcessorBase::initialRingSetData() const
{
  if (m_InitialRingSetData == NULL) {
    emit printMessage("Problem QxrdDataProcessorBase::initialRingSetData == NULL");
  }

  return m_InitialRingSetData;
}

QxrdRingSetFitParametersPtr QxrdDataProcessorBase::refinedRingSetFitParameters() const
{
  if (m_RefinedRingSetFitParameters == NULL) {
    emit printMessage("Problem QxrdDataProcessorBase::refinedRingSetFitParameters == NULL");
  }

  return m_RefinedRingSetFitParameters;
}

QxrdRingSetSampledDataPtr QxrdDataProcessorBase::refinedRingSetData() const
{
  if (m_RefinedRingSetData == NULL) {
    emit printMessage("Problem QxrdDataProcessorBase::refinedRingSetData == NULL");
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
  fileSaverThread()->writeOutputScan(m_LogFile, data);
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
      emit printMessage(tr("file %1 written in %2 msec").arg(fileName).arg(dt));
    }
  }

  emit printMessage(tr("average write speed %1 MB/sec").arg(((double) sz)*40.0*1000.0/(1e6*((double) totalt))));
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

    emit printMessage(tr("ROI calculated in %1 msec").arg(dt));

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

    emit printMessage(tr("Histogram calculated in %1 msec").arg(dt));

    delete [] res;
  }
}

QxrdGenerateTestImagePtr QxrdDataProcessorBase::generateTestImage() const
{
  return m_GenerateTestImage;
}

/******************************************************************
*
*  $Log: qxrddataprocessorbase.cpp,v $
*  Revision 1.4  2010/10/21 19:44:03  jennings
*  Adding code to display overflow pixels, removed cuda and simple processors
*
*  Revision 1.3  2010/09/24 22:29:37  jennings
*  Work on NSIS installer
*  Fixed startup problem on debug builds when calling QxrdAcquisitionThread->initialize()
*
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.7  2010/08/16 21:45:27  jennings
*  Distinguish between initial and refined fit parameters and data in powder ring fitting
*  Add read/write settings routines for powder ring fitting data
*
*  Revision 1.1.2.6  2010/08/03 19:24:19  jennings
*  Added data types to hold sampled powder ring coordinates
*
*  Revision 1.1.2.5  2010/08/02 21:10:31  jennings
*  Added "rings" object to data processor
*
*  Revision 1.1.2.4  2010/07/28 20:50:15  jennings
*  Implementing routines to access image data from scripts
*  Implement min max and average routines for data
*  Changed 'data', 'dark' and 'mask' script functions to
*  dynamically return correct objects
*
*  Revision 1.1.2.3  2010/07/28 19:29:45  jennings
*  Modified script output for void function results - no output in window
*  Added 'ellipse' command to processor
*  Implemented QxrdGenerateTestImage.generateImage
*
*  Revision 1.1.2.2  2010/07/27 21:53:03  jennings
*  Added double list property type
*  Added 'testImage' script object, interface to QxrdGenerateTestImage
*  Added QxrdDetectorGeometry class
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.20  2010/07/21 19:39:08  jennings
*  Improved calculation of estimated processing time
*
*  Revision 1.1.2.19  2010/07/20 20:30:25  jennings
*  Added memory usage display to status bar
*  Improved calculation of processing timings
*
*  Revision 1.1.2.18  2010/07/09 21:33:50  jennings
*  Tried to improve memory management by predicting memory requirements better
*  The allocators are more accurate in limiting allocated memory
*
*  Revision 1.1.2.17  2010/06/23 21:45:53  jennings
*  Eliminated some test code, and lots of unneeded locks
*
*  Revision 1.1.2.16  2010/06/22 23:37:33  jennings
*  Made most acquisition and processing output conditional
*
*  Revision 1.1.2.15  2010/06/17 18:38:03  jennings
*  Implemented multi-threaded processing, with serialization of saves/display operation
*
*  Revision 1.1.2.14  2010/06/17 16:02:35  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*  Revision 1.1.2.13  2010/06/15 20:36:25  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.12  2010/06/15 20:18:53  jennings
*  First steps to adding CUDA support
*
*  Revision 1.1.2.11  2010/06/14 20:18:00  jennings
*  Implemented plotting and saving of integrated data - still need some kind of 'serializer' to maintain order of saving and plotting
*
*  Revision 1.1.2.10  2010/06/11 21:22:54  jennings
*  When calling integrator, pass data, dark and mask images explicitly.
*
*  Revision 1.1.2.9  2010/06/09 19:19:37  jennings
*  Removed references to the QxrdFileSaver - all public accesses now through QxrdFileSaverThread
*
*  Revision 1.1.2.8  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.1.2.7  2010/05/20 20:16:58  jennings
*  Added simple threaded processor implementation using QtConcurrent::run
*
*  Revision 1.1.2.6  2010/05/19 21:15:35  jennings
*  PE acquisition debugging
*
*  Revision 1.1.2.5  2010/05/19 20:35:27  jennings
*  Fixed prefs saving problem
*
*  Revision 1.1.2.4  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.1.2.3  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.1.2.2  2010/04/27 19:38:28  jennings
*  Eliminate some deadlocks during image processing
*
*  Revision 1.1.2.1  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.71.2.9  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.71.2.8  2010/04/25 03:38:39  jennings
*  Added scrolling area to acquire dialog.  Instrument image queues.
*
*  Revision 1.71.2.7  2010/04/23 21:26:32  jennings
*  Rearranged message output
*
*  Revision 1.71.2.6  2010/04/22 21:42:07  jennings
*  Simulated acquisition is beginning to work
*
*  Revision 1.71.2.5  2010/04/22 20:56:26  jennings
*  Simulated acquisition is beginning to work
*
*  Revision 1.71.2.4  2010/04/22 19:40:33  jennings
*  Further rearrangement of acquisition code
*
*  Revision 1.71.2.3  2010/04/21 21:45:03  jennings
*  Changed mask values to shorts instead of bools.  Added ROI and histogram calculation tests.
*
*  Revision 1.71.2.2  2010/04/19 21:48:27  jennings
*  More rearrangement
*
*  Revision 1.71.2.1  2010/04/15 19:32:34  jennings
*  Fixed typo in saveSubtractedTime property
*
*  Revision 1.71  2010/03/05 23:45:00  jennings
*  Fixed problems with text output separators
*
*  Revision 1.70  2010/03/05 23:06:27  jennings
*  Modified text saving
*
*  Revision 1.69  2010/03/05 22:32:03  jennings
*  Version 0.3.9 adds text file output and conversion
*
*  Revision 1.68  2009/12/11 17:49:04  jennings
*  Added 'ImageSaved' property to image data and used this to avoid double-saving raw data when
*  processing data off-line
*
*  Revision 1.67  2009/12/03 21:35:37  jennings
*  Corrected locking problem with dark image
*
*  Revision 1.66  2009/11/17 20:42:59  jennings
*  Added instrumented QxrdMutexLocker which tracks how long locks are held, and prints
*  info about any held for more than 100 msec
*
*  Revision 1.65  2009/11/02 20:16:22  jennings
*  Changes to make it work with VC compiler
*
*  Revision 1.64  2009/10/26 02:40:24  jennings
*  Fixed initial mask display state
*
*  Revision 1.63  2009/10/24 05:23:55  jennings
*  Fixed problem with integrated curves coming in wrong order
*
*  Revision 1.62  2009/10/16 21:54:17  jennings
*  Implemented various processDataSequence commands
*
*  Revision 1.61  2009/10/02 21:03:11  jennings
*  Call fflush at strategic places during log file output
*
*  Revision 1.60  2009/10/02 20:18:35  jennings
*  Added support for (optionally) saving and/or displaying integrated data
*
*  Revision 1.59  2009/10/02 20:11:02  jennings
*  Added support for (optionally) saving and/or displaying integrated data
*
*  Revision 1.58  2009/10/01 21:44:05  jennings
*  Delete QxrdDataProcessorBase object at program exit
*  Removed some commented out dead wood
*
*  Revision 1.57  2009/09/29 18:39:46  jennings
*  Removed references to 'QxrdDataProcessorBase::processedCount'
*  Fixed up the various 'status' scripting functions so that they work properly
*
*  Revision 1.56  2009/09/28 22:06:55  jennings
*  Added circular integration to processing steps
*
*  Revision 1.55  2009/09/26 04:55:46  jennings
*  Removed some commented-out sections
*  Removed QxrdDataProcessorBase::copyImage(src,dst), replaced with templatized image member fn
*
*  Revision 1.54  2009/09/25 22:42:48  jennings
*  Masking changes
*
*  Revision 1.53  2009/09/25 14:22:16  jennings
*  Simplified double-buffering for plotted data - there is now a separate copy of data and mask
*  in QxrdWindow
*
*  Revision 1.52  2009/09/22 18:19:00  jennings
*  Added slicing routines
*  Set title for traces in avg data graph
*
*  Revision 1.51  2009/09/21 19:40:46  jennings
*  Added version number to window title, added more measurement output
*
*  Revision 1.50  2009/09/21 16:27:58  jennings
*  Added user interface to log file path
*
*  Revision 1.49  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.48  2009/09/18 20:44:49  jennings
*  Add separate status functions for acquisition and processing, as well as an aggregated function
*  combining the status of the two.
*
*  Revision 1.47  2009/09/14 19:08:57  jennings
*  Added more checks for appropriate data type / exposure etc. before subtracting
*  backgrounds
*
*  Revision 1.46  2009/09/13 13:59:47  jennings
*  Added 'canOverwrite' argument to data saving routines and arrange
*  that saves via file dialogs can overwrite, programmatic saves use
*  unique file names
*
*  Revision 1.45  2009/09/12 14:44:20  jennings
*  Moved lock to base class, made into mutex
*
*  Revision 1.44  2009/09/12 13:44:37  jennings
*  Renamed convertImage to copyImage, added double->double version of copyImage,
*  added timer to copyImage
*
*  Revision 1.43  2009/09/12 13:36:19  jennings
*  Added more locks to QxrdDataProcessorBase
*
*  Revision 1.42  2009/09/10 21:33:30  jennings
*  Added TIFF error handling
*
*  Revision 1.41  2009/09/09 22:32:12  jennings
*  Started to add TIFF metadata support
*
*  Revision 1.40  2009/09/08 21:41:59  jennings
*  Update estimated processing time accumulators
*
*  Revision 1.39  2009/09/07 22:10:14  jennings
*  Allow NULL mask
*
*  Revision 1.38  2009/09/04 21:11:41  jennings
*  Support for file write timing tests
*
*  Revision 1.37  2009/09/04 20:04:31  jennings
*  Debugging pre-trigger acquisition
*
*  Revision 1.36  2009/09/04 15:15:42  jennings
*  Added log file routines
*  Removed newlines from any printMessage calls.
*
*  Revision 1.35  2009/09/03 21:16:24  jennings
*  Added properties and user interface elements for pre- and post- trigger counts
*  Added properties and user interface elements for fine-grained control of processing chain
*
*  Revision 1.34  2009/08/27 21:55:43  jennings
*  Added code to make sure file saving routines will not overwrite data
*
*  Revision 1.33  2009/08/27 21:02:17  jennings
*  Partial implementation of lazy plotting
*
*  Revision 1.32  2009/08/27 17:06:52  jennings
*  Added code to load/save dark and mask data
*
*  Revision 1.31  2009/08/26 20:57:13  jennings
*  Starting to implement separate Double Int16 and Int32 processing chains
*
*  Revision 1.30  2009/08/25 18:49:19  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.29  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.28  2009/08/09 15:40:32  jennings
*  Added measurer tool to all graphs
*
*  Revision 1.27  2009/08/08 20:14:55  jennings
*  Added powder ring calculation operations
*
*  Revision 1.26  2009/08/07 22:21:56  jennings
*  Added a number of sample data creation routines to QxrdDataProcessorBase
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

