#define _CRT_SECURE_NO_WARNINGS

#include "qxrddebug.h"
#include "qxrdprocessor.h"
#include "qcepallocator.h"
#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdexperiment.h"
#include "qxrdprocessorstep.h"
#include "qxrdfilesaver-ptr.h"
#include "qxrdfilesaver.h"
#include "qxrdmaskstack.h"
#include "qxrdzingerfinder.h"
#include "qxrdcenterfinder.h"
#include "qxrdapplication.h"
#include "qxrdacquisition.h"
#include "qxrdintegrator.h"
#include "qcepmutexlocker.h"
#include "qxrdpolartransform.h"
#include "qxrdpolarnormalization.h"
#include "qxrdgeneratetestimage.h"
#include "qxrdintegratorparmsdialog.h"
#include "qxrdpolartransformdialog.h"
#include "qxrdpolarnormalizationdialog.h"
#include "qxrdroicalculator.h"
#include "qxrdfilesaver.h"
#include "qxrdpowderringsmodel.h"
#include "qxrdroivector.h"
#include "qxrdroimodel.h"
#include <QFileInfo>
#include <QThread>
#include <QDir>
#include <QPainter>
#include <QtConcurrentRun>
#include <QDirIterator>

//TODO: emit data available signals when appropriate...
QxrdProcessor::QxrdProcessor(QString name) :
  QcepObject(name),
  m_FileName(this, "fileName","", "Current File Name"),
  m_DataPath(this,"dataPath", "", "Data Path"),
  m_DarkImagePath(this, "darkImagePath", "", "Dark Images Path"),
  m_BadPixelsPath(this, "badPixelsPath", "", "Bad Pixels Path"),
  m_GainMapPath(this, "gainMapPath", "", "Gain Map Path"),
  m_MaskPath(this, "maskPath", "", "Mask Path"),
  m_ScriptPath(this, "scriptPath", "", "Script Path"),
  m_Average(this,"average",0.0, "Average Value of Acquired Image (per exposure)"),
  m_AverageDark(this,"averageDark",0.0, "Average Value of Dark Image"),
  m_AverageRaw(this,"averageRaw",0.0, "Average Value of Raw Image"),
  m_DetectorDisplayMode(this, "detectorDisplayMode", ImageDisplayMode, "Detector Display Mode"),
  m_MaskMinimumValue(this, "maskMinimumValue", 0, "Mask Minimum Value"),
  m_MaskMaximumValue(this, "maskMaximumValue", 20000, "Mask Maximum Value"),
  m_MaskCircleRadius(this, "maskCircleRadius", 10, "Mask Circle Radius"),
  m_MaskSetPixels(this, "maskSetPixels", true, "Mask Set Pixels"),
  m_PerformBadPixels(this, "performBadPixels", true, "Perform Bad Pixel Correction?"),
  m_PerformBadPixelsTime(this, "performBadPixelsTime", 0.01, "Avg Time to Perform Bad Pixel Correction (in sec)"),
  m_PerformGainCorrection(this, "performGainCorrection", true, "Perform Gain Correction?"),
  m_PerformGainCorrectionTime(this, "performGainCorrectionTime", 0.01, "Avg Time to Perform Gain Correction (in sec)"),
  m_PerformDarkSubtraction(this, "performDarkSubtraction", true, "Perform Dark Subtraction?"),
  m_PerformDarkSubtractionTime(this, "performDarkSubtractionTime", 0.01, "Avg Time to Perform Dark Subtraction (in sec)"),
  m_SaveSubtracted(this, "saveSubtracted", true, "Save Dark Subtracted Data?"),
  m_SaveSubtractedTime(this, "saveSubtractedTime", 0.1, "Avg Time to Save Subtracted Data (in sec)"),
  m_SaveSubtractedInSubdirectory(this,"saveSubtractedInSubdirectory",0, "Save Subtracted in Subdirectory?"),
  m_SaveSubtractedSubdirectory(this,"saveSubtractedSubdirectory","", "Subtracted Subdirectory"),
  m_SaveRawImages(this, "saveRawImages", true, "Save Raw Images?"),
  m_SaveRawInSubdirectory(this,"saveRawInSubdirectory",0, "Save Raw in Subdirectory?"),
  m_SaveRawSubdirectory(this,"saveRawSubdirectory","", "Raw Subdirectory"),
  m_SaveDarkImages(this, "saveDarkImages", true, "Save Dark Images?"),
  m_SaveDarkInSubdirectory(this,"saveDarkInSubdirectory",0, "Save Dark In Subdirectory?"),
  m_SaveDarkSubdirectory(this,"saveDarkSubdirectory","", "Dark Subdirectory"),
  m_SaveAsText(this, "saveAsText", false, "Save as Text Files (warning - Large and Slow!)"),
  m_SaveAsTextTime(this, "saveAsTextTime", 0.1, "Avg Time to Save Images as Text (in sec)"),
  m_SaveAsTextSeparator(this, "saveAsTextSeparator", " ", "Separator for Images Saved as Text"),
  m_SaveAsTextPerLine(this,"saveAsTextPerLine",16, "Pixels per line in Images Saved as Text"),
  m_SaveOverflowFiles(this,"saveOverflowFiles",0, "Save Overflow Pixel Files?"),
  m_CorrectionQueueLength(this, "correctionQueueLength", 0, "Image correction backlog"),
  m_PerformIntegration(this, "performIntegration", true, "Perform Circular Integration?"),
  m_PerformIntegrationTime(this, "performIntegrationTime", 0.05, "Avg Time to Perform Integration (in sec/core)"),
  m_IntegrationQueueLength(this, "integrationQueueLength", 0, "Image integration backlog"),
  m_DisplayIntegratedData(this, "displayIntegratedData", true, "Display Integrated Data?"),
  m_DisplayIntegratedDataTime(this, "displayIntegratedDataTime", 0.2, "Avg Time to Display Integrated Data (in sec)"),
  m_SaveIntegratedData(this, "saveIntegratedData", true, "Save Integrated Data?"),
  m_SaveIntegratedDataTime(this, "saveIntegratedDataTime", 0.01, "Avg Time to Save Integrated Data (in sec)"),
  m_SaveIntegratedPath(this, "saveIntegratedPath", "", "Integrated Data Path"),
  m_SaveIntegratedInSeparateFiles(this,"saveIntegratedInSeparateFiles",0, "Save Integrated in Separate Files?"),
  m_SaveIntegratedInSubdirectory(this,"saveIntegratedInSubdirectory",0, "Save Integrated in Subdirectory?"),
  m_SaveIntegratedSubdirectory(this,"saveIntegratedSubdirectory","", "Integrated Subdirectory"),
  m_AccumulateIntegrated2D(this, "accumulateIntegrated2D", 0, "Accumulate integrated data in 2-d dataset"),
  m_AccumulateIntegratedName(this, "accumulateIntegratedName", "/accumulated/2d-data", "Dataset to accumulate to"),
  m_AccumulateIntegratedDirectory(this, "accumulateIntegratedDirectory", "", "Accumulator save directory"),
  m_AccumulateIntegratedFileName(this, "accumulateIntegratedFileName", "", "Accumulator save file"),
  m_AccumulateIntegratedFormat(this, "accumulateIntegratedFormat", "", "Accumulator save format"),
  m_HistogramQueueLength(this, "histogramQueueLength", 0, "Image histogram backlog"),
  m_SaverQueueLength(this, "saverQueueLength", 0, "Data saving backlog"),
  m_EstimatedProcessingTime(this, "estimatedProcessingTime", 0.1, "Overall Estimated Processing Time (in sec/image)"),
  m_AveragingRatio(this, "averagingRatio", 0.1, "Averaging Ratio for Estimated Timing"),
  m_CalculateROICounts(this, "calculateROICounts", true, "Calculate ROI Counts"),
  m_DisplayROIBorders(this, "displayROIBorders", true, "Display ROIs in image"),
  m_RoiCounts(this, "roiCounts", QcepDoubleVector(), "ROI Counts"),
  m_Data(QcepAllocator::newDoubleImage("data", 2048, 2048, QcepAllocator::WaitTillAvailable)),
  m_Dark(NULL),
  m_BadPixels(NULL),
  m_GainMap(NULL),
  m_MaskStack(),
  m_ZingerFinder(),
  m_CenterFinder(),
  m_ROICalculator(),
  m_Mutex(QMutex::Recursive),
  m_AcquiredInt16Images("acquiredInt16Images"),
  m_AcquiredInt32Images("acquiredInt32Images"),
  m_AcquiredCount(0),
  m_CorrectedImages(prop_CorrectionQueueLength()),
  m_IntegratedData(prop_IntegrationQueueLength()),
  //  m_ROIData(NULL, this),
  m_HistogramData(prop_HistogramQueueLength())
{
  m_MaskStack = QxrdMaskStackPtr(
        new QxrdMaskStack("maskStack"));

  m_ZingerFinder = QxrdZingerFinderPtr(
        new QxrdZingerFinder("zingerFinder"));

  m_CenterFinder = QxrdCenterFinderPtr(
        new QxrdCenterFinder("centerFinder"));

  m_Integrator = QxrdIntegratorPtr(
        new QxrdIntegrator("integrator"));

  m_PolarTransform = QxrdPolarTransformPtr(
        new QxrdPolarTransform("polarTransform"));

  m_PolarNormalization = QxrdPolarNormalizationPtr(
        new QxrdPolarNormalization("polarNormalization"));

  m_GenerateTestImage = QxrdGenerateTestImagePtr(
        new QxrdGenerateTestImage("testImage"));

  m_PowderRings = QxrdPowderRingsModelPtr(
        new QxrdPowderRingsModel());

  m_ROIVector = QxrdROIVectorPtr(
        new QxrdROIVector("roiVector"));

  m_ROIModel = QxrdROIModelPtr(
        new QxrdROIModel(m_ROIVector));

  m_ROICalculator = QxrdROICalculatorPtr(
        new QxrdROICalculator("roiCalculator", m_ROIVector, m_ROIModel));

  connect(&m_CorrectedImages, &QxrdResultSerializerBase::resultAvailable, this, &QxrdProcessor::onCorrectedImageAvailable);
  connect(&m_IntegratedData,  &QxrdResultSerializerBase::resultAvailable, this, &QxrdProcessor::onIntegratedDataAvailable);
//  connect(&m_ROIData,         &QxrdResultSerializerBase::resultAvailable, this, &QxrdProcessor::onROIDataAvailable);
  connect(&m_HistogramData,   &QxrdResultSerializerBase::resultAvailable, this, &QxrdProcessor::onHistogramDataAvailable);

  connect(prop_SaveRawImages(), &QcepBoolProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformDarkSubtraction(), &QcepBoolProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformBadPixels(), &QcepBoolProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformGainCorrection(), &QcepBoolProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_SaveSubtracted(), &QcepBoolProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_SaveAsText(), &QcepBoolProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformIntegration(), &QcepBoolProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_DisplayIntegratedData(), &QcepBoolProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_SaveIntegratedData(), &QcepBoolProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformDarkSubtractionTime(), &QcepDoubleProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformBadPixelsTime(), &QcepDoubleProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformGainCorrectionTime(), &QcepDoubleProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_SaveSubtractedTime(), &QcepDoubleProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_SaveAsTextTime(), &QcepDoubleProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_PerformIntegrationTime(), &QcepDoubleProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_DisplayIntegratedDataTime(), &QcepDoubleProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  connect(prop_SaveIntegratedDataTime(), &QcepDoubleProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);

  connect(prop_MaskPath(), &QcepStringProperty::valueChanged, this, &QxrdProcessor::onMaskPathChanged);
  connect(prop_DarkImagePath(), &QcepStringProperty::valueChanged, this, &QxrdProcessor::onDarkImagePathChanged);
  connect(prop_BadPixelsPath(), &QcepStringProperty::valueChanged, this, &QxrdProcessor::onBadPixelsPathChanged);
  connect(prop_GainMapPath(), &QcepStringProperty::valueChanged, this, &QxrdProcessor::onGainMapPathChanged);
}

QxrdProcessor::~QxrdProcessor()
{
}

void QxrdProcessor::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_MaskStack          -> initialize(sharedFromThis());
  m_ZingerFinder       -> initialize(sharedFromThis());
  m_CenterFinder       -> initialize(sharedFromThis());
  m_Integrator         -> initialize(sharedFromThis());
  m_PolarTransform     -> initialize(sharedFromThis());
  m_PolarNormalization -> initialize(sharedFromThis());
  m_GenerateTestImage  -> initialize(sharedFromThis());
  m_ROIVector          -> initialize(sharedFromThis());
  m_ROIModel           -> initialize(sharedFromThis());
  m_ROICalculator      -> initialize(sharedFromThis());

  QxrdAcquisitionPtr acqp(
        qSharedPointerDynamicCast<QxrdAcquisition>(acquisition()));

  if (acqp) {
    connect(acqp -> prop_SummedExposures(), &QcepIntProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
    connect(acqp -> prop_Raw16SaveTime(), &QcepDoubleProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
    connect(acqp -> prop_Raw32SaveTime(), &QcepDoubleProperty::valueChanged, this, &QxrdProcessor::updateEstimatedProcessingTime);
  }
}

//TODO: is this needed...
void QxrdProcessor::shutdown()
{
  thread()->exit();
}

QxrdProcessorWPtr QxrdProcessor::findProcessor(QcepObjectWPtr p)
{
  QxrdProcessorWPtr res =
      qSharedPointerDynamicCast<QxrdProcessor>(p);

  if (res == NULL) {
    QcepObjectPtr objp =
        qSharedPointerDynamicCast<QcepObject>(p);

    if (objp) {
      res = findProcessor(objp->parentPtr());
    }
  }

  return res;
}

QxrdExperimentWPtr QxrdProcessor::experiment() const
{
  QxrdExperimentWPtr expt(qSharedPointerDynamicCast<QxrdExperiment>(parentPtr()));

  if (expt == NULL) {
    printMessage("QxrdProcessor::experiment == NULL");
  }

  return expt;
}

QxrdAcqCommonWPtr QxrdProcessor::acquisition() const
{
  QxrdAcqCommonWPtr res;

  QxrdExperimentPtr expt(experiment());

  if (expt) {
    res =
        qSharedPointerDynamicCast<QxrdAcqCommon>(expt->acquisition());
  }

  return res;
}

QxrdDetectorSettingsWPtr QxrdProcessor::detector() const
{
  QxrdDetectorSettingsWPtr set =
      qSharedPointerDynamicCast<QxrdDetectorSettings>(parentPtr());

  return set;
}

QxrdFileSaverWPtr QxrdProcessor::fileSaver() const
{
  QxrdFileSaverWPtr res;

  QxrdExperimentPtr expt(experiment());

  if (expt) {
    res = expt->fileSaver();
  }

  return res;
}

QxrdCenterFinderWPtr QxrdProcessor::centerFinder() const
{
  if (m_CenterFinder == NULL) {
    printMessage("Problem QxrdProcessor::centerFinder == NULL");
  }

  return m_CenterFinder;
}

QxrdIntegratorPtr QxrdProcessor::integrator() const
{
  if (m_Integrator == NULL) {
    printMessage("Problem QxrdProcessor::integrator == NULL");
  }

  return m_Integrator;
}

QxrdGenerateTestImageWPtr QxrdProcessor::generateTestImage() const
{
  return m_GenerateTestImage;
}

QxrdROICalculatorPtr QxrdProcessor::roiCalculator() const
{
  return m_ROICalculator;
}

QxrdPowderRingsModelWPtr QxrdProcessor::powderRings() const
{
  if (m_PowderRings == NULL) {
    printMessage("Problem QxrdProcessor::powderRings == NULL");
  }

  return m_PowderRings;
}

QxrdROIModelWPtr QxrdProcessor::roiModel() const
{
  if (m_ROIModel == NULL) {
    printMessage("Problem QxrdProcessor::roiModel == NULL");
  }

  return m_ROIModel;
}

QxrdPolarTransformPtr QxrdProcessor::polarTransform() const
{
  if (m_PolarTransform == NULL) {
    printMessage("Problem QxrdProcessor::polarTransform == NULL");
  }

  return m_PolarTransform;
}

QxrdPolarNormalizationPtr QxrdProcessor::polarNormalization() const
{
  if (m_PolarNormalization == NULL) {
    printMessage("Problem QxrdProcessor::polarNormalization == NULL");
  }

  return m_PolarNormalization;
}

void QxrdProcessor::readSettings(QSettings *settings)
{
  QcepObject::readSettings(settings);

  if (m_MaskStack) {
    settings->beginGroup("maskStack");
    m_MaskStack -> readSettings(settings);
    settings->endGroup();
  }

  if (m_ZingerFinder) {
    settings->beginGroup("zingerFinder");
    m_ZingerFinder -> readSettings(settings);
    settings->endGroup();
  }

  if (m_CenterFinder) {
    settings->beginGroup("centerFinder");
    m_CenterFinder->readSettings(settings);
    settings->endGroup();
  }

  if (m_Integrator) {
    settings->beginGroup("integrator");
    m_Integrator   -> readSettings(settings);
    settings->endGroup();
  }

  if (m_PolarTransform) {
    settings->beginGroup("polarTransform");
    m_PolarTransform -> readSettings(settings);
    settings->endGroup();
  }

  if (m_PolarNormalization) {
    settings->beginGroup("polarNormalization");
    m_PolarNormalization -> readSettings(settings);
    settings->endGroup();
  }

  if (m_GenerateTestImage) {
    settings->beginGroup("generateTestImage");
    m_GenerateTestImage -> readSettings(settings);
    settings->endGroup();
  }

  if (m_ROIVector) {
    settings->beginGroup("roiVector");
    m_ROIVector->readSettings(settings);
    settings->endGroup();
  }

  if (m_ROICalculator) {
    settings->beginGroup("roiCalculator");
    m_ROICalculator->readSettings(settings);
    settings->endGroup();
  }

  int n = settings->beginReadArray("processorSteps");

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);

    QcepObjectPtr obj = QcepObject::readObject(settings);

    if (obj) {
      QxrdProcessorStepPtr s =
          qSharedPointerDynamicCast<QxrdProcessorStep>(obj);

      if (s) {
        s->initialize(sharedFromThis());

        m_ProcessorSteps.append(s);
      }
    }
  }

  settings->endArray();
}

void QxrdProcessor::writeSettings(QSettings *settings)
{
  QcepObject::writeSettings(settings);

  if (m_MaskStack) {
    settings->beginGroup("maskStack");
    m_MaskStack->writeSettings(settings);
    settings->endGroup();
  }

  if (m_ZingerFinder) {
    settings->beginGroup("zingerFinder");
    m_ZingerFinder -> writeSettings(settings);
    settings->endGroup();
  }

  if (m_CenterFinder) {
    settings->beginGroup("centerFinder");
    m_CenterFinder->writeSettings(settings);
    settings->endGroup();
  }

  if (m_Integrator) {
    settings->beginGroup("integrator");
    m_Integrator   -> writeSettings(settings);
    settings->endGroup();
  }

  if (m_PolarTransform) {
    settings->beginGroup("polarTransform");
    m_PolarTransform -> writeSettings(settings);
    settings->endGroup();
  }

  if (m_PolarNormalization) {
    settings->beginGroup("polarNormalization");
    m_PolarNormalization -> writeSettings(settings);
    settings->endGroup();
  }

  if (m_GenerateTestImage) {
    settings->beginGroup("generateTestImage");
    m_GenerateTestImage -> writeSettings(settings);
    settings->endGroup();
  }

  if (m_ROICalculator) {
    settings->beginGroup("roiCalculator");
    m_ROICalculator->writeSettings(settings);
    settings->endGroup();
  }

  settings->beginWriteArray("processorSteps");

  for (int i=0; i<m_ProcessorSteps.count(); i++) {
    settings->setArrayIndex(i);

    QxrdProcessorStepPtr s = m_ProcessorSteps.value(i);

    if (s) {
      s -> writeSettings(settings);
    }
  }

  settings->endArray();
}

QString QxrdProcessor::pwd() const
{
  return dataDirectory();
}

void QxrdProcessor::cd(QString path)
{
  QDir dir(currentDirectory());

  if (dir.cd(path)) {
    setDataDirectory(dir.path());
  }
}

QStringList QxrdProcessor::ls() const
{
  QStringList res;
  QDir dir(dataDirectory());

  res = dir.entryList(QStringList());

  return res;
}

QStringList QxrdProcessor::ls(QString pattern) const
{
  QStringList res;
  QDir dir(dataDirectory());

  res = dir.entryList(QStringList(pattern));

  return res;
}

void QxrdProcessor::loadData(QString name)
{
  THREAD_CHECK;

  if (qcepDebug(DEBUG_FILES)) {
    printMessage(
          tr("QxrdProcessor::loadData(%1)").arg(name));
  }

  QcepDoubleImageDataPtr res = QcepAllocator::newDoubleImage("data", 0,0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  if (res && res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();

    int typ = res->get_DataType();

    if ((typ == QcepDoubleImageData::Raw16Data) ||
        (typ == QcepDoubleImageData::Raw32Data))
    {
      subtractDarkImage(res, m_Dark);
    }

    newData(res);

    set_DataPath(res -> get_FileName());

    printMessage(tr("Loaded data from %1").arg(path));
  }
}

void QxrdProcessor::saveData(QString name, int canOverwrite)
{
  THREAD_CHECK;

  QString path = filePathInDataDirectory(name);

  saveNamedImageData(path, data(), QcepMaskDataPtr(), canOverwrite);

  set_DataPath(m_Data -> get_FileName());
}

void QxrdProcessor::clearData()
{
  THREAD_CHECK;

  newData(QcepDoubleImageDataPtr());

  set_DataPath("");
}

QcepImageDataBaseWPtr QxrdProcessor::data() const
{
  return m_Data;
}

void QxrdProcessor::newData(QcepImageDataBaseWPtr image)
{
  THREAD_CHECK;

  m_Data = image;
//  m_Overflow = overflow;

//  QxrdWindowPtr w = m_Window;

//  if (m_Data) {
  QcepDoubleImageDataPtr dimg(qSharedPointerDynamicCast<QcepDoubleImageData>(image));

  if (dimg) {
    emit dataAvailable(dimg);
  } else {
    printMessage("QxrdProcessor::newData != double");
  }
//  }

//  if (w) {
//    w -> newDataAvailable(m_Data, overflow);
//  }

//  if (m_CenterFinder) {
//    m_CenterFinder->setData(m_Data);
//  }

//  QxrdExperimentPtr exp(experiment());

//  if (exp) {
//    QcepDatasetModelPtr ds = exp->dataset();

//    if (ds && image) {
//      ds->append(image->get_Name(), image);
//    }
//  }
}

QString QxrdProcessor::filePathInDataDirectory(QString name) const
{
  return QDir(dataDirectory()).filePath(name);
}

QString QxrdProcessor::currentDirectory() const
{
  return dataDirectory();
}

QString QxrdProcessor::dataDirectory() const
{
  QxrdExperimentPtr exp(experiment());

  if (exp) {
    return QDir(exp->get_ExperimentDirectory()).filePath(exp->get_DataDirectory());
  } else {
    return QString();
  }
}

//TODO: need to write...
void QxrdProcessor::setDataDirectory(QString d)
{
}

QString QxrdProcessor::existingOutputDirectory(QString dir, QString subdir) const
{
  return QDir(dir).filePath(subdir);
}

QString QxrdProcessor::filePathInExperimentDirectory(QString name) const
{
  return QDir(experimentDirectory()).filePath(name);
}

QString QxrdProcessor::experimentDirectory() const
{
  QxrdExperimentPtr exp(experiment());

  if (exp) {
    return exp->get_ExperimentDirectory();
  } else {
    return QString();
  }
}

QString QxrdProcessor::filePathInDarkOutputDirectory(QString name) const
{
  return QDir(darkOutputDirectory()).filePath(name);
}

QString QxrdProcessor::darkOutputDirectory() const
{
  if (get_SaveDarkInSubdirectory()) {
    return existingOutputDirectory(dataDirectory(), get_SaveDarkSubdirectory());
  } else {
    return dataDirectory();
  }
}

QString QxrdProcessor::filePathInRawOutputDirectory(QString name) const
{
  return QDir(rawOutputDirectory()).filePath(name);
}

QString QxrdProcessor::rawOutputDirectory() const
{
  if (get_SaveRawInSubdirectory()) {
    return existingOutputDirectory(dataDirectory(), get_SaveRawSubdirectory());
  } else {
    return dataDirectory();
  }
}

QString QxrdProcessor::filePathInSubtractedOutputDirectory(QString name) const
{
  return QDir(subtractedOutputDirectory()).filePath(name);
}

QString QxrdProcessor::subtractedOutputDirectory() const
{
  if (get_SaveSubtractedInSubdirectory()) {
    return existingOutputDirectory(dataDirectory(), get_SaveSubtractedSubdirectory());
  } else {
    return dataDirectory();
  }
}

QString QxrdProcessor::filePathInIntegratedOutputDirectory(QString name) const
{
  return QDir(integratedOutputDirectory()).filePath(name);
}

QString QxrdProcessor::integratedOutputDirectory() const
{
  if (get_SaveIntegratedInSubdirectory()) {
    return existingOutputDirectory(dataDirectory(), get_SaveIntegratedSubdirectory());
  } else {
    return dataDirectory();
  }
}

int QxrdProcessor::status(double time)
{
  QMutex mutex;
  QcepMutexLocker lock(__FILE__, __LINE__, &mutex);

  if (getAcquiredCount() == 0) {
    return 1;
  }

  if (m_ProcessWaiting.wait(&mutex, (int)(time*1000))) {
    return getAcquiredCount()==0;
  } else {
    return 0;
  }
}

int QxrdProcessor::incrementAcquiredCount()
{
  return m_AcquiredCount.fetchAndAddOrdered(+1) + 1;
}

int QxrdProcessor::decrementAcquiredCount()
{
  int res = m_AcquiredCount.fetchAndAddOrdered(-1) - 1;

  if (res == 0) {
    m_ProcessWaiting.wakeAll();
  }

  return res;
}

int QxrdProcessor::getAcquiredCount()
{
  return m_AcquiredCount.fetchAndAddOrdered(0);
}

void QxrdProcessor::loadDark(QString name)
{
  THREAD_CHECK;

  if (qcepDebug(DEBUG_FILES)) {
    printMessage(
          tr("QxrdProcessor::loadDark(%1)").arg(name));
  }

  QcepDoubleImageDataPtr res =
      QcepAllocator::newDoubleImage("dark", 0,0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  if (res && res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QcepDoubleImageData::DarkData);

    newDark(res);

    set_DarkImagePath(res -> get_FileName());

    printMessage(tr("Loaded Dark Image from %1").arg(path));
  } else {
    printMessage(tr("loadDark(%1) failed").arg(name));
  }
}

void QxrdProcessor::saveDark(QString name, int canOverwrite)
{
  THREAD_CHECK;

  QString path = filePathInDataDirectory(name);

  if (m_Dark) {
    saveNamedImageData(path, m_Dark, QcepMaskDataPtr(), canOverwrite);

    set_DarkImagePath(m_Dark -> get_FileName());
  }
}

void QxrdProcessor::clearDark()
{
  newDark(QcepDoubleImageDataPtr());

  set_DarkImagePath("");
}

QcepDoubleImageDataWPtr QxrdProcessor::dark() const
{
  return m_Dark;
}

void QxrdProcessor::newDark(QcepDoubleImageDataWPtr imageW)
{
  m_Dark = imageW;

  if (m_Dark) {
    set_DarkImagePath(m_Dark->get_FileName());

    int height = m_Dark->get_Height();
    int width  = m_Dark->get_Width();
    int ndrk = m_Dark -> get_SummedExposures();
    int npixels = width*height;
    if (ndrk <= 0) ndrk = 1;

    double *dk     = m_Dark->data();
    double avgdark = 0;

    for (int i=0; i<npixels; i++) {
      avgdark  += dk[i];
    }
    set_AverageDark(avgdark/npixels/ndrk);
  } else {
    set_DarkImagePath("");
    set_AverageDark(0);
  }
}

void QxrdProcessor::newDarkInt16(QcepUInt16ImageDataWPtr imageW)
{
  QcepUInt16ImageDataPtr image(imageW);

  if (image) {
    QcepDoubleImageDataPtr d
        = QcepAllocator::newDoubleImage("dark",
                                        image->get_Width(),
                                        image->get_Height(),
                                        QcepAllocator::NullIfNotAvailable);

    if (d) {
      d -> copyFrom(image);
      newDark(d);
    }
  } else {
    newDark(QcepDoubleImageDataPtr(NULL));
  }
}

void QxrdProcessor::newDarkInt32(QcepUInt32ImageDataWPtr imageW)
{
  QcepUInt32ImageDataPtr image(imageW);

  if (image) {
    QcepDoubleImageDataPtr d
        = QcepAllocator::newDoubleImage("dark",
                                        image->get_Width(),
                                        image->get_Height(),
                                        QcepAllocator::NullIfNotAvailable);

    if (d) {
      d -> copyFrom(image);
      newDark(d);
    }
    //  set_DarkImagePath(m_DarkFrame -> get_FileName());
  } else {
    newDark(QcepDoubleImageDataPtr(NULL));
  }
}

void QxrdProcessor::onDarkImagePathChanged(QString newPath)
{
  if (newPath.length() == 0) {
    printMessage("Clear Dark Image");
    m_Dark = QcepDoubleImageDataPtr();
  } else {
    printMessage(tr("Load Dark Image from %1").arg(newPath));

    QcepDoubleImageDataPtr dark = QcepAllocator::newDoubleImage("dark", 0,0, QcepAllocator::NullIfNotAvailable);

    if (dark && dark -> readImage(newPath)) {
      m_Dark = dark;
    }
  }
}

void QxrdProcessor::loadMask(QString name)
{
  THREAD_CHECK;

  if (qcepDebug(DEBUG_FILES)) {
    printMessage(tr("QxrdProcessor::loadMask(%1)").arg(name));
  }

  QcepMaskDataPtr res =
      QcepAllocator::newMask("mask", 0,0, 0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  if (res && res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QcepMaskData::MaskData);

    //    res -> copyMaskTo(m_Mask);

    newMask(res);

    set_MaskPath(res -> get_FileName());

    printMessage(tr("Loaded Mask from %1").arg(path));
  } else {
    printMessage(tr("loadMask(%1) failed").arg(name));
  }
}

void QxrdProcessor::saveMask(QString name, int canOverwrite)
{
  THREAD_CHECK;

  if (m_MaskStack) {
    QString path = filePathInDataDirectory(name);

    QcepMaskDataPtr m(m_MaskStack->first());

    if (m) {
      saveNamedMaskData(path, m, canOverwrite);

      set_MaskPath(m -> get_FileName());
    }
  }
}

void QxrdProcessor::clearMask()
{
  THREAD_CHECK;

  if (m_MaskStack) {
    m_MaskStack -> clearMaskStack();
  }

  set_MaskPath("");
}

QcepMaskDataWPtr QxrdProcessor::mask() const
{
  QcepMaskDataWPtr res;

  if (m_MaskStack) {
    res = m_MaskStack->first();
  }

  return res;
}

void QxrdProcessor::newMask(QcepMaskDataWPtr mask)
{
  if (m_MaskStack) {
    m_MaskStack->push(mask);

    emit maskAvailable(mask);
  }

//  QxrdWindowPtr w = m_Window;

//  if (w) {
//    w -> newMaskAvailable(mask());
//  }
}

void QxrdProcessor::loadBadPixels(QString name)
{
  if (qcepDebug(DEBUG_FILES)) {
    printMessage(tr("QxrdProcessor::loadBadPixels(%1)").arg(name));
  }

  QcepDoubleImageDataPtr res = QcepAllocator::newDoubleImage("bad", 0,0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  if (res && res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QcepDoubleImageData::BadPixelsData);

    newBadPixelsImage(res);

    set_BadPixelsPath(res -> get_FileName());
  }
}

void QxrdProcessor::saveBadPixels(QString name, int canOverwrite)
{
  QString path = filePathInDataDirectory(name);

  if (m_BadPixels) {
    saveNamedImageData(path, m_BadPixels, QcepMaskDataPtr(), canOverwrite);

    set_BadPixelsPath(m_BadPixels -> get_FileName());
  }
}

void QxrdProcessor::clearBadPixels()
{
  newBadPixelsImage(QcepDoubleImageDataPtr());

  set_BadPixelsPath("");
}

QcepDoubleImageDataWPtr QxrdProcessor::badPixels() const
{
  return m_BadPixels;
}

void QxrdProcessor::newBadPixelsImage(QcepDoubleImageDataWPtr image)
{
  m_BadPixels = image;

  if (m_BadPixels) {
    set_BadPixelsPath(m_BadPixels->get_FileName());
  } else {
    set_BadPixelsPath("");
  }
}

void QxrdProcessor::onBadPixelsPathChanged(QString newPath)
{
  if (newPath.length() == 0) {
    printMessage("Clear Bad Pixels");
    m_BadPixels = QcepDoubleImageDataPtr();
  } else {
    printMessage(tr("Load Bad Pixels from %1").arg(newPath));

    QcepDoubleImageDataPtr bad = QcepAllocator::newDoubleImage("bad", 0,0, QcepAllocator::NullIfNotAvailable);

    if (bad && bad->readImage(newPath)) {
      m_BadPixels = bad;
    }
  }
}

void QxrdProcessor::loadGainMap(QString name)
{
  if (qcepDebug(DEBUG_FILES)) {
    printMessage(tr("QxrdProcessor::loadGainMap(%1)").arg(name));
  }

  QcepDoubleImageDataPtr res = QcepAllocator::newDoubleImage("gain", 0,0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  if (res && res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QcepDoubleImageData::GainData);
    res -> setDefaultValue(1.0);

    newGainMapImage(res);

    set_GainMapPath(res -> get_FileName());
  }
}

void QxrdProcessor::saveGainMap(QString name, int canOverwrite)
{
  QString path = filePathInDataDirectory(name);

  if (m_GainMap) {
    saveNamedImageData(path, m_GainMap, QcepMaskDataPtr(), canOverwrite);

    set_GainMapPath(m_GainMap -> get_FileName());
  }
}

void QxrdProcessor::clearGainMap()
{
  newGainMapImage(QcepDoubleImageDataPtr());

  set_GainMapPath("");
}

QcepDoubleImageDataWPtr QxrdProcessor::gainMap() const
{
  return m_GainMap;
}


void QxrdProcessor::newGainMapImage(QcepDoubleImageDataWPtr image)
{
  m_GainMap = image;

  if (m_GainMap) {
    set_GainMapPath(m_GainMap->get_FileName());
  } else {
    set_GainMapPath("");
  }
}

void QxrdProcessor::onGainMapPathChanged(QString newPath)
{
  if (newPath.length() == 0) {
    printMessage("Clear Gain Map");
  } else {
    printMessage(tr("Load Gain Map from %1").arg(newPath));
  }
}

QcepDoubleImageDataWPtr QxrdProcessor::liveData() const
{
  return m_LiveData;
}

void QxrdProcessor::newLiveData(QcepDoubleImageDataWPtr image)
{
  m_LiveData = image;

  emit liveDataAvailable(m_LiveData);
}

QcepMaskDataWPtr QxrdProcessor::overflow() const
{
  return m_Overflow;
}

void QxrdProcessor::newOverflow(QcepMaskDataWPtr ovf)
{
  m_Overflow = ovf;

  emit overflowAvailable(ovf);
}

//TODO: don't load things that would be loaded by readSettings
void QxrdProcessor::loadDefaultImages()
{
  QString fileName = get_MaskPath();
  QFileInfo fileInfo(fileName);

//  if (fileInfo.exists() && fileInfo.isFile()) {
//    loadMask(fileName);
//  }

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

void QxrdProcessor::subtractDark()
{
  QcepDoubleImageDataPtr data = qSharedPointerDynamicCast<QcepDoubleImageData>(m_Data);
  QcepDoubleImageDataPtr dark = m_Dark;

  subtractDarkImage(data, dark);
}

void QxrdProcessor::unsubtractDark()
{
  QcepDoubleImageDataPtr data = qSharedPointerDynamicCast<QcepDoubleImageData>(m_Data);
  QcepDoubleImageDataPtr dark = m_Dark;

  unsubtractDarkImage(data, dark);
}

void QxrdProcessor::multiplyData(double scalar)
{
  QcepDoubleImageDataPtr dat = qSharedPointerDynamicCast<QcepDoubleImageData>(m_Data);

  int wid = dat->get_Width();
  int ht  = dat->get_Height();

  for (int y=0; y<ht; y++) {
    for (int x=0; x<wid; x++) {
      dat->setValue(x, y, dat->value(x, y) * scalar);
    }
  }
}

void QxrdProcessor::offsetData(double offset)
{
  QcepDoubleImageDataPtr dat = qSharedPointerDynamicCast<QcepDoubleImageData>(m_Data);

  int wid = dat->get_Width();
  int ht  = dat->get_Height();

  for (int y=0; y<ht; y++) {
    for (int x=0; x<wid; x++) {
      dat->setValue(x, y, dat->value(x, y) + offset);
    }
  }
}

void QxrdProcessor::subtractDarkImage(QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark)
{
  //  if (get_PerformDarkSubtraction()) {
  if (dark && image) {
    if (dark->get_ExposureTime() != image->get_ExposureTime()) {
      printMessage("Exposure times of acquired data and dark image are different, skipping");
      return;
    }

    if (dark->get_Width() != image->get_Width() ||
        dark->get_Height() != image->get_Height()) {
      printMessage("Dimensions of acquired data and dark image are different, skipping");
      return;
    }

    if (dark->get_CameraGain() != image->get_CameraGain()) {
      printMessage("Gains of acquired data and dark image are different, skipping");
      return;
    }

//    if (!(image->get_DataType() == QcepDoubleImageData::Raw16Data ||
//          image->get_DataType() == QcepDoubleImageData::Raw32Data)) {
//      printMessage("Acquired data is not a raw image, skipping background subtraction");
//      return;
//    }

//    QcepMutexLocker lock1(__FILE__, __LINE__, dark->mutex());
//    QcepMutexLocker lock2(__FILE__, __LINE__, image->mutex());

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

    image -> set_DataType(QcepDoubleImageData::SubtractedData);
  }
  //  }
}

void QxrdProcessor::unsubtractDarkImage(QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark)
{
  //  if (get_PerformDarkSubtraction()) {
  if (dark && image) {
    if (dark->get_ExposureTime() != image->get_ExposureTime()) {
      printMessage("Exposure times of acquired data and dark image are different, skipping");
      return;
    }

    if (dark->get_Width() != image->get_Width() ||
        dark->get_Height() != image->get_Height()) {
      printMessage("Dimensions of acquired data and dark image are different, skipping");
      return;
    }

    if (dark->get_CameraGain() != image->get_CameraGain()) {
      printMessage("Gains of acquired data and dark image are different, skipping");
      return;
    }

//    if (!(image->get_DataType() == QcepDoubleImageData::Raw16Data ||
//          image->get_DataType() == QcepDoubleImageData::Raw32Data)) {
//      printMessage("Acquired data is not a raw image, skipping background subtraction");
//      return;
//    }

//    QcepMutexLocker lock1(__FILE__, __LINE__, dark->mutex());
//    QcepMutexLocker lock2(__FILE__, __LINE__, image->mutex());

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
      result[i] = result[i]+ratio*dk[i];
    }

    //      set_AverageDark(avgdark/npixels/ndrk);
    set_AverageRaw(avgraw/npixels/nres);
    set_Average(get_AverageRaw() - get_AverageDark());

    image -> set_DataType(QcepDoubleImageData::SubtractedData);
  }
  //  }
}

void QxrdProcessor::fixupBadBackgroundSubtraction(QString imagePattern, int nImgExposures, QString darkPath, int nDarkExposures)
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

void QxrdProcessor::idleInt16Image(QcepUInt16ImageDataPtr image, bool liveView)
{
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

QcepImageDataBasePtr QxrdProcessor::doDarkSubtraction(QcepImageDataBasePtr img)
{
  QcepDoubleImageDataPtr dark = m_Dark;
  QcepImageDataBasePtr   res  = img;

  if (img && dark) {
    if (img->get_ExposureTime() != dark->get_ExposureTime()) {
      printMessage("Exposure times of acquired data and dark image are different, skipping");
      return img;
    }

    if (img->get_Width() != dark->get_Width() ||
        img->get_Height() != dark->get_Height()) {
      printMessage("Dimensions of acquired data and dark image are different, skipping");
      return img;
    }

    if (img->get_CameraGain() != dark->get_CameraGain()) {
      printMessage("Gains of acquired data and dark image are different, skipping");
      return img;
    }

    int height = img->get_Height();
    int width  = img->get_Width();
    int nres = img -> get_SummedExposures();
    int ndrk = dark -> get_SummedExposures();
    int npixels = width*height;

    if (nres <= 0) nres = 1;


    double ratio = ((double) nres)/((double) ndrk);

    QcepDoubleImageDataPtr result = QcepAllocator::newDoubleImage("image",
                                                                  width, height,
                                                                  QcepAllocator::NullIfNotAvailable);

    if (result) {
      result->copyPropertiesFrom(img);

      double sumraw = 0, sumdark = 0;

      double *resptr = result->data();
      double *drkptr = dark->data();

      QcepUInt16ImageDataPtr i16 = qSharedPointerDynamicCast<QcepUInt16ImageData>(img);

      if (i16) {
        quint16 *imgptr = i16->data();

        for (int i=0; i<npixels; i++) {
          double valraw = imgptr[i];
          double valdark = drkptr[i];

          sumraw  += valraw;
          sumdark += valdark;

          resptr[i] = valraw - ratio*valdark;
        }
      } else {
        QcepUInt32ImageDataPtr i32 = qSharedPointerDynamicCast<QcepUInt32ImageData>(img);

        if (i32) {
          quint32 *imgptr = i32->data();

          for (int i=0; i<npixels; i++) {
            double valraw = imgptr[i];
            double valdark = drkptr[i];

            sumraw  += valraw;
            sumdark += valdark;

            resptr[i] = valraw - ratio*valdark;
          }
        } else {
          npixels = 0;

          for (int row=0; row<height; row++) {
            for (int col=0; col<width; col++) {
              double valraw  = img  -> getImageData(col, row);
              double valdark = dark -> getImageData(col, row);
              if (valraw == valraw && valdark == valdark) { // Check for NaNs
                sumraw += valraw; sumdark += valdark;
                npixels += 1;
                double resval = valraw - ratio*valdark;

                result->setImageData(col, row, resval);
              }
            }
          }
        }
      }
    }

    res = result;
  }

  return res;
}

QcepImageDataBasePtr QxrdProcessor::doBadPixels(QcepImageDataBasePtr img)
{
  printMessage("Bad Pixel Correction not yet implemented");

  return img;
}

QcepImageDataBasePtr QxrdProcessor::doGainCorrection(QcepImageDataBasePtr img)
{
  printMessage("Gain Correction not yet implemented");

  return img;
}

//QcepDoubleVector QxrdProcessor::doCalculateROICounts(QcepImageDataBasePtr img)
//{
//  QcepDoubleVector res;

//  if (img && m_ROICalculator) {
//    res = m_ROICalculator->values(img, mask());
//  }

//  if (qcepDebug(DEBUG_ACQUIRE)) {
//    QString s = "[";

//    for (int i=0; i<res.count(); i++) {
//      if (i == 0) {
//        s.append(tr("%1").arg(res.value(i)));
//      } else {
//        s.append(tr(", %1").arg(res.value(i)));
//      }
//    }

//    s.append("]");

//    printMessage(tr("ROI Values = %1").arg(s));
//  }

//  return res;
//}

void QxrdProcessor::doSaveRawImage(QcepImageDataBasePtr img, QcepMaskDataPtr ovf)
{
  QxrdExperimentPtr expt(experiment());
  QxrdFileSaverPtr  fsav(fileSaver());

  if (fsav && expt && img) {
    QString fullPath = filePathInRawOutputDirectory(img->get_FileBase());

    fsav->saveImageData(fullPath, img, ovf, QxrdFileSaver::NoOverwrite);
  }
}

void QxrdProcessor::doSaveDarkImage(QcepImageDataBasePtr img, QcepMaskDataPtr ovf)
{
  QxrdExperimentPtr expt(experiment());
  QxrdFileSaverPtr  fsav(fileSaver());

  if (fsav && expt && img) {
    QString fullPath = filePathInDarkOutputDirectory(img->get_FileBase());

    fsav->saveImageData(fullPath, img, ovf, QxrdFileSaver::NoOverwrite);
  }
}

void QxrdProcessor::doSaveSubtractedImage(QcepImageDataBasePtr img, QcepMaskDataPtr ovf)
{
  QxrdExperimentPtr expt(experiment());
  QxrdFileSaverPtr  fsav(fileSaver());

  if (fsav && expt && img) {
    QString fullPath = filePathInSubtractedOutputDirectory(img->get_FileBase());

    fsav->saveImageData(fullPath, img, ovf, QxrdFileSaver::NoOverwrite);
  }
}

void QxrdProcessor::setAcquiredImageProperties(QcepImageDataBasePtr image,
                                               int fileIndex,
                                               int phase,
                                               int nPhases,
                                               bool trig)
{
  QxrdAcquisitionPtr acq(
        qSharedPointerDynamicCast<QxrdAcquisition>(acquisition()));

  QxrdDetectorSettingsPtr det(detector());

  if (image && acq) {
    QDateTime now = QDateTime::currentDateTime();
    double msec = QcepImageDataBase::secondsSinceEpoch();

    image -> set_Name             (image -> get_FileBase());
    image -> set_ExposureTime     (acq   -> get_ExposureTime());
    image -> set_DateTime         (now);
    image -> set_TimeStamp        (msec);

    if (det) {
      image -> set_HBinning         (det   -> get_HBinning());
      image -> set_VBinning         (det   -> get_VBinning());
    } else {
      image -> set_HBinning         (1);
      image -> set_VBinning         (1);
    }

    image -> set_DataType(QcepUInt32ImageData::Raw32Data);

    image -> set_UserComment1     (acq   -> get_UserComment1());
    image -> set_UserComment2     (acq   -> get_UserComment2());
    image -> set_UserComment3     (acq   -> get_UserComment3());
    image -> set_UserComment4     (acq   -> get_UserComment4());
    image -> set_ObjectSaved      (false);
    image -> set_Triggered        (trig);
    image -> set_Normalization    (acq   -> get_Normalization());

    image -> set_ImageNumber      (fileIndex);
    image -> set_PhaseNumber      (phase);
    image -> set_NPhases          (nPhases);

    acq -> copyDynamicProperties(image.data());
  }
}

void QxrdProcessor::processAcquiredImage(QcepUInt32ImageDataPtr image,
                                         QcepMaskDataPtr overflow,
                                         int fileIndex,
                                         int phase,
                                         int nPhases,
                                         bool trig)
{
  THREAD_CHECK;

  if (image) {
    m_Data     = image;
    m_Overflow = overflow;

    QcepDoubleVector scalers;

    QcepImageDataBasePtr img = image;

    if (qcepDebug(DEBUG_ACQUIRE)) {
      printMessage(tr("QxrdDetectorProcessor::processAcquiredImage(\"%1\",...)")
                   .arg(img->get_FileName()));
    }

    QTime tic;
    tic.start();

    setAcquiredImageProperties(img, fileIndex, phase, nPhases, trig);

//    QxrdDetectorControlWindowPtr ctrl(m_ControlWindow);

    if (get_SaveRawImages()) {
      doSaveRawImage(img, overflow);

      int saveTime = tic.restart();

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Save took %1 msec").arg(saveTime));
      }
    }

    if (img && get_PerformDarkSubtraction()) {
      img = doDarkSubtraction(img);

      int subTime = tic.restart();

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Subtraction took %1 msec").arg(subTime));
      }
    }

    if (img && get_PerformBadPixels()) {
      img = doBadPixels(img);

      int pxlTime = tic.restart();

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Bd pixels took %1 msec").arg(pxlTime));
      }
    }

    if (img && get_PerformGainCorrection()) {
      img = doGainCorrection(img);

      int gainTime = tic.restart();

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Gain correction took %1 msec").arg(gainTime));
      }
    }

    QcepDoubleImageDataPtr dimg =
        qSharedPointerCast<QcepDoubleImageData>(img);

    if (dimg) {
      emit dataAvailable(dimg);
    }

//    if (ctrl && get_DetectorDisplayMode() == ImageDisplayMode) {
//      ctrl->displayNewData(img, overflow);

//      int displayTime = tic.restart();

//      if (qcepDebug(DEBUG_ACQUIRETIME)) {
//        printMessage(tr("Display took %1 msec").arg(displayTime));
//      }
//    }

    if (img && get_CalculateROICounts()) {
      const QcepDoubleVector s = doCalculateROICounts(img);

      scalers += s;

      set_RoiCounts(scalers);

      int roiTime = tic.restart();

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("ROI calculation took %1 msec").arg(roiTime));
      }
    }

    if (img && get_SaveSubtracted()) {
      doSaveSubtractedImage(img, overflow);

      int saveTime = tic.restart();

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Save Subtracted took %1 msec").arg(saveTime));
      }
    }

    //    if (get_PerformIntegration()) {
    //      integ = doPerformIntegration(img);

    //      if (ctrl && get_DisplayIntegratedData()) {
    //        ctrl->displayIntegratedData(integ);
    //      }

    //      if (get_SaveIntegratedData()) {
    //        doSaveIntegratedData(integ);
    //      }

    //      if (get_SaveIntegratedDataSeparate()) {
    //        doSaveIntegratedDataSeparate(integ);
    //      }

    //      if (get_AccumulateIntegrated2D()) {
    //        doAccumulateIntegrated2D(integ);
    //      }
    //    }
  }
}

void QxrdProcessor::processDarkImage(QcepDoubleImageDataPtr image,
                                             QcepMaskDataPtr overflow,
                                             int fileIndex)
{
  THREAD_CHECK;

  if (image) {
    if (qcepDebug(DEBUG_ACQUIRE)) {
      printMessage(tr("QxrdDetectorProcessor::processDarkImage(\"%1\",...)")
                   .arg(image->get_FileName()));
    }

    setAcquiredImageProperties(image, fileIndex, -1, 0, true);

    if (get_SaveDarkImages()) {
      doSaveDarkImage(image, overflow);

      set_DarkImagePath(image->get_FileName());
    }

    m_Dark = image;
  }
}

void QxrdProcessor::processIdleImage(QcepImageDataBasePtr image)
{
  THREAD_CHECK;

  if (image) {
    QxrdExperimentPtr expt(experiment());

    if (expt) {
      QxrdAcquisitionPtr acq(
            qSharedPointerDynamicCast<QxrdAcquisition>(acquisition()));

      if (acq && acq->get_LiveViewAtIdle()) {
        QcepDoubleVector scalers;

        QcepImageDataBasePtr img = image;

        if (qcepDebug(DEBUG_ACQUIRE)) {
          printMessage(tr("QxrdDetectorProcessor::processIdleImage(\"%1\")")
                       .arg(image->get_FileName()));
        }

        QTime tic;
        tic.start();

        setAcquiredImageProperties(img, -1, -1, 0, true);

//        QxrdDetectorControlWindowPtr ctrl(m_ControlWindow);

        if (img && get_PerformDarkSubtraction()) {
          img = doDarkSubtraction(img);

          int subTime = tic.restart();

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("Subtraction took %1 msec").arg(subTime));
          }
        }

        if (img && get_PerformBadPixels()) {
          img = doBadPixels(img);

          int pxlTime = tic.restart();

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("Bd pixels took %1 msec").arg(pxlTime));
          }
        }

        if (img && get_PerformGainCorrection()) {
          img = doGainCorrection(img);

          int gainTime = tic.restart();

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("Gain correction took %1 msec").arg(gainTime));
          }
        }

//        if (ctrl && get_DetectorDisplayMode() == ImageDisplayMode) {
//          ctrl->displayNewData(img, QcepMaskDataWPtr());

//          int displayTime = tic.restart();

//          if (qcepDebug(DEBUG_ACQUIRETIME)) {
//            printMessage(tr("Display took %1 msec").arg(displayTime));
//          }
//        }

        if (img && get_CalculateROICounts()) {
          const QcepDoubleVector s = doCalculateROICounts(img);

          scalers += s;

          set_RoiCounts(scalers);

          int roiTime = tic.restart();

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("ROI calculation took %1 msec").arg(roiTime));
          }
        }
      }
    }
  }
}

QcepDoubleImageDataPtr QxrdProcessor::processAcquiredInt16Image(
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
        saveNamedRawImageData(img->rawFileName(), img, overflow, QxrdProcessor::NoOverwrite);
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

QcepDoubleImageDataPtr QxrdProcessor::processAcquiredInt32Image(
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
        saveNamedRawImageData(img->rawFileName(), img, overflow, QxrdProcessor::NoOverwrite);
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

QcepDoubleImageDataPtr QxrdProcessor::processAcquiredDoubleImage(
    QcepDoubleImageDataPtr processed,
    QcepDoubleImageDataPtr dimg,
    QcepDoubleImageDataPtr dark,
    QcepMaskDataPtr mask,
    QcepMaskDataPtr overflow)
{
  return processAcquiredImage(processed, dimg, dark, mask, overflow);
}

QcepDoubleImageDataPtr QxrdProcessor::processAcquiredDoubleImage(
    QcepDoubleImageDataPtr processed,
    QcepDoubleImageDataPtr dimg,
    QcepDoubleImageDataPtr dark,
    QcepMaskDataPtr mask,
    QcepMaskDataPtr overflow,
    QcepDoubleList v)
{
  return processAcquiredImage(processed, dimg, dark, mask, overflow, v);
}

QcepDoubleImageDataPtr QxrdProcessor::processAcquiredImage(
    QcepDoubleImageDataPtr processed,
    QcepDoubleImageDataPtr img,
    QcepDoubleImageDataPtr dark,
    QcepMaskDataPtr /*mask*/,
    QcepMaskDataPtr overflow,
    QcepDoubleList v)
{
  if (processed && img) {
    processed->copyFrom(img);

    statusMessage(tr("Processing Image \"%1\"").arg(processed->get_FileName()));

    QTime tic;
    tic.start();

    if (v.length() > 0) {
      processed->set_Normalization(v);
    }

    if (qcepDebug(DEBUG_PROCESS)) {
      printMessage(tr("Processing Image \"%1\", image number %2, count %3")
                   .arg(processed->get_FileName()).arg(processed->get_ImageNumber()).arg(getAcquiredCount()));
    }

    if (get_PerformDarkSubtraction()) {
      subtractDarkImage(processed, dark);
      processed -> set_ObjectSaved(false);

      int subTime = tic.restart();

      updateEstimatedTime(prop_PerformDarkSubtractionTime(), subTime);

      if (qcepDebug(DEBUG_PROCESS)) {
        printMessage(tr("Dark subtraction took %1 msec").arg(subTime));
      }
    }

    if (get_PerformBadPixels()) {
      correctBadPixels(processed);
      processed -> set_ObjectSaved(false);

      int badPxlTime = tic.restart();

      updateEstimatedTime(prop_PerformBadPixelsTime(), badPxlTime);

      if (qcepDebug(DEBUG_PROCESS)) {
        printMessage(tr("Bad Pixel correction took %1 msec").arg(badPxlTime));
      }
    }

    if (get_PerformGainCorrection()) {
      correctImageGains(processed);
      processed -> set_ObjectSaved(false);

      int gainTime = tic.restart();

      updateEstimatedTime(prop_PerformGainCorrectionTime(), gainTime);

      if (qcepDebug(DEBUG_PROCESS)) {
        printMessage(tr("Gain correction took %1 msec").arg(gainTime));
      }
    }

    if (get_SaveSubtracted()) {
      if (processed->get_ObjectSaved()) {
        printMessage(tr("Image \"%1\" is already saved").arg(processed->rawFileName()));
      } else {
        saveNamedImageData(QDir(subtractedOutputDirectory()).filePath(processed->get_FileBase()), processed, overflow);
      }
    }

    if (get_SaveAsText()) {
      saveNamedImageDataAsText(processed->get_FileName(), processed, overflow);

      updateEstimatedTime(prop_SaveAsTextTime(), tic.elapsed());
    }

    newData(processed);
    newOverflow(overflow);

    if (qcepDebug(DEBUG_PROCESS)) {
      printMessage(tr("Processing took %1 msec").arg(tic.restart()));
    }

    statusMessage(tr("Completed Processing Image \"%1\"").arg(processed->get_FileName()));
  }

  return processed;
}

void QxrdProcessor::saveNamedImageData(QString name, QcepImageDataBasePtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveImageData(name, image, overflow, canOverwrite);
  }
}
void QxrdProcessor::saveNamedDoubleImageData(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveDoubleData(name, image, overflow, canOverwrite);
  }
}

void QxrdProcessor::saveNamedUInt16ImageData(QString name, QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveInt16Data(name, image, overflow, canOverwrite);
  }
}

void QxrdProcessor::saveNamedRawImageData(QString name, QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveRaw16Data(name, image, overflow, canOverwrite);
  }
}

void QxrdProcessor::saveNamedUInt32ImageData(QString name, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveInt32Data(name, image, overflow, canOverwrite);
  }
}

void QxrdProcessor::saveNamedRawImageData(QString name, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveRaw32Data(name, image, overflow, canOverwrite);
  }
}

void QxrdProcessor::saveNamedMaskData(QString name, QcepMaskDataPtr image, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveMaskData(name, image, canOverwrite);
  }
}

void QxrdProcessor::saveNamedImageDataAsText(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    f -> saveTextData(name, image, overflow, canOverwrite);
  }
}

void QxrdProcessor::saveCachedGeometry(QString name)
{
  QString path = filePathInDataDirectory(name);

  QcepUInt32ImageDataPtr data = m_Integrator->cachedGeometry();

  if (data) {
    saveNamedImageData(path, data, QcepMaskDataPtr(), true);
  }
}

void QxrdProcessor::saveCachedIntensity(QString name)
{
  QString path = filePathInDataDirectory(name);

  QcepDoubleImageDataPtr data = m_Integrator->cachedIntensity();

  if (data) {
    saveNamedImageData(path, data, QcepMaskDataPtr(), true);
  }
}

void QxrdProcessor::updateEstimatedTime(QcepDoubleProperty *prop, int msec)
{
  double newVal = prop -> value() * (1.0 - get_AveragingRatio()) + ((double) msec)/1000.0* get_AveragingRatio();

  prop -> setValue(newVal);
}

void QxrdProcessor::updateEstimatedProcessingTime()
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

double QxrdProcessor::estimatedProcessingTime(double estSerialTime, double estParallelTime)
{
  int nThreads = QThreadPool::globalInstance()->maxThreadCount();

  if (nThreads >= 2) {
    return qMax(estSerialTime, estParallelTime/((double)nThreads));
  } else {
    return estSerialTime + estParallelTime;
  }
}

void QxrdProcessor::onMaskPathChanged(QString newPath)
{
  if (newPath.length() == 0) {
    printMessage("Clear Mask");
    m_MaskStack->clearMaskStack();
  } else {
    printMessage(tr("Load mask from %1").arg(newPath));

    QcepMaskDataPtr m = QcepAllocator::newMask(newPath, 0,0, 0, QcepAllocator::NullIfNotAvailable);

    if (m && m->readImage(newPath)) {
      m_MaskStack->push(m);

      emit maskAvailable(m);
    }
  }
}

int QxrdProcessor::newMaskWidth() const
{
  QcepImageDataBasePtr   d(data());
  QcepMaskDataPtr        m(mask());

  int w=0;

  if (d) {
    w = d->get_Width();
  } else if (m) {
    w = m->get_Width();
  }

  return w;
}

int QxrdProcessor::newMaskHeight() const
{
  QcepImageDataBasePtr   d(data());
  QcepMaskDataPtr        m(mask());

  int h=0;

  if (d) {
    h = d->get_Height();
  } else if (m) {
    h = m->get_Height();
  }

  return h;
}

QxrdMaskStackWPtr QxrdProcessor::maskStack() const
{
  return m_MaskStack;
}

void QxrdProcessor::newEmptyMask()
{

  QcepMaskDataPtr m = QcepAllocator::newMask("mask",
                                             newMaskWidth(), newMaskHeight(), 1,
                                             QcepAllocator::NullIfNotAvailable);

  if (m && m_MaskStack) {
    m_MaskStack -> push(m);

    printMessage(tr("new mask, %1 on stack").arg(m_MaskStack -> maskCount()));

    newMask(m);
  }
}

void QxrdProcessor::duplicateMask()
{
  QcepMaskDataPtr m =  QcepAllocator::newMask("mask",
                                              newMaskWidth(), newMaskHeight(), 1,
                                              QcepAllocator::NullIfNotAvailable);

  if (m_MaskStack) {
    QcepMaskDataPtr m1 = mask();

    if (m1) {
      m1->copyMaskTo(m);
    }

    m_MaskStack -> push(m);

    printMessage(tr("dup mask, %1 on stack").arg(m_MaskStack -> maskCount()));

    newMask(m);
  }
}

void QxrdProcessor::showMaskRange()
{
  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  showMaskRange(min, max);
}

void QxrdProcessor::showMaskRange(double min, double max)
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m_Data && m) {
    m -> showMaskRange(m_Data, min, max);
  }
}

void QxrdProcessor::hideMaskAll()
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    m -> hideMaskAll();
  }
}

void QxrdProcessor::showMaskAll()
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    m -> showMaskAll();
  }
}

void QxrdProcessor::hideMaskRange()
{
  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  hideMaskRange(min, max);
}

void QxrdProcessor::hideMaskRange(double min, double max)
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m_Data && m) {
    m -> hideMaskRange(m_Data, min, max);
  }
}

void QxrdProcessor::invertMask()
{
  QcepMaskDataPtr m(mask());

  if (m) {
    m -> invertMask();
  }
}

void QxrdProcessor::growMask()
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    m -> growMask();
  }
}

void QxrdProcessor::shrinkMask()
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    m -> shrinkMask();
  }
}

void QxrdProcessor::maskCircle(QRectF rect)
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    if ((rect.left() == rect.right()) && (rect.bottom() == rect.top())) {
      m -> maskCircle(rect.left(), rect.top(), get_MaskCircleRadius(), get_MaskSetPixels());
    } else {
      double cx = rect.center().x();
      double cy = rect.center().y();
      double rad = rect.width()/2;

      m -> maskCircle(cx, cy, rad, get_MaskSetPixels());
    }
  }
}

void QxrdProcessor::maskPolygon(QVector<QPointF> poly)
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    //  printf("QxrdProcessor::maskPolygon(%d points ...)\n", poly.size());
    bool newVal = get_MaskSetPixels();
    m -> maskPolygon(poly, newVal);
//    int nRows = m -> get_Height();
//    int nCols = m -> get_Width();

//    QImage polyImage(nCols, nRows, QImage::Format_RGB32);
//    QPainter polyPainter(&polyImage);
//    QPolygonF polygon;

//    foreach(QPointF pt, poly) {
//      polygon.append(pt);
//    }

//    polyPainter.setPen(Qt::white);
//    polyPainter.fillRect(0,0,nCols,nRows,Qt::black);
//    polyPainter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
//    polyPainter.drawPolygon(poly);


//    for (int j=0; j<nRows; j++) {
//      for (int i=0; i<nCols; i++) {
//        if (qGray(polyImage.pixel(i,j))) {
//          m -> setMaskValue(i, j, newval);
//        }
//      }
//    }
  }
}

void QxrdProcessor::createMaskIfNeeded()
{
  if (m_MaskStack == NULL) {
    m_MaskStack = QxrdMaskStackPtr(new QxrdMaskStack("maskStack"));
  }

  if (m_MaskStack && m_MaskStack -> isEmpty()) {
    newEmptyMask();
  }
}

QxrdZingerFinderWPtr QxrdProcessor::zingerFinder() const
{
  return m_ZingerFinder;
}

void QxrdProcessor::correctBadPixels(QcepDoubleImageDataPtr /*image*/)
{
}

void QxrdProcessor::correctImageGains(QcepDoubleImageDataPtr image)
{
  if (image) {
    QcepDoubleImageDataPtr gains = gainMap();

    if (gains) {
      image -> multiply(gains);
    }
  }
}

QcepDoubleImageDataPtr QxrdProcessor::correctDoubleImage
    (QcepDoubleImageDataPtr corrected, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow)
{
  QThread::currentThread()->setObjectName("correctDoubleImage");

  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("QxrdProcessor::correctDoubleImage"));
  }

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      return processAcquiredDoubleImage(corrected, image, dark, mask, overflow);
    } else {
      if (get_SaveDarkImages()) {
        saveNamedImageData(image->get_FileName(), image, overflow);

        set_DarkImagePath(image->get_FileName());
      }

      newDark(image);
    }
  }

  return QcepDoubleImageDataPtr();
}

QcepDoubleImageDataPtr QxrdProcessor::correctDoubleImage
    (QcepDoubleImageDataPtr corrected, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr overflow, QcepDoubleList v)
{
  QThread::currentThread()->setObjectName("correctDoubleImage");

  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("QxrdProcessor::correctDoubleImage"));
  }

  if (image) {
    if ((image -> get_ImageNumber()) >= 0) {
      return processAcquiredDoubleImage(corrected, image, dark, image->mask(), overflow, v);
    } else {
      if (get_SaveDarkImages()) {
        saveNamedImageData(image->get_FileName(), image, overflow);

        set_DarkImagePath(image->get_FileName());
      }

      newDark(image);
    }
  }

  return QcepDoubleImageDataPtr();
}

QcepIntegratedDataPtr QxrdProcessor::integrateImage
    (QcepDoubleImageDataPtr image, QcepMaskDataPtr mask, double /*cx*/, double /*cy*/)
{
  QcepIntegratedDataPtr res;

  QThread::currentThread()->setObjectName("integrateImage");

  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("QxrdProcessor::integrateImage"));
  }

  if (image && get_PerformIntegration()) {
    QTime tic;
    tic.start();

    res = m_Integrator -> performIntegration(image, mask);

    updateEstimatedTime(prop_PerformIntegrationTime(), tic.restart());
  }

  return res;
}

void QxrdProcessor::integrateData(QString name)
{
  QThread::currentThread()->setObjectName("integrateData");

  QcepDoubleImageDataPtr img =
      QcepAllocator::newDoubleImage("image", 0,0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  QxrdCenterFinderPtr cf(centerFinder());

  if (cf && img && img -> readImage(path)) {
    printMessage(tr("Load image from %1").arg(path));
    statusMessage(tr("Load image from %1").arg(path));

    img -> loadMetaData();

    m_IntegratedData.enqueue(QtConcurrent::run(this, &QxrdProcessor::integrateImage,
                                               img, mask(),
                                               cf -> get_Center().x(),
                                               cf -> get_Center().y()));
  } else {
    printMessage(tr("Couldn't load %1").arg(path));
    statusMessage(tr("Couldn't load %1").arg(path));
  }
}

void QxrdProcessor::processDataSequence(QString path, QString filter)
{
  QDirIterator iter(path, QStringList(filter));

  while (iter.hasNext()) {
    QString path = iter.next();

    printMessage(path);

    processData(path);
  }
}

void QxrdProcessor::processDataSequence(QStringList paths)
{
  QString path;

  foreach(path, paths) {
    printMessage(path);

    processData(path);
  }
}

void QxrdProcessor::processDataSequence(QString path, QStringList filters)
{
  QDirIterator iter(path, filters);

  while (iter.hasNext()) {
    QString path = iter.next();

    printMessage(path);

    processData(path);
  }
}

void QxrdProcessor::processNormalizedFile(QString path, double v1)
{
  QList<double> v;
  v << v1;

  processNormalizedFile(path, v);
}

void QxrdProcessor::processNormalizedFile(QString path, double v1, double v2)
{
  QList<double> v;
  v << v1 << v2;

  processNormalizedFile(path, v);
}

void QxrdProcessor::processNormalizedFile(QString name, QList<double> v)
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

void QxrdProcessor::setFileNormalization(QString path, double v1)
{
  QList<double> v;
  v << v1;

  setFileNormalization(path, v);
}

void QxrdProcessor::setFileNormalization(QString path, double v1, double v2)
{
  QList<double> v;
  v << v1 << v2;

  setFileNormalization(path, v);
}

void QxrdProcessor::setFileNormalization(QString name, QList<double> v)
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

void QxrdProcessor::processData(QString name)
{
  QcepDoubleImageDataPtr res = QcepAllocator::newDoubleImage("image", 0,0, QcepAllocator::NullIfNotAvailable);

  QString path = filePathInDataDirectory(name);

  if (res && res -> readImage(path)) {
    printMessage(tr("Load image from %1").arg(path));
    statusMessage(tr("Load image from %1").arg(path));

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();

    processDoubleImage(res, /*darkImage(), mask(),*/ QcepMaskDataPtr());

    set_DataPath(res -> get_FileName());
  } else {
    printMessage(tr("Couldn't load %1").arg(path));
    statusMessage(tr("Couldn't load %1").arg(path));
  }
}

void QxrdProcessor::processDoubleImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow)
{
  QcepDoubleImageDataPtr corrected =
      QcepAllocator::newDoubleImage("acquired", image->get_Width(), image->get_Height(), QcepAllocator::AlwaysAllocate);

  typedef QcepDoubleImageDataPtr (QxrdProcessor::*MFType)(QcepDoubleImageDataPtr, QcepDoubleImageDataPtr, QcepDoubleImageDataPtr, QcepMaskDataPtr, QcepMaskDataPtr);
  MFType p = &QxrdProcessor::correctDoubleImage;

  m_CorrectedImages.enqueue(QtConcurrent::run(this,
                                              p,
                                              corrected, image, dark(), mask(), overflow));
}

void QxrdProcessor::processDoubleImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, QList<double> v)
{
  QcepDoubleImageDataPtr corrected =
      QcepAllocator::newDoubleImage("acquired", image->get_Width(), image->get_Height(), QcepAllocator::AlwaysAllocate);

  typedef QcepDoubleImageDataPtr (QxrdProcessor::*MFType)(QcepDoubleImageDataPtr, QcepDoubleImageDataPtr, QcepDoubleImageDataPtr, QcepMaskDataPtr, QList<double>);
  MFType p = &QxrdProcessor::correctDoubleImage;

  m_CorrectedImages.enqueue(QtConcurrent::run(this,
                                              p,
                                              corrected, image, dark(), overflow, v));
}

QcepDoubleVector QxrdProcessor::doCalculateROICounts(QcepImageDataBasePtr img)
{
  QcepDoubleVector res;

  if (img && m_ROICalculator) {
    // TODO: check mask source...
    res = m_ROICalculator->values(img, mask());
  }

  if (qcepDebug(DEBUG_ACQUIRE)) {
    QString s = "[";

    for (int i=0; i<res.count(); i++) {
      if (i == 0) {
        s.append(tr("%1").arg(res.value(i)));
      } else {
        s.append(tr(", %1").arg(res.value(i)));
      }
    }

    s.append("]");

    printMessage(tr("ROI Values = %1").arg(s));
  }

  return res;
}

void QxrdProcessor::onCorrectedImageAvailable()
{
  QcepDoubleImageDataPtr img = m_CorrectedImages.dequeue();
  QcepMaskDataPtr mask = (img ? img->mask() : QcepMaskDataPtr());
  QxrdCenterFinderPtr cf(centerFinder());

  if (img && cf) {
    m_IntegratedData.enqueue(QtConcurrent::run(this, &QxrdProcessor::integrateImage,
                                               img, mask,
                                               cf -> get_Center().x(),
                                               cf -> get_Center().y()));

//    m_ROIData.enqueue(QtConcurrent::run(this, &QxrdProcessor::calculateROI,
//                                        img, mask));

    m_HistogramData.enqueue(QtConcurrent::run(this, &QxrdProcessor::calculateHistogram,
                                              img, mask));
  }
}

void QxrdProcessor::onIntegratedDataAvailable()
{
  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("QxrdProcessor::onIntegratedDataAvailable"));
  }

  QcepIntegratedDataPtr integ = m_IntegratedData.dequeue();

  if (integ) {
    writeOutputScan(integ);
    displayIntegratedData(integ);

    if (get_AccumulateIntegrated2D()) {
      integrator() -> appendIntegration(get_AccumulateIntegratedName(), integ);
    }
  }
}


void QxrdProcessor::writeOutputScan(QcepIntegratedDataPtr data)
{
  QxrdFileSaverPtr f(fileSaver());

  if (f) {
    if (this->get_SaveIntegratedData()) {
      QxrdExperimentPtr expt(experiment());

      if (expt) {
        expt->openScanFile();
        f->writeOutputScan(expt->scanFile(), data);
      }
    }

    if (this->get_SaveIntegratedInSeparateFiles()) {
      f->writeOutputScan(integratedOutputDirectory(), data);
    }
  }
}

QxrdHistogramDataPtr QxrdProcessor::calculateHistogram
    (QcepDoubleImageDataPtr /*image*/, QcepMaskDataPtr /*mask*/)
{
  QThread::currentThread()->setObjectName("calculateHistogram");

  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("QxrdProcessor::calculateHistogram"));
  }

  return QxrdHistogramDataPtr();
}

void QxrdProcessor::onHistogramDataAvailable()
{
  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("QxrdProcessor::onHistogramDataAvailable"));
  }

  QxrdHistogramDataPtr histData = m_HistogramData.dequeue();
}

//TODO: consider if this should be changed...
void QxrdProcessor::displayIntegratedData(QcepIntegratedDataPtr data)
{
  if (this->get_DisplayIntegratedData()) {
    emit newIntegrationAvailable(data);
  }
}

void QxrdProcessor::integrateSaveAndDisplay()
{
  QcepDoubleImageDataPtr dimg = qSharedPointerDynamicCast<QcepDoubleImageData>(data());
  QxrdCenterFinderPtr cf(centerFinder());

  if (dimg) {
    if (qcepDebug(DEBUG_INTEGRATOR)) {
      if (cf) {
        printMessage(tr("processor.integrateSaveAndDisplay: %1, %2, %3")
                     .arg(dimg->get_FileName())
                     .arg(cf->get_Center().x())
                     .arg(cf->get_Center().y()));
      } else {
        printMessage("QxrdProcessor::integrateSaveAndDisplay no center finder");
      }
    }

    QxrdIntegrator *integ = integrator().data();

    m_IntegratedData.enqueue(
          QtConcurrent::run(integ,
                            &QxrdIntegrator::performIntegration,
                            dimg, mask()));
  }
}

void QxrdProcessor::correlateImages(QStringList names)
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

void QxrdProcessor::shiftImage(int dx, int dy)
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

void QxrdProcessor::sumImages(QStringList names)
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

void QxrdProcessor::addImages(QStringList names)
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

void QxrdProcessor::subtractImages(QStringList names)
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

void QxrdProcessor::clearAccumulator()
{
  m_Integrator -> clearAccumulator(get_AccumulateIntegratedName());
}

void QxrdProcessor::integrateAndAccumulate(QStringList names)
{
  int nImages   = names.count();
  m_Integrator -> prepareAccumulator(get_AccumulateIntegratedName(), nImages);

  foreach(QString name, names) {
    QcepDoubleImageDataPtr img =
        QcepAllocator::newDoubleImage("image", 0,0, QcepAllocator::NullIfNotAvailable);
    QString path = filePathInDataDirectory(name);

    if (img && img->readImage(path)) {
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

void QxrdProcessor::saveAccumulator(QString &path, QString filter)
{
  m_Integrator -> saveAccumulator(get_AccumulateIntegratedName(), path, filter);
}

void QxrdProcessor::slicePolygon(QVector<QPointF> poly)
{
  QcepDoubleImageDataPtr dimg = qSharedPointerDynamicCast<QcepDoubleImageData>(data());
  QxrdIntegrator *integ = m_Integrator.data();

  if (dimg) {
    m_IntegratedData.enqueue(
          QtConcurrent::run(integ,
                            &QxrdIntegrator::slicePolygon,
                            dimg, poly, 0));
  }
}

void QxrdProcessor::measurePolygon(QVector<QPointF> poly)
{
  foreach(QPointF pt, poly) {
    printMessage(tr("Measure pt (%1,%2) = %3").arg(pt.x()).arg(pt.y())
                      .arg(m_Data -> getImageData(pt.x(),pt.y())));
  }

  summarizeMeasuredPolygon(poly);
}

void QxrdProcessor::printMeasuredPolygon(QVector<QPointF> poly)
{
  foreach(QPointF pt, poly) {
    printMessage(tr("Measure pt (%1,%2)").arg(pt.x()).arg(pt.y()));
  }

  summarizeMeasuredPolygon(poly);
}

void QxrdProcessor::summarizeMeasuredPolygon(QVector<QPointF> poly)
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

    statusMessage(tr("Angle: @ %1,%2, ang %3 deg").arg(x1).arg(y1).arg((a2-a1)/M_PI*180.0));
  } else if (poly.size() == 2) {
    double x0 = poly[0].x();
    double y0 = poly[0].y();
    double x1 = poly[1].x();
    double y1 = poly[1].y();
    double dx = x1-x0;
    double dy = y1-y0;
    double ang = atan2(dy,dx);
    double len = sqrt(dx*dx+dy*dy);

    statusMessage(tr("Line: %1,%2 - %3,%4 : D %5,%6 : L %7 : Ang %8").
                       arg(x0).arg(y0).arg(x1).arg(y1).
                       arg(dx).arg(dy).arg(len).arg(ang/M_PI*180.0));

  } else if (poly.size() == 1) {
    statusMessage(tr("Point: %1,%2").arg(poly[0].x()).arg(poly[0].y()));
  }
}

QStringList QxrdProcessor::integrateRectangle(int x0, int y0, int x1, int y1)
{
  double sum = 0;
  double npx = 0;

  QcepImageDataBasePtr   dat = m_Data;
  QcepMaskDataPtr        msk = mask();
  QStringList            res;

  if (dat) {
    if (msk) {
      for (int y=y0; y<y1; y++) {
        for (int x=x0; x<x1; x++) {
          if (msk->value(x,y)) {
            sum += dat->getImageData(x,y);
            npx += 1;
          }
        }
      }
    } else {
      for (int y=y0; y<y1; y++) {
        for (int x=x0; x<x1; x++) {
          sum += dat->getImageData(x,y);
          npx += 1;
        }
      }
    }

    printMessage(tr("integrateRectange(\"%1\",%2,%3,%4,%5)=[%6,%7]=%8")
                 .arg(dat->get_FileName())
                 .arg(x0).arg(y0).arg(x1).arg(y1).arg(sum).arg(npx).arg(sum/npx));

    if (npx > 0) {
      res << tr("%1").arg(sum/npx);
    } else {
      res << "0";
    }

    res << tr("%1").arg(sum);
    res << tr("%1").arg(npx);
    res << dat->get_FileName();
  }

  return res;
}

bool QxrdProcessor::integrateParameters()
{
  GUI_THREAD_CHECK;

  bool res = false;

  QxrdProcessorPtr proc(
        qSharedPointerDynamicCast<QxrdProcessor>(sharedFromThis()));

  if (proc) {
    QxrdIntegratorParmsDialog dlg(proc);

    if (dlg.exec() == QDialog::Accepted) {
      res = true;
    }
  }

  return res;
}

bool QxrdProcessor::polarTransformParameters()
{
  GUI_THREAD_CHECK;

  bool res = false;

  QxrdProcessorPtr proc(
        qSharedPointerDynamicCast<QxrdProcessor>(sharedFromThis()));

  if (proc) {
    QxrdPolarTransformDialog dlg(proc);

    if (dlg.exec() == QDialog::Accepted) {
      res = true;
    }
  }

  return res;
}

bool QxrdProcessor::polarIntegrateParameters()
{
  GUI_THREAD_CHECK;

  bool res = false;

  QxrdProcessorPtr proc(
        qSharedPointerDynamicCast<QxrdProcessor>(sharedFromThis()));


  if (proc) {
    QxrdPolarNormalizationDialog dlg(proc);

    if (dlg.exec() == QDialog::Accepted) {
      res = true;
    }
  }

  return res;
}

QcepDataObjectPtr QxrdProcessor::integrate(QcepDoubleImageDataPtr img)
{
  QcepDataObjectPtr res;
  QxrdIntegratorPtr integ = integrator();

  if (integ) {
    res = integ->performIntegration(img, mask());
  }

  return res;
}

QcepDataObjectPtr QxrdProcessor::polarTransform(QcepDoubleImageDataPtr img)
{
  QcepDataObjectPtr res;
  QxrdPolarTransformPtr xform = polarTransform();

  if (xform) {
    res = xform->transform(img, mask());
  }

  return res;
}

QcepDataObjectPtr QxrdProcessor::polarIntegrate(QcepDoubleImageDataPtr img)
{
  QcepDataObjectPtr res;
  QxrdPolarNormalizationPtr norm = polarNormalization();

  if (norm) {
    res = norm->transform(img);
  }

  return res;
}

void QxrdProcessor::reflectHorizontally()
{
  QcepDoubleImageDataPtr image = qSharedPointerDynamicCast<QcepDoubleImageData>(data());

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

  newData(image);
}

void QxrdProcessor::reflectVertically()
{
  QcepDoubleImageDataPtr image = qSharedPointerDynamicCast<QcepDoubleImageData>(data());

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

  newData(image);
}

void QxrdProcessor::projectImages(QStringList names, int px, int py, int pz)
{
  QcepDoubleImageDataPtr sumx, sumy, sumz;

  int nx = 0;
  int ny = 0;
  int nz = names.count();
  int first = true;

  if (px) {
    sumx = QcepAllocator::newDoubleImage("sumx", 0,0, QcepAllocator::NullIfNotAvailable);
    printMessage(tr("Projecting %1 images onto X").arg(nz));
  }

  if (py) {
    sumy = QcepAllocator::newDoubleImage("sumy", 0,0, QcepAllocator::NullIfNotAvailable);
    printMessage(tr("Projecting %1 images onto Y").arg(nz));
  }

  if (pz) {
    sumz = QcepAllocator::newDoubleImage("sumz", 0,0, QcepAllocator::NullIfNotAvailable);
    printMessage(tr("Projecting %1 images onto Z").arg(nz));
  }

  QxrdExperimentPtr expt(experiment());

  if (expt) {
    expt->commenceWork(nz);
  }

  for (int i=0; i<nz; i++) {
    QcepDoubleImageDataPtr img = QcepAllocator::newDoubleImage(tr("image-%1").arg(i), 0,0, QcepAllocator::NullIfNotAvailable);
    QString path = filePathInDataDirectory(names[i]);

    if (img && img->readImage(path)) {
      printMessage(tr("Load image from %1").arg(path));
      statusMessage(tr("Load image from %1").arg(path));

      img->loadMetaData();
      int typ = img->get_DataType();

      if ((typ == QcepDoubleImageData::Raw16Data) ||
          (typ == QcepDoubleImageData::Raw32Data))
      {
        subtractDarkImage(img, dark());
      }

      if (first) {
        nx = img->get_Width();
        ny = img->get_Height();

        if (px && sumx) {
          sumx->copyPropertiesFrom(img);
          sumx->resize(nz,ny);
          sumx->clear();
          sumx->set_SummedExposures(0);
        }

        if (py && sumy) {
          sumy->copyPropertiesFrom(img);
          sumy->resize(nz,nx);
          sumy->clear();
          sumy->set_SummedExposures(0);
        }

        if (pz && sumz) {
          sumz->copyPropertiesFrom(img);
          sumz->resize(nx,ny);
          sumz->clear();
          sumz->set_SummedExposures(0);
        }

        first = false;
      }

      if (px && sumx) {
        sumx->prop_SummedExposures()->incValue(1);

        for (int y=0; y<ny; y++) {
          double sum=0;

          for (int x=0; x<nx; x++) {
            sum += img->getImageData(x,y);
          }

          sumx->addValue(i,y, sum);
        }
      }

      if (py && sumy) {
        sumy->prop_SummedExposures()->incValue(1);

        for (int x=0; x<nx; x++) {
          double sum=0;

          for (int y=0; y<ny; y++) {
            sum += img->getImageData(x,y);
          }

          sumy->addValue(i,x, sum);
        }
      }

      if (pz && sumz) {
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

  if (px && sumx) {
    newData(sumx);
  }

  if (py && sumy) {
    newData(sumy);
  }

  if (pz && sumz) {
    newData(sumz);
  }

  if (expt) {
    expt->finishedWork(nz);
  }
}

void QxrdProcessor::fitPeakNear(double x, double y)
{
  QxrdCenterFinderPtr cf(centerFinder());

  if (cf) {
    cf->fitPeakNear(x,y);
  }
}

void QxrdProcessor::newImage(int ncols, int nrows)
{
  m_Data -> resize(ncols, nrows);
  m_Data -> fill(0);

  newData(m_Data);
}

void QxrdProcessor::exponentialTail(double cx, double cy, double width, int oversample)
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

void QxrdProcessor::reciprocalTail(double cx, double cy, double strength, int oversample)
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

void QxrdProcessor::powderRing(double cx, double cy, double radius, double width, double strength, int oversample)
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

void QxrdProcessor::ellipse(double cx, double cy, double a, double e, double ang, double width, double strength, int oversample)
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

