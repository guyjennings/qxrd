#define _CRT_SECURE_NO_WARNINGS

#include "qxrddebug.h"
#include "qxrddataprocessor.h"
#include "qxrddataprocessorthread.h"
#include <QtConcurrentRun>
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qcepimagedata.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrator.h"
#include "qcepintegrateddata.h"
#include "qcepmutexlocker.h"
#include "qcepallocator.h"
#include "qxrdfilesaverthread.h"
#include "qxrdfilesaver.h"
#include "qxrdgeneratetestimage.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qcepdatasetmodel.h"
#include "qxrdintegratorparmsdialog.h"
#include "qxrdpolartransformdialog.h"
#include "qxrdpolarnormalizationdialog.h"
#include <QTime>
#include <QPainter>
#include <qmath.h>
#include <QDir>
#include <QDirIterator>

QxrdDataProcessor::QxrdDataProcessor(QString name) :
  inherited(name)
//  m_OutputDirectory(saver, this,"outputDirectory", ""),
//  m_FileName(this, "fileName","", "Current File Name"),
//  m_DataPath(this,"dataPath", "", "Data Path"),
//  m_DarkImagePath(this, "darkImagePath", "", "Dark Images Path"),
//  m_BadPixelsPath(this, "badPixelsPath", "", "Bad Pixels Path"),
//  m_GainMapPath(this, "gainMapPath", "", "Gain Map Path"),
//  m_MaskPath(this, "maskPath", "", "Mask Path"),
//  m_PerformDarkSubtraction(this, "performDarkSubtraction", true, "Perform Dark Subtraction?"),
//  m_SaveRawImages(this, "saveRawImages", true, "Save Raw Images?"),
//  m_SaveDarkImages(this, "saveDarkImages", true, "Save Dark Images?"),
//  m_PerformBadPixels(this, "performBadPixels", true, "Perform Bad Pixel Correction?"),
//  m_PerformGainCorrection(this, "performGainCorrection", true, "Perform Gain Correction?"),
//  m_SaveSubtracted(this, "saveSubtracted", true, "Save Dark Subtracted Data?"),
//  m_SaveAsText(this, "saveAsText", false, "Save as Text Files (warning - Large and Slow!)"),
//  m_SaveAsTextSeparator(this, "saveAsTextSeparator", " ", "Separator for Images Saved as Text"),
//  m_SaveAsTextPerLine(this,"saveAsTextPerLine",16, "Pixels per line in Images Saved as Text"),
//  m_SaveOverflowFiles(this,"saveOverflowFiles",0, "Save Overflow Pixel Files?"),
//  m_PerformIntegration(this, "performIntegration", true, "Perform Circular Integration?"),
//  m_DisplayIntegratedData(this, "displayIntegratedData", true, "Display Integrated Data?"),
//  m_SaveIntegratedData(this, "saveIntegratedData", true, "Save Integrated Data?"),
//  m_SaveIntegratedPath(this, "saveIntegratedPath", "", "Integrated Data Path"),
//  m_SaveDarkInSubdirectory(this,"saveDarkInSubdirectory",0, "Save Dark In Subdirectory?"),
//  m_SaveDarkSubdirectory(this,"saveDarkSubdirectory","", "Dark Subdirectory"),
//  m_SaveRawInSubdirectory(this,"saveRawInSubdirectory",0, "Save Raw in Subdirectory?"),
//  m_SaveRawSubdirectory(this,"saveRawSubdirectory","", "Raw Subdirectory"),
//  m_SaveSubtractedInSubdirectory(this,"saveSubtractedInSubdirectory",0, "Save Subtracted in Subdirectory?"),
//  m_SaveSubtractedSubdirectory(this,"saveSubtractedSubdirectory","", "Subtracted Subdirectory"),
//  m_SaveIntegratedInSeparateFiles(this,"saveIntegratedInSeparateFiles",0, "Save Integrated in Separate Files?"),
//  m_SaveIntegratedInSubdirectory(this,"saveIntegratedInSubdirectory",0, "Save Integrated in Subdirectory?"),
//  m_SaveIntegratedSubdirectory(this,"saveIntegratedSubdirectory","", "Integrated Subdirectory"),
//  m_AccumulateIntegrated2D(this, "accumulateIntegrated2D", 0, "Accumulate integrated data in 2-d dataset"),
//  m_AccumulateIntegratedName(this, "accumulateIntegratedName", "/accumulated/2d-data", "Dataset to accumulate to"),
//  m_AccumulateIntegratedDirectory(this, "accumulateIntegratedDirectory", "", "Accumulator save directory"),
//  m_AccumulateIntegratedFileName(this, "accumulateIntegratedFileName", "", "Accumulator save file"),
//  m_AccumulateIntegratedFormat(this, "accumulateIntegratedFormat", "", "Accumulator save format"),
//  m_PerformDarkSubtractionTime(this, "performDarkSubtractionTime", 0.01, "Avg Time to Perform Dark Subtraction (in sec)"),
//  m_PerformBadPixelsTime(this, "performBadPixelsTime", 0.01, "Avg Time to Perform Bad Pixel Correction (in sec)"),
//  m_PerformGainCorrectionTime(this, "performGainCorrectionTime", 0.01, "Avg Time to Perform Gain Correction (in sec)"),
//  m_SaveSubtractedTime(this, "saveSubtractedTime", 0.1, "Avg Time to Save Subtracted Data (in sec)"),
//  m_SaveAsTextTime(this, "saveAsTextTime", 0.1, "Avg Time to Save Images as Text (in sec)"),
//  m_PerformIntegrationTime(this, "performIntegrationTime", 0.05, "Avg Time to Perform Integration (in sec/core)"),
//  m_DisplayIntegratedDataTime(this, "displayIntegratedDataTime", 0.2, "Avg Time to Display Integrated Data (in sec)"),
//  m_SaveIntegratedDataTime(this, "saveIntegratedDataTime", 0.01, "Avg Time to Save Integrated Data (in sec)"),
//  m_EstimatedProcessingTime(this, "estimatedProcessingTime", 0.1, "Overall Estimated Processing Time (in sec/image)"),
//  m_AveragingRatio(this, "averagingRatio", 0.1, "Averaging Ratio for Estimated Timing"),
//  m_CompressImages(this, "compressImages", false, "Compress Images"),
//  m_Average(this,"average",0.0, "Average Value of Acquired Image (per exposure)"),
//  m_AverageDark(this,"averageDark",0.0, "Average Value of Dark Image"),
//  m_AverageRaw(this,"averageRaw",0.0, "Average Value of Raw Image"),
//  m_CorrectionQueueLength(this, "correctionQueueLength", 0, "Image correction backlog"),
//  m_IntegrationQueueLength(this, "integrationQueueLength", 0, "Image integration backlog"),
//  m_SaverQueueLength(this, "saverQueueLength", 0, "Data saving backlog"),
//  m_ZingerAlgorithm(this, "zingerAlgorithm", 0, "Zinger Detection Algorithm"),
//  m_ZingerMask(this, "zingerMask", 0, "Zinger Mask Source"),
//  m_ZingerDestination(this, "zingerDestination", -1, "Zinger Destination"),
//  m_ZingerSize1(this, "zingerSize1", 1, "Inner Zinger Search Box Size"),
//  m_ZingerSize2(this, "zingerSize2", 5, "Outer Zinger Search Box Size"),
//  m_ZingerThreshold(this, "zingerThreshold", 2.5, "Zinger Detection Threshold"),
//  m_Window(),
//  m_Mutex(QMutex::Recursive),
//  m_AcquiredInt16Images("acquiredInt16Images"),
//  m_AcquiredInt32Images("acquiredInt32Images"),
//  m_AcquiredCount(0),
  //    m_Mask(allocator -> newMask()),
//  m_CenterFinder(NULL),
//  m_Integrator(NULL),
//  m_PolarTransform(NULL),
//  m_PolarNormalization(NULL),
//  m_GenerateTestImage(NULL)
//  m_CorrectedImages(prop_CorrectionQueueLength(), sharedFromThis()),
//  m_IntegratedData(prop_IntegrationQueueLength(), sharedFromThis()),
//  m_ROIData(NULL, sharedFromThis()),
//  m_HistogramData(NULL, sharedFromThis())
{
//  m_SaverQueueLength.setDebug(1);
//  m_IntegrationQueueLength.setDebug(1);
//  m_CorrectionQueueLength.setDebug(1);
#ifndef QT_NO_DEBUG
  printf("Constructing processor\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDataProcessor::QxrdDataProcessor(%p)\n", this);
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdDataProcessor::QxrdDataProcessor");
  }

//  m_Integrator = QxrdIntegrator::newIntegrator();
//  m_PolarTransform = QxrdPolarTransform::newPolarTransform();
//  m_PolarNormalization = QxrdPolarNormalization::newPolarNormalization();
//  m_GenerateTestImage = QxrdGenerateTestImage::newGenerateTestImage();

//  m_Integrator->initialize(centerFinder());

//  connect(&m_CorrectedImages, &QxrdResultSerializerBase::resultAvailable, this, &QxrdDataProcessor::onCorrectedImageAvailable);
//  connect(&m_IntegratedData,  &QxrdResultSerializerBase::resultAvailable, this, &QxrdDataProcessor::onIntegratedDataAvailable);
////  connect(&m_ROIData,         &QxrdResultSerializerBase::resultAvailable, this, &QxrdDataProcessor::onROIDataAvailable);
//  connect(&m_HistogramData,   &QxrdResultSerializerBase::resultAvailable, this, &QxrdDataProcessor::onHistogramDataAvailable);
}

QxrdDataProcessor::~QxrdDataProcessor()
{
#ifndef QT_NO_DEBUG
  printf("Deleting processor\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDataProcessor::~QxrdDataProcessor(%p)\n", this);
  }
}

QxrdDataProcessorPtr QxrdDataProcessor::newDataProcessor()
{
  QxrdDataProcessorPtr proc(new QxrdDataProcessor("processor"));

  return proc;
}

//QxrdAcqCommonWPtr QxrdDataProcessor::acquisition() const
//{
//  QxrdAcqCommonWPtr res;

//  QxrdExperimentPtr expt(experiment());

//  if (expt) {
//    res = expt->acquisition();
//  }

//  return res;
//}

void QxrdDataProcessor::shutdown()
{
  thread()->exit();
}

void QxrdDataProcessor::setAcquisition(QxrdAcqCommonWPtr acq)
{
  connect(prop_SaveRawImages(), &QcepBoolProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformDarkSubtraction(), &QcepBoolProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformBadPixels(), &QcepBoolProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformGainCorrection(), &QcepBoolProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_SaveSubtracted(), &QcepBoolProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_SaveAsText(), &QcepBoolProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformIntegration(), &QcepBoolProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_DisplayIntegratedData(), &QcepBoolProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_SaveIntegratedData(), &QcepBoolProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformDarkSubtractionTime(), &QcepDoubleProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformBadPixelsTime(), &QcepDoubleProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformGainCorrectionTime(), &QcepDoubleProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_SaveSubtractedTime(), &QcepDoubleProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_SaveAsTextTime(), &QcepDoubleProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformIntegrationTime(), &QcepDoubleProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_DisplayIntegratedDataTime(), &QcepDoubleProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  connect(prop_SaveIntegratedDataTime(), &QcepDoubleProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);

  QxrdAcquisitionPtr acqp(qSharedPointerDynamicCast<QxrdAcquisition>(acq));

  if (acqp) {
    connect(acqp -> prop_SummedExposures(), &QcepIntProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
    connect(acqp -> prop_Raw16SaveTime(), &QcepDoubleProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
    connect(acqp -> prop_Raw32SaveTime(), &QcepDoubleProperty::valueChanged, this, &QxrdDataProcessor::updateEstimatedProcessingTime);
  }
}

////TODO: this should go...
//void QxrdDataProcessor::setWindow(QxrdWindowWPtr win)
//{
//  m_Window = win;
//  newData(m_Data);
//  newMask(mask());
//}

//QcepSettingsSaver *QxrdDataProcessor::saver()
//{
//  return m_Saver;
//}

void QxrdDataProcessor::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

//  if (m_CenterFinder) {
//    settings->beginGroup("centerfinder");
//    m_CenterFinder -> writeSettings(settings);
//    settings->endGroup();
//  }

//  if (m_Integrator) {
//    settings->beginGroup("integrator");
//    m_Integrator   -> writeSettings(settings);
//    settings->endGroup();
//  }

//  if (m_PolarTransform) {
//    settings->beginGroup("polarTransform");
//    m_PolarTransform -> writeSettings(settings);
//    settings->endGroup();
//  }

//  if (m_PolarNormalization) {
//    settings->beginGroup("polarNormalization");
//    m_PolarNormalization -> writeSettings(settings);
//    settings->endGroup();
//  }
}

void QxrdDataProcessor::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

//  if (m_CenterFinder) {
//    settings->beginGroup("centerfinder");
//    m_CenterFinder -> readSettings(settings);
//    settings->endGroup();
//  }

//  if (m_Integrator) {
//    settings->beginGroup("integrator");
//    m_Integrator   -> readSettings(settings);
//    settings->endGroup();
//  }

//  if (m_PolarTransform) {
//    settings->beginGroup("polarTransform");
//    m_PolarTransform -> readSettings(settings);
//    settings->endGroup();
//  }

//  if (m_PolarNormalization) {
//    settings->beginGroup("polarNormalization");
//    m_PolarNormalization -> readSettings(settings);
//    settings->endGroup();
//  }
}

//QString QxrdDataProcessor::existingOutputDirectory(QString dir, QString subdir) const
//{
//  return QDir(dir).filePath(subdir);
//}

//QString QxrdDataProcessor::filePathInExperimentDirectory(QString name) const
//{
//  return QDir(experimentDirectory()).filePath(name);
//}

//QString QxrdDataProcessor::experimentDirectory() const
//{
//  QxrdExperimentPtr exp(experiment());

//  if (exp) {
//    return exp->get_ExperimentDirectory();
//  } else {
//    return QString();
//  }
//}

//QString QxrdDataProcessor::filePathInDarkOutputDirectory(QString name) const
//{
//  return QDir(darkOutputDirectory()).filePath(name);
//}

//QString QxrdDataProcessor::darkOutputDirectory() const
//{
//  if (get_SaveDarkInSubdirectory()) {
//    return existingOutputDirectory(dataDirectory(), get_SaveDarkSubdirectory());
//  } else {
//    return dataDirectory();
//  }
//}

//QString QxrdDataProcessor::filePathInRawOutputDirectory(QString name) const
//{
//  return QDir(rawOutputDirectory()).filePath(name);
//}

//QString QxrdDataProcessor::rawOutputDirectory() const
//{
//  if (get_SaveRawInSubdirectory()) {
//    return existingOutputDirectory(dataDirectory(), get_SaveRawSubdirectory());
//  } else {
//    return dataDirectory();
//  }
//}

//QString QxrdDataProcessor::filePathInSubtractedOutputDirectory(QString name) const
//{
//  return QDir(subtractedOutputDirectory()).filePath(name);
//}

//QString QxrdDataProcessor::subtractedOutputDirectory() const
//{
//  if (get_SaveSubtractedInSubdirectory()) {
//    return existingOutputDirectory(dataDirectory(), get_SaveSubtractedSubdirectory());
//  } else {
//    return dataDirectory();
//  }
//}

//QString QxrdDataProcessor::filePathInIntegratedOutputDirectory(QString name) const
//{
//  return QDir(integratedOutputDirectory()).filePath(name);
//}

//QString QxrdDataProcessor::integratedOutputDirectory() const
//{
//  if (get_SaveIntegratedInSubdirectory()) {
//    return existingOutputDirectory(dataDirectory(), get_SaveIntegratedSubdirectory());
//  } else {
//    return dataDirectory();
//  }
//}

//QString QxrdDataProcessor::pwd() const
//{
//  return dataDirectory();
//}

////void QxrdDataProcessor::cd(QString path)
////{
////  QDir dir(currentDirectory());

////  if (dir.cd(path)) {
////    set_OutputDirectory(dir.path());
////  }
////}

//QStringList QxrdDataProcessor::ls() const
//{
//  QStringList res;
//  QDir dir(dataDirectory());

//  res = dir.entryList(QStringList());

//  return res;
//}

//QStringList QxrdDataProcessor::ls(QString pattern) const
//{
//  QStringList res;
//  QDir dir(dataDirectory());

//  res = dir.entryList(QStringList(pattern));

//  return res;
//}

//void QxrdDataProcessor::saveData(QcepDataObjectPtr object, QString name, int canOverwrite)
//{
//  if (QThread::currentThread() != thread()) {
//    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveData",
//                                           Qt::BlockingQueuedConnection,
//                                           Q_ARG(QcepDataObjectPtr, object),
//                                           Q_ARG(QString, name), Q_ARG(int, canOverwrite)))
//  } else {
//    QString path = filePathInDataDirectory(name);

//    QcepDoubleImageDataPtr image = qSharedPointerDynamicCast<QcepDoubleImageData>(object);

//    if (image) {
//      saveNamedImageData(path, image, QcepMaskDataPtr(), canOverwrite);
//    } else {
//      printMessage(tr("Don't know how to save %1").arg(object->pathName()));
//    }
//  }
//}

//void QxrdDataProcessor::saveData(QcepDoubleImageDataPtr data, QString name, int canOverwrite)
//{
//  if (QThread::currentThread() != thread()) {
//    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveData",
//                                           Qt::BlockingQueuedConnection,
//                                           Q_ARG(QcepDoubleImageDataPtr, data),
//                                           Q_ARG(QString, name), Q_ARG(int, canOverwrite)))
//  } else {
//    QString path = filePathInDataDirectory(name);

//    saveNamedImageData(path, data, QcepMaskDataPtr(), canOverwrite);
//  }
//}

//void QxrdDataProcessor::saveCachedGeometry(QString name)
//{
//  QString path = filePathInDataDirectory(name);

//  QcepUInt32ImageDataPtr data = m_Integrator->cachedGeometry();

//  if (data) {
//    saveNamedImageData(path, data, QcepMaskDataPtr(), true);
//  }
//}

//void QxrdDataProcessor::saveCachedIntensity(QString name)
//{
//  QString path = filePathInDataDirectory(name);

//  QcepDoubleImageDataPtr data = m_Integrator->cachedIntensity();

//  if (data) {
//    saveNamedImageData(path, data, QcepMaskDataPtr(), true);
//  }
//}

//QxrdMaskStackPtr QxrdDataProcessor::maskStack()
//{
//  return m_Mask;
//}

//int QxrdDataProcessor::maskStackSize()
//{
//  return m_Masks.count();
//}

//void QxrdDataProcessor::clearBadPixels()
//{
//  newBadPixelsImage(QcepDoubleImageDataPtr());

//  set_BadPixelsPath("");
//}

//void QxrdDataProcessor::clearGainMap()
//{
//  newGainMapImage(QcepDoubleImageDataPtr());

//  set_GainMapPath("");
//}

QcepDoubleImageDataPtr QxrdDataProcessor::processAcquiredInt16Image(
    QcepDoubleImageDataPtr corrected,
    QcepUInt16ImageDataPtr img,
    QcepDoubleImageDataPtr dark,
    QcepMaskDataPtr mask,
    QcepMaskDataPtr overflow)
{
  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("processing acquired 16 bit image, %1 remaining")
                 .arg(getAcquiredCount()));
  }

  if (img) {
    if (get_SaveRawImages()) {
      if (img->get_ObjectSaved()) {
        printMessage(tr("Image \"%1\" is already saved").arg(img->rawFileName()));
      } else {
        saveNamedRawImageData(img->rawFileName(), img, overflow, QxrdDataProcessor::NoOverwrite);
      }
    }

    corrected -> copyFrom(img);
    corrected -> set_DateTime(QDateTime::currentDateTime());

    processAcquiredImage(corrected, corrected, dark, mask, overflow);

    return corrected;
  } else {
    return QcepDoubleImageDataPtr();
  }
}

QcepDoubleImageDataPtr QxrdDataProcessor::processAcquiredInt32Image(
    QcepDoubleImageDataPtr corrected,
    QcepUInt32ImageDataPtr img,
    QcepDoubleImageDataPtr dark,
    QcepMaskDataPtr mask,
    QcepMaskDataPtr overflow)
{
  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("processing acquired 32 bit image, %1 remaining")
                 .arg(getAcquiredCount()));
  }

  if (img) {
    if (get_SaveRawImages()) {
      if (img->get_ObjectSaved()) {
        printMessage(tr("Image \"%1\" is already saved").arg(img->rawFileName()));
      } else {
        saveNamedRawImageData(img->rawFileName(), img, overflow, QxrdDataProcessor::NoOverwrite);
      }
    }

    corrected -> copyFrom(img);
    corrected -> set_DateTime(QDateTime::currentDateTime());

    processAcquiredImage(corrected, corrected, dark, mask, overflow);

    return corrected;
  } else {
    return QcepDoubleImageDataPtr();
  }
}

//QcepDoubleImageDataPtr QxrdDataProcessor::processAcquiredDoubleImage(
//    QcepDoubleImageDataPtr processed,
//    QcepDoubleImageDataPtr dimg,
//    QcepDoubleImageDataPtr dark,
//    QcepMaskDataPtr mask,
//    QcepMaskDataPtr overflow)
//{
//  return processAcquiredImage(processed, dimg, dark, mask, overflow);
//}

//QcepDoubleImageDataPtr QxrdDataProcessor::processAcquiredDoubleImage(
//    QcepDoubleImageDataPtr processed,
//    QcepDoubleImageDataPtr dimg,
//    QcepDoubleImageDataPtr dark,
//    QcepMaskDataPtr mask,
//    QcepMaskDataPtr overflow,
//    QcepDoubleList v)
//{
//  return processAcquiredImage(processed, dimg, dark, mask, overflow, v);
//}

//QcepDoubleImageDataPtr QxrdDataProcessor::processAcquiredImage(
//    QcepDoubleImageDataPtr processed,
//    QcepDoubleImageDataPtr img,
//    QcepDoubleImageDataPtr dark,
//    QcepMaskDataPtr /*mask*/,
//    QcepMaskDataPtr overflow,
//    QcepDoubleList v)
//{
//  if (processed && img) {
//    processed->copyFrom(img);

//    statusMessage(tr("Processing Image \"%1\"").arg(processed->get_FileName()));

//    QTime tic;
//    tic.start();

//    if (v.length() > 0) {
//      processed->set_Normalization(v);
//    }

//    if (qcepDebug(DEBUG_PROCESS)) {
//      printMessage(tr("Processing Image \"%1\", image number %2, count %3")
//                   .arg(processed->get_FileName()).arg(processed->get_ImageNumber()).arg(getAcquiredCount()));
//    }

//    if (get_PerformDarkSubtraction()) {
//      subtractDarkImage(processed, dark);
//      processed -> set_ObjectSaved(false);

//      int subTime = tic.restart();

//      updateEstimatedTime(prop_PerformDarkSubtractionTime(), subTime);

//      if (qcepDebug(DEBUG_PROCESS)) {
//        printMessage(tr("Dark subtraction took %1 msec").arg(subTime));
//      }
//    }

//    if (get_PerformBadPixels()) {
//      correctBadPixels(processed);
//      processed -> set_ObjectSaved(false);

//      int badPxlTime = tic.restart();

//      updateEstimatedTime(prop_PerformBadPixelsTime(), badPxlTime);

//      if (qcepDebug(DEBUG_PROCESS)) {
//        printMessage(tr("Bad Pixel correction took %1 msec").arg(badPxlTime));
//      }
//    }

//    if (get_PerformGainCorrection()) {
//      correctImageGains(processed);
//      processed -> set_ObjectSaved(false);

//      int gainTime = tic.restart();

//      updateEstimatedTime(prop_PerformGainCorrectionTime(), gainTime);

//      if (qcepDebug(DEBUG_PROCESS)) {
//        printMessage(tr("Gain correction took %1 msec").arg(gainTime));
//      }
//    }

//    if (get_SaveSubtracted()) {
//      if (processed->get_ObjectSaved()) {
//        printMessage(tr("Image \"%1\" is already saved").arg(processed->rawFileName()));
//      } else {
//        saveNamedImageData(QDir(subtractedOutputDirectory()).filePath(processed->get_FileBase()), processed, overflow);
//      }
//    }

//    if (get_SaveAsText()) {
//      saveNamedImageDataAsText(processed->get_FileName(), processed, overflow);

//      updateEstimatedTime(prop_SaveAsTextTime(), tic.elapsed());
//    }

//    newData(processed);
//    newOverflow(overflow);

//    if (qcepDebug(DEBUG_PROCESS)) {
//      printMessage(tr("Processing took %1 msec").arg(tic.restart()));
//    }

//    statusMessage(tr("Completed Processing Image \"%1\"").arg(processed->get_FileName()));
//  }

//  return processed;
//}

//void QxrdDataProcessor::updateEstimatedTime(QcepDoubleProperty *prop, int msec)
//{
//  double newVal = prop -> value() * (1.0 - get_AveragingRatio()) + ((double) msec)/1000.0* get_AveragingRatio();

//  prop -> setValue(newVal);
//}

//void QxrdDataProcessor::correctBadPixels(QcepDoubleImageDataPtr /*image*/)
//{
//}

//void QxrdDataProcessor::correctImageGains(QcepDoubleImageDataPtr image)
//{
//  if (image) {
//    QcepDoubleImageDataPtr gains = gainMap();

//    if (gains) {
//      image -> multiply(gains);
//    }
//  }
//}

void QxrdDataProcessor::updateEstimatedProcessingTime()
{
  double estSerialTime = 0, estParallelTime = 0;

  QxrdAcquisitionPtr acq(
        qSharedPointerDynamicCast<QxrdAcquisition>(acquisition()));

  if (acq && get_SaveRawImages()) {
    if (acq -> get_SummedExposures() > 1) {
      estSerialTime += acq -> get_Raw32SaveTime();
    } else {
      estSerialTime += acq -> get_Raw16SaveTime();
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

//double QxrdDataProcessor::estimatedProcessingTime(double estSerialTime, double estParallelTime)
//{
//  return estSerialTime + estParallelTime;
//}

double QxrdDataProcessor::estimatedProcessingTime(double estSerialTime, double estParallelTime)
{
  int nThreads = QThreadPool::globalInstance()->maxThreadCount();

  if (nThreads >= 2) {
    return qMax(estSerialTime, estParallelTime/((double)nThreads));
  } else {
    return estSerialTime + estParallelTime;
  }
}

//void QxrdDataProcessor::measurePolygon(QVector<QPointF> poly)
//{
//  foreach(QPointF pt, poly) {
//    printMessage(tr("Measure pt (%1,%2) = %3").arg(pt.x()).arg(pt.y())
//                      .arg(m_Data -> getImageData(pt.x(),pt.y())));
//  }

//  summarizeMeasuredPolygon(poly);
//}

//void QxrdDataProcessor::printMeasuredPolygon(QVector<QPointF> poly)
//{
//  foreach(QPointF pt, poly) {
//    printMessage(tr("Measure pt (%1,%2)").arg(pt.x()).arg(pt.y()));
//  }

//  summarizeMeasuredPolygon(poly);
//}

//void QxrdDataProcessor::summarizeMeasuredPolygon(QVector<QPointF> poly)
//{
//  if (poly.size() >= 3) {
//    double x0 = poly[0].x();
//    double y0 = poly[0].y();
//    double x1 = poly[1].x();
//    double y1 = poly[1].y();
//    double x2 = poly[2].x();
//    double y2 = poly[2].y();
//    double dx1 = x0-x1, dy1 = y0-y1, dx2 = x2-x1, dy2 = y2-y1;
//    double a1 = atan2(dy1,dx1), a2 = atan2(dy2,dx2);

//    statusMessage(tr("Angle: @ %1,%2, ang %3 deg").arg(x1).arg(y1).arg((a2-a1)/M_PI*180.0));
//  } else if (poly.size() == 2) {
//    double x0 = poly[0].x();
//    double y0 = poly[0].y();
//    double x1 = poly[1].x();
//    double y1 = poly[1].y();
//    double dx = x1-x0;
//    double dy = y1-y0;
//    double ang = atan2(dy,dx);
//    double len = sqrt(dx*dx+dy*dy);

//    statusMessage(tr("Line: %1,%2 - %3,%4 : D %5,%6 : L %7 : Ang %8").
//                       arg(x0).arg(y0).arg(x1).arg(y1).
//                       arg(dx).arg(dy).arg(len).arg(ang/M_PI*180.0));

//  } else if (poly.size() == 1) {
//    statusMessage(tr("Point: %1,%2").arg(poly[0].x()).arg(poly[0].y()));
//  }
//}

//QcepDoubleImageDataPtr QxrdDataProcessor::data() const
//{
////  if (qcepDebug(DEBUG_INTEGRATOR)) {
////    printMessage(tr("processor.data() == %1").arg((long) m_Data.data()));
////  }

//  return m_Data;
//}

//QcepDoubleImageDataPtr QxrdDataProcessor::liveData() const
//{
//  return m_LiveData;
//}

QcepMaskDataPtr QxrdDataProcessor::overflow() const
{
  return m_Overflow;
}

//int QxrdDataProcessor::status(double time)
//{
//  QMutex mutex;
//  QcepMutexLocker lock(__FILE__, __LINE__, &mutex);

//  if (getAcquiredCount() == 0) {
//    return 1;
//  }

//  if (m_ProcessWaiting.wait(&mutex, (int)(time*1000))) {
//    return getAcquiredCount()==0;
//  } else {
//    return 0;
//  }
//}

//QxrdExperimentPtr QxrdDataProcessor::experiment() const
//{
//  if (m_Experiment == NULL) {
//    printMessage("Problem: QxrdDataProcessor::experiment == NULL");
//  }

//  return m_Experiment;
//}

//QxrdCenterFinderPtr QxrdDataProcessor::centerFinder() const
//{
//  if (m_CenterFinder == NULL) {
//    printMessage("Problem QxrdDataProcessor::centerFinder == NULL");
//  }

//  return m_CenterFinder;
//}

//QxrdIntegratorPtr QxrdDataProcessor::integrator() const
//{
//  if (m_Integrator == NULL) {
//    printMessage("Problem QxrdDataProcessor::integrator == NULL");
//  }

//  return m_Integrator;
//}

//QxrdPolarTransformPtr QxrdDataProcessor::polarTransform() const
//{
//  if (m_PolarTransform == NULL) {
//    printMessage("Problem QxrdDataProcessor::polarTransform == NULL");
//  }

//  return m_PolarTransform;
//}

//QxrdPolarNormalizationPtr QxrdDataProcessor::polarNormalization() const
//{
//  if (m_PolarNormalization == NULL) {
//    printMessage("Problem QxrdDataProcessor::polarNormalization == NULL");
//  }

//  return m_PolarNormalization;
//}

void QxrdDataProcessor::newImage(int ncols, int nrows)
{
  m_Data -> resize(ncols, nrows);
  m_Data -> fill(0);

  newData(m_Data);
}

void QxrdDataProcessor::exponentialTail(double cx, double cy, double width, int oversample)
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

      m_Data -> setImageData(x, y,
                             m_Data -> getImageData(x,y) + sum/(oversample*oversample));
    }
  }

  newData(m_Data);
}

void QxrdDataProcessor::reciprocalTail(double cx, double cy, double strength, int oversample)
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

      m_Data -> setImageData(x, y,
                             m_Data -> getImageData(x,y) + sum/(oversample*oversample));
    }
  }

  newData(m_Data);
}

void QxrdDataProcessor::powderRing(double cx, double cy, double radius, double width, double strength, int oversample)
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

      m_Data -> setImageData(x, y,
                             m_Data -> getImageData(x,y) + sum/(oversample*oversample));
    }
  }

  newData(m_Data);
}

void QxrdDataProcessor::ellipse(double cx, double cy, double a, double e, double ang, double width, double strength, int oversample)
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

      m_Data -> setImageData(x, y,
                             m_Data -> getImageData(x,y) + sum/(oversample*oversample));
    }
  }

  newData(m_Data);
}

//void QxrdDataProcessor::writeOutputScan(QcepIntegratedDataPtr data)
//{
//  QxrdFileSaverPtr f(fileSaver());

//  if (f) {
//    if (this->get_SaveIntegratedData()) {
//      QxrdExperimentPtr expt(experiment());

//      if (expt) {
//        expt->openScanFile();
//        f->writeOutputScan(expt->scanFile(), data);
//      }
//    }

//    if (this->get_SaveIntegratedInSeparateFiles()) {
//      f->writeOutputScan(integratedOutputDirectory(), data);
//    }
//  }
//}

//QxrdGenerateTestImageWPtr QxrdDataProcessor::generateTestImage() const
//{
//  return m_GenerateTestImage;
//}

void QxrdDataProcessor::newOutputScan(QString title)
{
  m_OutputScan = QcepAllocator::newIntegratedData(title, 0, QcepAllocator::NullIfNotAvailable);
}

void QxrdDataProcessor::appendToOutputScan(double x, double y)
{
  if (m_OutputScan) {
    m_OutputScan->append(x,y);
  }
}

void QxrdDataProcessor::plotOutputScan()
{
  if (m_OutputScan) {
    emit newIntegrationAvailable(m_OutputScan);
  }
}

void QxrdDataProcessor::saveOutputScan(QString /*fileName*/)
{
  if (m_OutputScan) {

  }
}

//QStringList QxrdDataProcessor::integrateRectangle(int x0, int y0, int x1, int y1)
//{
//  double sum = 0;
//  double npx = 0;

//  QcepImageDataBasePtr   dat = m_Data;
//  QcepMaskDataPtr        msk = mask();
//  QStringList            res;

//  if (dat) {
//    if (msk) {
//      for (int y=y0; y<y1; y++) {
//        for (int x=x0; x<x1; x++) {
//          if (msk->value(x,y)) {
//            sum += dat->getImageData(x,y);
//            npx += 1;
//          }
//        }
//      }
//    } else {
//      for (int y=y0; y<y1; y++) {
//        for (int x=x0; x<x1; x++) {
//          sum += dat->getImageData(x,y);
//          npx += 1;
//        }
//      }
//    }

//    printMessage(tr("integrateRectange(\"%1\",%2,%3,%4,%5)=[%6,%7]=%8")
//                 .arg(dat->get_FileName())
//                 .arg(x0).arg(y0).arg(x1).arg(y1).arg(sum).arg(npx).arg(sum/npx));

//    if (npx > 0) {
//      res << tr("%1").arg(sum/npx);
//    } else {
//      res << "0";
//    }

//    res << tr("%1").arg(sum);
//    res << tr("%1").arg(npx);
//    res << dat->get_FileName();
//  }

//  return res;
//}

//QcepDataObjectPtr QxrdDataProcessor::integrate(QcepDoubleImageDataPtr img)
//{
//  QcepDataObjectPtr res;
//  QxrdIntegratorPtr integ = integrator();

//  if (integ) {
//    res = integ->performIntegration(img, mask());
//  }

//  return res;
//}

//QcepDataObjectPtr QxrdDataProcessor::polarTransform(QcepDoubleImageDataPtr img)
//{
//  QcepDataObjectPtr res;
//  QxrdPolarTransformPtr xform = polarTransform();

//  if (xform) {
//    res = xform->transform(img, mask());
//  }

//  return res;
//}

//QcepDataObjectPtr QxrdDataProcessor::polarIntegrate(QcepDoubleImageDataPtr img)
//{
//  QcepDataObjectPtr res;
//  QxrdPolarNormalizationPtr norm = polarNormalization();

//  if (norm) {
//    res = norm->transform(img);
//  }

//  return res;
//}

bool QxrdDataProcessor::integrateParameters()
{
  GUI_THREAD_CHECK;

  bool res = false;
  QxrdExperimentPtr expt(experiment());

  if (expt) {
    QxrdDataProcessorPtr proc(expt->dataProcessor());

    if (proc) {
      QxrdIntegratorParmsDialog dlg(proc);

      if (dlg.exec() == QDialog::Accepted) {
        res = true;
      }
    }
  }

  return res;
}

bool QxrdDataProcessor::polarTransformParameters()
{
  GUI_THREAD_CHECK;

  bool res = false;

  QxrdExperimentPtr expt(experiment());

  if (expt) {
    QxrdDataProcessorPtr proc(expt->dataProcessor());

    if (proc) {
      QxrdPolarTransformDialog dlg(proc);

      if (dlg.exec() == QDialog::Accepted) {
        res = true;
      }
    }
  }

  return res;
}

bool QxrdDataProcessor::polarIntegrateParameters()
{
  GUI_THREAD_CHECK;

  bool res = false;

  QxrdExperimentPtr expt(experiment());

  if (expt) {
    QxrdDataProcessorPtr proc(expt->dataProcessor());

    if (proc) {
      QxrdPolarNormalizationDialog dlg(proc);

      if (dlg.exec() == QDialog::Accepted) {
        res = true;
      }
    }
  }

  return res;
}

void QxrdDataProcessor::idleInt16Image(QcepUInt16ImageDataPtr image, bool liveView)
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

    QcepDoubleImageDataPtr corrected =
        QcepAllocator::newDoubleImage("idle", image->get_Width(), image->get_Height(), QcepAllocator::AlwaysAllocate);
    QcepDoubleImageDataPtr d      = dark();

    corrected->copyFrom(image);
    subtractDarkImage(corrected, d);

    newData(corrected);

    m_LiveData = corrected;
  }
}

//void QxrdDataProcessor::acquiredInt16Image(QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow)
//{
//  QcepDoubleImageDataPtr corrected =
//      QcepAllocator::newDoubleImage("acquired", image->get_Width(), image->get_Height(), QcepAllocator::AlwaysAllocate);

//  m_CorrectedImages.enqueue(QtConcurrent::run(this,
//                                              &QxrdDataProcessor::correctInt16Image,
//                                              corrected, image, dark(), mask(), overflow));
//}

//void QxrdDataProcessor::processInt32Image(QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow)
//{
//  QcepDoubleImageDataPtr corrected =
//      QcepAllocator::newDoubleImage("acquired", image->get_Width(), image->get_Height(), QcepAllocator::AlwaysAllocate);

//  m_CorrectedImages.enqueue(QtConcurrent::run(this,
//                                              &QxrdDataProcessor::correctInt32Image,
//                                              corrected, image, dark(), mask(), overflow));
//}

//void QxrdDataProcessor::processDoubleImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow)
//{
//  QcepDoubleImageDataPtr corrected =
//      QcepAllocator::newDoubleImage("acquired", image->get_Width(), image->get_Height(), QcepAllocator::AlwaysAllocate);

//  typedef QcepDoubleImageDataPtr (QxrdDataProcessor::*MFType)(QcepDoubleImageDataPtr, QcepDoubleImageDataPtr, QcepDoubleImageDataPtr, QcepMaskDataPtr, QcepMaskDataPtr);
//  MFType p = &QxrdDataProcessor::correctDoubleImage;

//  m_CorrectedImages.enqueue(QtConcurrent::run(this,
//                                              p,
//                                              corrected, image, dark(), mask(), overflow));
//}

//void QxrdDataProcessor::processDoubleImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, QList<double> v)
//{
//  QcepDoubleImageDataPtr corrected =
//      QcepAllocator::newDoubleImage("acquired", image->get_Width(), image->get_Height(), QcepAllocator::AlwaysAllocate);

//  typedef QcepDoubleImageDataPtr (QxrdDataProcessor::*MFType)(QcepDoubleImageDataPtr, QcepDoubleImageDataPtr, QcepDoubleImageDataPtr, QcepMaskDataPtr, QList<double>);
//  MFType p = &QxrdDataProcessor::correctDoubleImage;

//  m_CorrectedImages.enqueue(QtConcurrent::run(this,
//                                              p,
//                                              corrected, image, dark(), overflow, v));
//}

//QcepDoubleImageDataPtr QxrdDataProcessor::correctInt16Image
//    (QcepDoubleImageDataPtr corrected, QcepUInt16ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow)
//{
//  QThread::currentThread()->setObjectName("correctInt16Image");

//  if (qcepDebug(DEBUG_PROCESS)) {
//    printMessage(tr("QxrdDataProcessorThreaded::correctInt16Image"));
//  }

//  if (image) {
//    if ((image -> get_ImageNumber()) >= 0) {
//      return processAcquiredInt16Image(corrected, image, dark, mask, overflow);
//    } else {
//      if (get_SaveDarkImages()) {
//        saveNamedImageData(image->get_FileName(), image, overflow);
//        set_DarkImagePath(image->get_FileName());
//      }

//      newDark(image);
//    }
//  }

//  return QcepDoubleImageDataPtr();
//}

//QcepDoubleImageDataPtr QxrdDataProcessor::correctInt32Image
//    (QcepDoubleImageDataPtr corrected, QcepUInt32ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow)
//{
//  QThread::currentThread()->setObjectName("correctInt32Image");

//  if (qcepDebug(DEBUG_PROCESS)) {
//    printMessage(tr("QxrdDataProcessorThreaded::correctInt32Image"));
//  }

//  if (image) {
//    if ((image -> get_ImageNumber()) >= 0) {
//      return processAcquiredInt32Image(corrected, image, dark, mask, overflow);
//    } else {
//      if (get_SaveDarkImages()) {
//        saveNamedImageData(image->get_FileName(), image, overflow);

//        set_DarkImagePath(image->get_FileName());
//      }

//      newDark(image);
//    }
//  }

//  return QcepDoubleImageDataPtr();
//}

//QcepDoubleImageDataPtr QxrdDataProcessor::correctDoubleImage
//    (QcepDoubleImageDataPtr corrected, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow)
//{
//  QThread::currentThread()->setObjectName("correctDoubleImage");

//  if (qcepDebug(DEBUG_PROCESS)) {
//    printMessage(tr("QxrdDataProcessorThreaded::correctDoubleImage"));
//  }

//  if (image) {
//    if ((image -> get_ImageNumber()) >= 0) {
//      return processAcquiredDoubleImage(corrected, image, dark, mask, overflow);
//    } else {
//      if (get_SaveDarkImages()) {
//        saveNamedImageData(image->get_FileName(), image, overflow);

//        set_DarkImagePath(image->get_FileName());
//      }

//      newDark(image);
//    }
//  }

//  return QcepDoubleImageDataPtr();
//}

//QcepDoubleImageDataPtr QxrdDataProcessor::correctDoubleImage
//    (QcepDoubleImageDataPtr corrected, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr overflow, QcepDoubleList v)
//{
//  QThread::currentThread()->setObjectName("correctDoubleImage");

//  if (qcepDebug(DEBUG_PROCESS)) {
//    printMessage(tr("QxrdDataProcessorThreaded::correctDoubleImage"));
//  }

//  if (image) {
//    if ((image -> get_ImageNumber()) >= 0) {
//      return processAcquiredDoubleImage(corrected, image, dark, image->mask(), overflow, v);
//    } else {
//      if (get_SaveDarkImages()) {
//        saveNamedImageData(image->get_FileName(), image, overflow);

//        set_DarkImagePath(image->get_FileName());
//      }

//      newDark(image);
//    }
//  }

//  return QcepDoubleImageDataPtr();
//}

//void QxrdDataProcessor::onCorrectedImageAvailable()
//{
//  QcepDoubleImageDataPtr img = m_CorrectedImages.dequeue();
//  QcepMaskDataPtr mask = (img ? img->mask() : QcepMaskDataPtr());
//  QxrdCenterFinderPtr cf(centerFinder());

//  if (img && cf) {
//    m_IntegratedData.enqueue(QtConcurrent::run(this, &QxrdDataProcessor::integrateImage,
//                                               img, mask,
//                                               cf -> get_Center().x(),
//                                               cf -> get_Center().y()));

////    m_ROIData.enqueue(QtConcurrent::run(this, &QxrdDataProcessor::calculateROI,
////                                        img, mask));

//    m_HistogramData.enqueue(QtConcurrent::run(this, &QxrdDataProcessor::calculateHistogram,
//                                              img, mask));
//  }
//}

//QcepIntegratedDataPtr QxrdDataProcessor::integrateImage
//    (QcepDoubleImageDataPtr image, QcepMaskDataPtr mask, double /*cx*/, double /*cy*/)
//{
//  QcepIntegratedDataPtr res;

//  QThread::currentThread()->setObjectName("integrateImage");

//  if (qcepDebug(DEBUG_PROCESS)) {
//    printMessage(tr("QxrdDataProcessorThreaded::integrateImage"));
//  }

//  if (image && get_PerformIntegration()) {
//    QTime tic;
//    tic.start();

//    res = m_Integrator -> performIntegration(image, mask);

//    updateEstimatedTime(prop_PerformIntegrationTime(), tic.restart());
//  }

//  return res;
//}

//void QxrdDataProcessor::onIntegratedDataAvailable()
//{
//  if (qcepDebug(DEBUG_PROCESS)) {
//    printMessage(tr("QxrdDataProcessorThreaded::onIntegratedDataAvailable"));
//  }

//  QcepIntegratedDataPtr integ = m_IntegratedData.dequeue();

//  if (integ) {
//    writeOutputScan(integ);
//    displayIntegratedData(integ);

//    if (get_AccumulateIntegrated2D()) {
//      integrator() -> appendIntegration(get_AccumulateIntegratedName(), integ);
//    }
//  }
//}

//QxrdHistogramDataPtr QxrdDataProcessor::calculateHistogram
//    (QcepDoubleImageDataPtr /*image*/, QcepMaskDataPtr /*mask*/)
//{
//  QThread::currentThread()->setObjectName("calculateHistogram");

//  if (qcepDebug(DEBUG_PROCESS)) {
//    printMessage(tr("QxrdDataProcessorThreaded::calculateHistogram"));
//  }

//  return QxrdHistogramDataPtr();
//}

//void QxrdDataProcessor::onHistogramDataAvailable()
//{
//  if (qcepDebug(DEBUG_PROCESS)) {
//    printMessage(tr("QxrdDataProcessorThreaded::onHistogramDataAvailable"));
//  }

//  QxrdHistogramDataPtr histData = m_HistogramData.dequeue();
//}

//double QxrdDataProcessorThreaded::estimatedProcessingTime(double estSerialTime, double estParallelTime)
//{
//  int nThreads = QThreadPool::globalInstance()->maxThreadCount();

//  if (nThreads >= 2) {
//    return qMax(estSerialTime, estParallelTime/((double)nThreads));
//  } else {
//    return estSerialTime + estParallelTime;
//  }
//}

void QxrdDataProcessor::sumImages(QStringList names)
{
  QcepDoubleImageDataPtr summed = QcepAllocator::newDoubleImage("sum", 0,0, QcepAllocator::NullIfNotAvailable);

  if (summed) {
    int first = true;

    foreach(QString name, names) {
      QcepDoubleImageDataPtr img =
          QcepAllocator::newDoubleImage("image", 0,0, QcepAllocator::NullIfNotAvailable);
      QString path = filePathInDataDirectory(name);

      if (img && img->readImage(path)) {
        printMessage(tr("Load image from %1").arg(path));
        statusMessage(tr("Load image from %1").arg(path));

        img -> loadMetaData();

        int typ = img->get_DataType();

        if ((typ == QcepDoubleImageData::Raw16Data) ||
            (typ == QcepDoubleImageData::Raw32Data))
        {
          subtractDarkImage(img, dark());
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
      newData(summed);
    }
  }
}

void QxrdDataProcessor::addImages(QStringList names)
{
  QcepDoubleImageDataPtr summed = qSharedPointerDynamicCast<QcepDoubleImageData>(data());

  if (summed) {
    foreach(QString name, names) {
      QcepDoubleImageDataPtr img =
          QcepAllocator::newDoubleImage("image", 0,0, QcepAllocator::NullIfNotAvailable);
      QString path = filePathInDataDirectory(name);

      if (img && img->readImage(path)) {
        printMessage(tr("Load image from %1").arg(path));
        statusMessage(tr("Load image from %1").arg(path));

        img -> loadMetaData();

        int typ = img->get_DataType();

        if ((typ == QcepDoubleImageData::Raw16Data) ||
            (typ == QcepDoubleImageData::Raw32Data))
        {
          subtractDarkImage(img, dark());
        }

        summed->add(img);
      } else {
        printMessage(tr("Couldn't load %1").arg(path));
        statusMessage(tr("Couldn't load %1").arg(path));
      }
    }

    newData(summed);
  } else {
    printMessage("Couldn't sum image data");
  }
}

void QxrdDataProcessor::subtractImages(QStringList names)
{
  QcepDoubleImageDataPtr summed = qSharedPointerDynamicCast<QcepDoubleImageData>(data());

  if (summed) {
    foreach(QString name, names) {
      QcepDoubleImageDataPtr img =
          QcepAllocator::newDoubleImage("image", 0,0, QcepAllocator::NullIfNotAvailable);
      QString path = filePathInDataDirectory(name);

      if (img && img->readImage(path)) {
        printMessage(tr("Load image from %1").arg(path));
        statusMessage(tr("Load image from %1").arg(path));

        img -> loadMetaData();

        int typ = img->get_DataType();

        if ((typ == QcepDoubleImageData::Raw16Data) ||
            (typ == QcepDoubleImageData::Raw32Data))
        {
          subtractDarkImage(img, dark());
        }

        summed->subtract(img);
      } else {
        printMessage(tr("Couldn't load %1").arg(path));
        statusMessage(tr("Couldn't load %1").arg(path));
      }
    }

    newData(summed);
  } else {
    printMessage("Couldn't subtract image data");
  }
}

//void QxrdDataProcessor::clearAccumulator()
//{
//  m_Integrator -> clearAccumulator(get_AccumulateIntegratedName());
//}

//void QxrdDataProcessor::integrateAndAccumulate(QStringList names)
//{
//  int nImages   = names.count();
//  m_Integrator -> prepareAccumulator(get_AccumulateIntegratedName(), nImages);

//  foreach(QString name, names) {
//    QcepDoubleImageDataPtr img =
//        QcepAllocator::newDoubleImage("image", 0,0, QcepAllocator::NullIfNotAvailable);
//    QString path = filePathInDataDirectory(name);

//    if (img && img->readImage(path)) {
//      printMessage(tr("Load image from %1").arg(path));
//      statusMessage(tr("Load image from %1").arg(path));

//      img -> loadMetaData();

//      m_Integrator -> appendIntegration(get_AccumulateIntegratedName(), img, mask());
//    } else {
//      printMessage(tr("Couldn't load %1").arg(path));
//      statusMessage(tr("Couldn't load %1").arg(path));
//    }
//  }

//  m_Integrator -> completeAccumulator(get_AccumulateIntegratedName());
//}

//void QxrdDataProcessor::saveAccumulator(QString &path, QString filter)
//{
//  m_Integrator -> saveAccumulator(get_AccumulateIntegratedName(), path, filter);
//}

//void QxrdDataProcessor::reflectHorizontally()
//{
//  QcepDoubleImageDataPtr image = qSharedPointerDynamicCast<QcepDoubleImageData>(data());

//  if (image) {
//    int wid = image->get_Width();
//    int hgt = image->get_Height();

//    for (int y=0; y<hgt; y++) {
//      for (int x=0; x<wid/2; x++) {
//        double val1 = image->getImageData(x,y);
//        double val2 = image->getImageData(wid-x-1, y);

//        image->setValue(wid-x-1, y, val1);
//        image->setValue(x,y, val2);
//      }
//    }
//  }

//  newData(image);
//}

//void QxrdDataProcessor::reflectVertically()
//{
//  QcepDoubleImageDataPtr image = qSharedPointerDynamicCast<QcepDoubleImageData>(data());

//  if (image) {
//    int wid = image->get_Width();
//    int hgt = image->get_Height();

//    for (int x=0; x<wid; x++) {
//      for (int y=0; y<hgt/2; y++) {
//        double val1 = image->getImageData(x,y);
//        double val2 = image->getImageData(x, hgt-y-1);

//        image->setValue(x, hgt-y-1, val1);
//        image->setValue(x,y, val2);
//      }
//    }
//  }

//  newData(image);
//}

//void QxrdDataProcessor::projectImages(QStringList names, int px, int py, int pz)
//{
//  QcepDoubleImageDataPtr sumx, sumy, sumz;

//  int nx = 0;
//  int ny = 0;
//  int nz = names.count();
//  int first = true;

//  if (px) {
//    sumx = QcepAllocator::newDoubleImage("sumx", 0,0, QcepAllocator::NullIfNotAvailable);
//    printMessage(tr("Projecting %1 images onto X").arg(nz));
//  }

//  if (py) {
//    sumy = QcepAllocator::newDoubleImage("sumy", 0,0, QcepAllocator::NullIfNotAvailable);
//    printMessage(tr("Projecting %1 images onto Y").arg(nz));
//  }

//  if (pz) {
//    sumz = QcepAllocator::newDoubleImage("sumz", 0,0, QcepAllocator::NullIfNotAvailable);
//    printMessage(tr("Projecting %1 images onto Z").arg(nz));
//  }

//  QxrdExperimentPtr expt(experiment());

//  if (expt) {
//    expt->commenceWork(nz);
//  }

//  for (int i=0; i<nz; i++) {
//    QcepDoubleImageDataPtr img = QcepAllocator::newDoubleImage(tr("image-%1").arg(i), 0,0, QcepAllocator::NullIfNotAvailable);
//    QString path = filePathInDataDirectory(names[i]);

//    if (img && img->readImage(path)) {
//      printMessage(tr("Load image from %1").arg(path));
//      statusMessage(tr("Load image from %1").arg(path));

//      img->loadMetaData();
//      int typ = img->get_DataType();

//      if ((typ == QcepDoubleImageData::Raw16Data) ||
//          (typ == QcepDoubleImageData::Raw32Data))
//      {
//        subtractDarkImage(img, dark());
//      }

//      if (first) {
//        nx = img->get_Width();
//        ny = img->get_Height();

//        if (px && sumx) {
//          sumx->copyPropertiesFrom(img);
//          sumx->resize(nz,ny);
//          sumx->clear();
//          sumx->set_SummedExposures(0);
//        }

//        if (py && sumy) {
//          sumy->copyPropertiesFrom(img);
//          sumy->resize(nz,nx);
//          sumy->clear();
//          sumy->set_SummedExposures(0);
//        }

//        if (pz && sumz) {
//          sumz->copyPropertiesFrom(img);
//          sumz->resize(nx,ny);
//          sumz->clear();
//          sumz->set_SummedExposures(0);
//        }

//        first = false;
//      }

//      if (px && sumx) {
//        sumx->prop_SummedExposures()->incValue(1);

//        for (int y=0; y<ny; y++) {
//          double sum=0;

//          for (int x=0; x<nx; x++) {
//            sum += img->getImageData(x,y);
//          }

//          sumx->addValue(i,y, sum);
//        }
//      }

//      if (py && sumy) {
//        sumy->prop_SummedExposures()->incValue(1);

//        for (int x=0; x<nx; x++) {
//          double sum=0;

//          for (int y=0; y<ny; y++) {
//            sum += img->getImageData(x,y);
//          }

//          sumy->addValue(i,x, sum);
//        }
//      }

//      if (pz && sumz) {
//        sumz->prop_SummedExposures()->incValue(1);

//        for (int x=0; x<nx; x++) {
//          for (int y=0; y<ny; y++) {
//            sumz->addValue(x,y, img->getImageData(x,y));
//          }
//        }
//      }
//    }

//    if (expt) {
//      expt->completeWork(1);
//    }
//  }

//  if (px && sumx) {
//    newData(sumx);
//  }

//  if (py && sumy) {
//    newData(sumy);
//  }

//  if (pz && sumz) {
//    newData(sumz);
//  }

//  if (expt) {
//    expt->finishedWork(nz);
//  }
//}

void QxrdDataProcessor::correlateImages(QStringList names)
{
  QcepDoubleImageDataPtr imga = qSharedPointerDynamicCast<QcepDoubleImageData>(data());

  if (imga) {
    foreach(QString name, names) {
      QcepDoubleImageDataPtr imgb = QcepAllocator::newDoubleImage("image", 0,0, QcepAllocator::NullIfNotAvailable);
      QString path = filePathInDataDirectory(name);

      if (imgb && imgb->readImage(path)) {
        printMessage(tr("Load image from %1").arg(path));
        statusMessage(tr("Load image from %1").arg(path));

        imgb -> loadMetaData();

        int typ = imgb->get_DataType();

        if ((typ == QcepDoubleImageData::Raw16Data) ||
            (typ == QcepDoubleImageData::Raw32Data))
        {
          subtractDarkImage(imgb, dark());
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
  } else {
    printMessage("Couldn't correlate images");
  }
}

void QxrdDataProcessor::shiftImage(int dx, int dy)
{
  QcepDoubleImageDataPtr img = qSharedPointerDynamicCast<QcepDoubleImageData>(data());

  if (img) {
    QcepDoubleImageDataPtr shft = QcepAllocator::newDoubleImage("image", img->get_Width(), img->get_Height(), QcepAllocator::NullIfNotAvailable);

    if (shft) {
      shft->shiftImage(img, dx, dy);

      newData(shft);
    }
  }
}

//void QxrdDataProcessor::integrateData(QString name)
//{
//  QThread::currentThread()->setObjectName("integrateData");

//  QcepDoubleImageDataPtr img =
//      QcepAllocator::newDoubleImage("image", 0,0, QcepAllocator::NullIfNotAvailable);

//  QString path = filePathInDataDirectory(name);

//  QxrdCenterFinderPtr cf(centerFinder());

//  if (cf && img && img -> readImage(path)) {
//    printMessage(tr("Load image from %1").arg(path));
//    statusMessage(tr("Load image from %1").arg(path));

//    img -> loadMetaData();

//    m_IntegratedData.enqueue(QtConcurrent::run(this, &QxrdDataProcessor::integrateImage,
//                                               img, mask(),
//                                               cf -> get_Center().x(),
//                                               cf -> get_Center().y()));
//  } else {
//    printMessage(tr("Couldn't load %1").arg(path));
//    statusMessage(tr("Couldn't load %1").arg(path));
//  }
//}

//void QxrdDataProcessor::processData(QString name)
//{
//  QcepDoubleImageDataPtr res = QcepAllocator::newDoubleImage("image", 0,0, QcepAllocator::NullIfNotAvailable);

//  QString path = filePathInDataDirectory(name);

//  if (res && res -> readImage(path)) {
//    printMessage(tr("Load image from %1").arg(path));
//    statusMessage(tr("Load image from %1").arg(path));

//    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

//    res -> loadMetaData();

//    processDoubleImage(res, /*darkImage(), mask(),*/ QcepMaskDataPtr());

//    set_DataPath(res -> get_FileName());
//  } else {
//    printMessage(tr("Couldn't load %1").arg(path));
//    statusMessage(tr("Couldn't load %1").arg(path));
//  }
//}

void QxrdDataProcessor::processDataSequence(QString path, QString filter)
{
  QDirIterator iter(path, QStringList(filter));

  while (iter.hasNext()) {
    QString path = iter.next();

    printMessage(path);

    processData(path);
  }
}

void QxrdDataProcessor::processDataSequence(QStringList paths)
{
  QString path;

  foreach(path, paths) {
    printMessage(path);

    processData(path);
  }
}

void QxrdDataProcessor::processDataSequence(QString path, QStringList filters)
{
  QDirIterator iter(path, filters);

  while (iter.hasNext()) {
    QString path = iter.next();

    printMessage(path);

    processData(path);
  }
}

void QxrdDataProcessor::processNormalizedFile(QString path, double v1)
{
  QList<double> v;
  v << v1;

  processNormalizedFile(path, v);
}

void QxrdDataProcessor::processNormalizedFile(QString path, double v1, double v2)
{
  QList<double> v;
  v << v1 << v2;

  processNormalizedFile(path, v);
}

void QxrdDataProcessor::processNormalizedFile(QString name, QList<double> v)
{
  QcepDoubleImageDataPtr res = QcepAllocator::newDoubleImage("image", 0,0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  if (res && res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> setMask(mask(), QcepMaskDataPtr());

    processDoubleImage(res, /*darkImage(), mask(),*/ QcepMaskDataPtr(), v);

    set_DataPath(res -> get_FileName());
  } else {
    printMessage(tr("Couldn't load %1").arg(path));
  }
}

void QxrdDataProcessor::setFileNormalization(QString path, double v1)
{
  QList<double> v;
  v << v1;

  setFileNormalization(path, v);
}

void QxrdDataProcessor::setFileNormalization(QString path, double v1, double v2)
{
  QList<double> v;
  v << v1 << v2;

  setFileNormalization(path, v);
}

void QxrdDataProcessor::setFileNormalization(QString name, QList<double> v)
{
  QcepDoubleImageDataPtr res = QcepAllocator::newDoubleImage("image", 0,0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  if (res && res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> setMask(mask(), QcepMaskDataPtr());
    res -> set_Normalization(v);
    res -> saveMetaData(name);
  } else {
    printMessage(tr("Couldn't load %1").arg(path));
  }
}

//void QxrdDataProcessor::slicePolygon(QVector<QPointF> poly)
//{
//  QcepDoubleImageDataPtr dimg = qSharedPointerDynamicCast<QcepDoubleImageData>(data());
//  QxrdIntegrator *integ = integrator().data();

//  if (dimg) {
//    m_IntegratedData.enqueue(
//          QtConcurrent::run(integ,
//                            &QxrdIntegrator::slicePolygon,
//                            dimg, poly, 0));
//  }
//}

//void QxrdDataProcessor::integrateSaveAndDisplay()
//{
//  QcepDoubleImageDataPtr dimg = qSharedPointerDynamicCast<QcepDoubleImageData>(data());
//  QxrdCenterFinderPtr cf(centerFinder());

//  if (dimg) {
//    if (qcepDebug(DEBUG_INTEGRATOR)) {
//      if (cf) {
//        printMessage(tr("processor.integrateSaveAndDisplay: %1, %2, %3")
//                     .arg(dimg->get_FileName())
//                     .arg(cf->get_Center().x())
//                     .arg(cf->get_Center().y()));
//      } else {
//        printMessage("QxrdDataProcessor::integrateSaveAndDisplay no center finder");
//      }
//    }

//    QxrdIntegrator *integ = integrator().data();

//    m_IntegratedData.enqueue(
//          QtConcurrent::run(integ,
//                            &QxrdIntegrator::performIntegration,
//                            dimg, mask()));
//  }
//}

void QxrdDataProcessor::fixupBadBackgroundSubtraction(QString imagePattern, int nImgExposures, QString darkPath, int nDarkExposures)
{
  QDirIterator imagePaths(dataDirectory(), QStringList(imagePattern));

  QcepDoubleImageDataPtr dark = QcepAllocator::newDoubleImage("dark", 0,0, QcepAllocator::NullIfNotAvailable);
  QString path = filePathInDataDirectory(darkPath);

  if (dark && dark->readImage(path)) {
    dark->loadMetaData();

    int nFileDarkExposures = dark->get_SummedExposures();

    printMessage(tr("Loaded Dark image from %1 (%2 summed)").arg(path).arg(nFileDarkExposures));

    while (imagePaths.hasNext()) {
      QString imagePath=imagePaths.next();

      QString path = filePathInDataDirectory(imagePath);
      QcepDoubleImageDataPtr image = QcepAllocator::newDoubleImage("image", 0,0, QcepAllocator::NullIfNotAvailable);

      if (image && image->readImage(path)) {
        image->loadMetaData();

        int nFileImageExposures = image->get_SummedExposures();

        printMessage(tr("Loaded image from %1 (%2 summed)").arg(path).arg(nFileImageExposures));

        image->correctBadBackgroundSubtraction(dark,nImgExposures,nDarkExposures);

        QxrdFileSaverPtr saver(fileSaver());

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

void QxrdDataProcessor::fitPeakNear(double x, double y)
{
  QxrdCenterFinderPtr cf(centerFinder());

  if (cf) {
    cf->fitPeakNear(x,y);
  }
}
