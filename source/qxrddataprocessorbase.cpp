#define _CRT_SECURE_NO_WARNINGS

#include "qxrddataprocessorbase.h"
#include "qxrddataprocessorthread.h"
#include <QtConcurrentRun>
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdimagedata.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrator.h"
#include "qxrdintegrateddata.h"
#include "qxrdmutexlocker.h"
#include "qxrdallocator.h"
#include "qxrdfilesaverthread.h"
#include "qxrdfilesaver.h"
#include "qxrdgeneratetestimage.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrdringsetsampleddata.h"
#include "qxrdringsetfitparameters.h"
#include "qxrdringfitparameters.h"

#include <QTime>
#include <QPainter>
#include <math.h>
#include <QDir>

QxrdDataProcessorBase::QxrdDataProcessorBase(
    QxrdSettingsSaverWPtr saver,
    QxrdExperimentWPtr doc,
    QxrdAcquisitionWPtr acq,
    QxrdAllocatorWPtr allocator,
    QxrdFileSaverWPtr filesaver) :

  QObject(NULL),
  m_ObjectNamer(this, "processor"),
  //    m_ProcessorType(this,"processorType",0),
  //    m_ProcessorTypeName(this,"processorTypeName","processorType"),
//  m_OutputDirectory(saver, this,"outputDirectory", ""),
  m_FileName(QxrdSettingsSaverPtr(), this, "fileName","", "Current File Name"),
  m_DataPath(saver, this,"dataPath", "", "Data Path"),
  m_DarkImagePath(saver, this, "darkImagePath", "", "Dark Images Path"),
  m_BadPixelsPath(saver, this, "badPixelsPath", "", "Bad Pixels Path"),
  m_GainMapPath(saver, this, "gainMapPath", "", "Gain Map Path"),
  m_MaskPath(saver, this, "maskPath", "", "Mask Path"),
  m_ScriptPath(saver, this, "scriptPath", "", "Script Path"),
  m_PerformDarkSubtraction(saver, this, "performDarkSubtraction", true, "Perform Dark Subtraction?"),
  m_SaveRawImages(saver, this, "saveRawImages", true, "Save Raw Images?"),
  m_SaveDarkImages(saver, this, "saveDarkImages", true, "Save Dark Images?"),
  m_PerformBadPixels(saver, this, "performBadPixels", true, "Perform Bad Pixel Correction?"),
  m_PerformGainCorrection(saver, this, "performGainCorrection", true, "Perform Gain Correction?"),
  m_SaveSubtracted(saver, this, "saveSubtracted", true, "Save Dark Subtracted Data?"),
  m_SaveAsText(saver, this, "saveAsText", false, "Save as Text Files (warning - Large and Slow!)"),
  m_SaveAsTextSeparator(saver, this, "saveAsTextSeparator", " ", "Separator for Images Saved as Text"),
  m_SaveAsTextPerLine(saver, this,"saveAsTextPerLine",16, "Pixels per line in Images Saved as Text"),
  m_SaveOverflowFiles(saver, this,"saveOverflowFiles",0, "Save Overflow Pixel Files?"),
  m_PerformIntegration(saver, this, "performIntegration", true, "Perform Circular Integration?"),
  m_DisplayIntegratedData(saver, this, "displayIntegratedData", true, "Display Integrated Data?"),
  m_SaveIntegratedData(saver, this, "saveIntegratedData", true, "Save Integrated Data?"),
  m_SaveIntegratedPath(saver, this, "saveIntegratedPath", "", "Integrated Data Path"),
  m_SaveDarkInSubdirectory(saver, this,"saveDarkInSubdirectory",0, "Save Dark In Subdirectory?"),
  m_SaveDarkSubdirectory(saver, this,"saveDarkSubdirectory","", "Dark Subdirectory"),
  m_SaveRawInSubdirectory(saver, this,"saveRawInSubdirectory",0, "Save Raw in Subdirectory?"),
  m_SaveRawSubdirectory(saver, this,"saveRawSubdirectory","", "Raw Subdirectory"),
  m_SaveSubtractedInSubdirectory(saver, this,"saveSubtractedInSubdirectory",0, "Save Subtracted in Subdirectory?"),
  m_SaveSubtractedSubdirectory(saver, this,"saveSubtractedSubdirectory","", "Subtracted Subdirectory"),
  m_SaveIntegratedInSeparateFiles(saver, this,"saveIntegratedInSeparateFiles",0, "Save Integrated in Separate Files?"),
  m_SaveIntegratedInSubdirectory(saver, this,"saveIntegratedInSubdirectory",0, "Save Integrated in Subdirectory?"),
  m_SaveIntegratedSubdirectory(saver, this,"saveIntegratedSubdirectory","", "Integrated Subdirectory"),
  m_PerformDarkSubtractionTime(saver, this, "performDarkSubtractionTime", 0.01, "Avg Time to Perform Dark Subtraction (in sec)"),
  m_PerformBadPixelsTime(saver, this, "performBadPixelsTime", 0.01, "Avg Time to Perform Bad Pixel Correction (in sec)"),
  m_PerformGainCorrectionTime(saver, this, "performGainCorrectionTime", 0.01, "Avg Time to Perform Gain Correction (in sec)"),
  m_SaveSubtractedTime(saver, this, "saveSubtractedTime", 0.1, "Avg Time to Save Subtracted Data (in sec)"),
  m_SaveAsTextTime(saver, this, "saveAsTextTime", 0.1, "Avg Time to Save Images as Text (in sec)"),
  m_PerformIntegrationTime(saver, this, "performIntegrationTime", 0.05, "Avg Time to Perform Integration (in sec/core)"),
  m_DisplayIntegratedDataTime(saver, this, "displayIntegratedDataTime", 0.2, "Avg Time to Display Integrated Data (in sec)"),
  m_SaveIntegratedDataTime(saver, this, "saveIntegratedDataTime", 0.01, "Avg Time to Save Integrated Data (in sec)"),
  m_EstimatedProcessingTime(saver, this, "estimatedProcessingTime", 0.1, "Overall Estimated Processing Time (in sec/image)"),
  m_AveragingRatio(saver, this, "averagingRatio", 0.1, "Averaging Ratio for Estimated Timing"),
  m_MaskMinimumValue(saver, this, "maskMinimumValue", 0, "Mask Minimum Value"),
  m_MaskMaximumValue(saver, this, "maskMaximumValue", 20000, "Mask Maximum Value"),
  m_MaskCircleRadius(saver, this, "maskCircleRadius", 10, "Mask Circle Radius"),
  m_MaskSetPixels(saver, this, "maskSetPixels", true, "Mask Set Pixels"),
//  m_CompressImages(saver, this, "compressImages", false, "Compress Images"),
  m_Average(QxrdSettingsSaverPtr(), this,"average",0.0, "Average Value of Acquired Image (per exposure)"),
  m_AverageDark(QxrdSettingsSaverPtr(), this,"averageDark",0.0, "Average Value of Dark Image"),
  m_AverageRaw(QxrdSettingsSaverPtr(), this,"averageRaw",0.0, "Average Value of Raw Image"),
  m_Mutex(QMutex::Recursive),
  m_Experiment(doc),
  m_Saver(saver),
  m_Window(),
  m_Allocator(allocator),
  m_FileSaver(filesaver),
  m_Acquisition(acq),
  m_AcquiredInt16Images("acquiredInt16Images"),
  m_AcquiredInt32Images("acquiredInt32Images"),
  m_Data(QxrdAllocator::newDoubleImage(allocator, QxrdAllocator::WaitTillAvailable, 2048, 2048)),
  m_DarkFrame(NULL),
  m_BadPixels(NULL),
  m_GainMap(NULL),
  //    m_Mask(allocator -> newMask()),
  m_AcquiredCount(0),
  m_CenterFinder(NULL),
  m_Integrator(NULL),
  m_InitialRingSetFitParameters(NULL),
  m_RefinedRingSetFitParameters(NULL),
  m_InitialRingSetData(NULL),
  m_RefinedRingSetData(NULL),
  m_GenerateTestImage(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDataProcessorBase::QxrdDataProcessorBase(%p)\n", this);
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdDataProcessorBase::QxrdDataProcessorBase");
  }

  m_CenterFinder = QxrdCenterFinderPtr(new QxrdCenterFinder(saver));
  m_Integrator   = QxrdIntegratorPtr(new QxrdIntegrator(saver, m_Experiment, m_CenterFinder, m_Allocator));
  m_GenerateTestImage = QxrdGenerateTestImagePtr(new QxrdGenerateTestImage(saver, m_Allocator));
  m_InitialRingSetFitParameters = QxrdRingSetFitParametersPtr(new QxrdRingSetFitParameters(saver));
  m_RefinedRingSetFitParameters = QxrdRingSetFitParametersPtr(new QxrdRingSetFitParameters(saver));
  m_InitialRingSetData = QxrdRingSetSampledDataPtr(new QxrdRingSetSampledData(saver));
  m_RefinedRingSetData = QxrdRingSetSampledDataPtr(new QxrdRingSetSampledData(saver));
}

QxrdDataProcessorBase::~QxrdDataProcessorBase()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDataProcessorBase::~QxrdDataProcessorBase(%p)\n", this);
  }
}

void QxrdDataProcessorBase::shutdown()
{
  thread()->exit();
}

void QxrdDataProcessorBase::setAcquisition(QxrdAcquisitionWPtr acq)
{
  m_Acquisition = acq;

  connect(prop_SaveRawImages(), SIGNAL(valueChanged(bool,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformDarkSubtraction(), SIGNAL(valueChanged(bool,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformBadPixels(), SIGNAL(valueChanged(bool,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformGainCorrection(), SIGNAL(valueChanged(bool,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveSubtracted(), SIGNAL(valueChanged(bool,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveAsText(), SIGNAL(valueChanged(bool,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformIntegration(), SIGNAL(valueChanged(bool,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_DisplayIntegratedData(), SIGNAL(valueChanged(bool,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveIntegratedData(), SIGNAL(valueChanged(bool,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformDarkSubtractionTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformBadPixelsTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformGainCorrectionTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveSubtractedTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveAsTextTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_PerformIntegrationTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_DisplayIntegratedDataTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateEstimatedProcessingTime()));
  connect(prop_SaveIntegratedDataTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateEstimatedProcessingTime()));

  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    connect(acqp -> prop_SummedExposures(), SIGNAL(valueChanged(int,int)), this, SLOT(updateEstimatedProcessingTime()));
    connect(acqp -> prop_Raw16SaveTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateEstimatedProcessingTime()));
    connect(acqp -> prop_Raw32SaveTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateEstimatedProcessingTime()));
  }
}

void QxrdDataProcessorBase::setWindow(QxrdWindow *win)
{
  m_Window = win;
  newData(m_Data, QxrdMaskDataPtr());
  newMask();
}

//QxrdSettingsSaver *QxrdDataProcessorBase::saver()
//{
//  return m_Saver;
//}

void QxrdDataProcessorBase::writeSettings(QSettings *settings, QString section)
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

void QxrdDataProcessorBase::readSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  m_CenterFinder -> readSettings(settings, section+"/centerfinder");
  m_Integrator   -> readSettings(settings, section+"/integrator");
  m_InitialRingSetFitParameters -> readSettings(settings, section+"/initialFit");
  m_RefinedRingSetFitParameters -> readSettings(settings, section+"/refinedFit");
  m_InitialRingSetData -> readSettings(settings, section+"/initialData");
  m_RefinedRingSetData -> readSettings(settings, section+"/refinedData");
}

void QxrdDataProcessorBase::printMessage(QString msg, QDateTime ts) const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->printMessage(msg, ts);
  }
}

void QxrdDataProcessorBase::criticalMessage(QString msg, QDateTime ts) const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->criticalMessage(msg);
  }
}

void QxrdDataProcessorBase::statusMessage(QString msg, QDateTime ts) const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->statusMessage(msg);
  }
}

QString QxrdDataProcessorBase::existingOutputDirectory(QString dir, QString subdir) const
{
  return QDir(dir).filePath(subdir);
}

QString QxrdDataProcessorBase::filePathInCurrentDirectory(QString name) const
{
  return QDir(currentDirectory()).filePath(name);
}

QString QxrdDataProcessorBase::currentDirectory() const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    return exp->get_ExperimentDirectory();
  } else {
    return QString();
  }
}

QString QxrdDataProcessorBase::darkOutputDirectory() const
{
  if (get_SaveDarkInSubdirectory()) {
    return existingOutputDirectory(currentDirectory(), get_SaveDarkSubdirectory());
  } else {
    return currentDirectory();
  }
}

QString QxrdDataProcessorBase::rawOutputDirectory() const
{
  if (get_SaveRawInSubdirectory()) {
    return existingOutputDirectory(currentDirectory(), get_SaveRawSubdirectory());
  } else {
    return currentDirectory();
  }
}

QString QxrdDataProcessorBase::subtractedOutputDirectory() const
{
  if (get_SaveSubtractedInSubdirectory()) {
    return existingOutputDirectory(currentDirectory(), get_SaveSubtractedSubdirectory());
  } else {
    return currentDirectory();
  }
}

QString QxrdDataProcessorBase::integratedOutputDirectory() const
{
  if (get_SaveIntegratedInSubdirectory()) {
    return existingOutputDirectory(currentDirectory(), get_SaveIntegratedSubdirectory());
  } else {
    return currentDirectory();
  }
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::takeNextFreeImage(int width, int height)
{
  QxrdDoubleImageDataPtr res = QxrdAllocator::newDoubleImage(m_Allocator,
                                                             QxrdAllocator::AlwaysAllocate,
                                                             width, height);

  return res;
}

void QxrdDataProcessorBase::newData(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{
  m_Data = image;
  m_Overflow = overflow;

  QxrdWindow *w = m_Window;

  if (w) {
    w -> newDataAvailable(m_Data, overflow);
  }
}

void QxrdDataProcessorBase::newDarkImage(QxrdDoubleImageDataPtr image)
{
  m_DarkFrame = image;

  if (image) {
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
  } else {
    set_DarkImagePath("");
    set_AverageDark(0);
  }
}

void QxrdDataProcessorBase::newDarkImage(QxrdInt16ImageDataPtr image)
{
  if (image) {
    if (m_DarkFrame == NULL) {
      m_DarkFrame = takeNextFreeImage(image->get_Width(), image->get_Height());
    }

    m_DarkFrame -> copyFrom(image);
    newDarkImage(m_DarkFrame);

    // set_DarkImagePath(m_DarkFrame -> get_FileName());
  } else {
    newDarkImage(QxrdDoubleImageDataPtr(NULL));
  }
}

void QxrdDataProcessorBase::newDarkImage(QxrdInt32ImageDataPtr image)
{
  if (image) {
    if (m_DarkFrame == NULL) {
      m_DarkFrame = takeNextFreeImage(image->get_Width(), image->get_Height());
    }

    m_DarkFrame -> copyFrom(image);
    newDarkImage(m_DarkFrame);

    //  set_DarkImagePath(m_DarkFrame -> get_FileName());
  } else {
    newDarkImage(QxrdDoubleImageDataPtr(NULL));
  }
}

void QxrdDataProcessorBase::newBadPixelsImage(QxrdDoubleImageDataPtr image)
{
  m_BadPixels = image;

  if (image) {
    set_BadPixelsPath(image->get_FileName());
  } else {
    set_BadPixelsPath("");
  }
}

void QxrdDataProcessorBase::newGainMapImage(QxrdDoubleImageDataPtr image)
{
  m_GainMap = image;

  if (image) {
    set_GainMapPath(image->get_FileName());
  } else {
    set_GainMapPath("");
  }
}

void QxrdDataProcessorBase::newMask()
{
  QxrdWindow *w = m_Window;

  if (w) {
    w -> newMaskAvailable(mask());
  }
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

QString QxrdDataProcessorBase::pwd() const
{
  return currentDirectory();
}

//void QxrdDataProcessorBase::cd(QString path)
//{
//  QDir dir(currentDirectory());

//  if (dir.cd(path)) {
//    set_OutputDirectory(dir.path());
//  }
//}

QStringList QxrdDataProcessorBase::ls() const
{
  QStringList res;
  QDir dir(currentDirectory());

  res = dir.entryList(QStringList());

  return res;
}

QStringList QxrdDataProcessorBase::ls(QString pattern) const
{
  QStringList res;
  QDir dir(currentDirectory());

  res = dir.entryList(QStringList(pattern));

  return res;
}

void QxrdDataProcessorBase::loadData(QString name)
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "loadData", Qt::QueuedConnection, Q_ARG(QString, name)))
  } else {
    if (qcepDebug(DEBUG_FILES)) {
      printMessage(
            tr("QxrdDataProcessorBase::loadData(%1)").arg(name));
    }

    QxrdDoubleImageDataPtr res = takeNextFreeImage(0,0);

    QString path = filePathInCurrentDirectory(name);

    if (res && res -> readImage(path)) {

      //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

      res -> loadMetaData();

      newData(res, QxrdMaskDataPtr());

      set_DataPath(res -> get_FileName());
    }
  }
}

void QxrdDataProcessorBase::saveData(QString name, int canOverwrite)
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveData", Qt::QueuedConnection, Q_ARG(QString, name), Q_ARG(int, canOverwrite)))
  } else {
    QString path = filePathInCurrentDirectory(name);

    saveNamedImageData(path, m_Data, QxrdMaskDataPtr(), canOverwrite);

    set_DataPath(m_Data -> get_FileName());
  }
}

void QxrdDataProcessorBase::loadDark(QString name)
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "loadDark", Qt::QueuedConnection, Q_ARG(QString, name)))
  } else {
    if (qcepDebug(DEBUG_FILES)) {
      printMessage(
            tr("QxrdDataProcessorBase::loadDark(%1)").arg(name));
    }

    QxrdDoubleImageDataPtr res = takeNextFreeImage(0,0);

    QString path = filePathInCurrentDirectory(name);

    if (res && res -> readImage(path)) {

      //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

      res -> loadMetaData();
      res -> set_DataType(QxrdDoubleImageData::DarkData);

      newDarkImage(res);

      set_DarkImagePath(res -> get_FileName());
    } else {
      printMessage(tr("loadDark(%1) failed").arg(name));
    }
  }
}

void QxrdDataProcessorBase::saveDark(QString name, int canOverwrite)
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveDark", Qt::QueuedConnection, Q_ARG(QString, name), Q_ARG(int, canOverwrite)))
  } else {
    QString path = filePathInCurrentDirectory(name);

    if (m_DarkFrame) {
      saveNamedImageData(path, m_DarkFrame, QxrdMaskDataPtr(), canOverwrite);

      set_DarkImagePath(m_DarkFrame -> get_FileName());
    }
  }
}

void QxrdDataProcessorBase::loadBadPixels(QString name)
{
  if (qcepDebug(DEBUG_FILES)) {
    printMessage(tr("QxrdDataProcessorBase::loadBadPixels(%1)").arg(name));
  }

  QxrdDoubleImageDataPtr res = takeNextFreeImage(0,0);

  QString path = filePathInCurrentDirectory(name);

  if (res && res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QxrdDoubleImageData::BadPixelsData);

    newBadPixelsImage(res);

    set_BadPixelsPath(res -> get_FileName());
  }
}

void QxrdDataProcessorBase::saveBadPixels(QString name, int canOverwrite)
{
  QString path = filePathInCurrentDirectory(name);

  if (m_BadPixels) {
    saveNamedImageData(path, m_BadPixels, QxrdMaskDataPtr(), canOverwrite);

    set_BadPixelsPath(m_BadPixels -> get_FileName());
  }
}

void QxrdDataProcessorBase::loadGainMap(QString name)
{
  if (qcepDebug(DEBUG_FILES)) {
    printMessage(tr("QxrdDataProcessorBase::loadGainMap(%1)").arg(name));
  }

  QxrdDoubleImageDataPtr res = takeNextFreeImage(0,0);

  QString path = filePathInCurrentDirectory(name);

  if (res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QxrdDoubleImageData::GainData);
    res -> setDefaultValue(1.0);

    newGainMapImage(res);

    set_GainMapPath(res -> get_FileName());
  }
}

void QxrdDataProcessorBase::saveGainMap(QString name, int canOverwrite)
{
  QString path = filePathInCurrentDirectory(name);

  if (m_GainMap) {
    saveNamedImageData(path, m_GainMap, QxrdMaskDataPtr(), canOverwrite);

    set_GainMapPath(m_GainMap -> get_FileName());
  }
}

QxrdMaskStackPtr QxrdDataProcessorBase::maskStack()
{
  return &m_Masks;
}

int QxrdDataProcessorBase::maskStackSize()
{
  return m_Masks.count();
}

int QxrdDataProcessorBase::maskStackPosition(int pos)
{
  int len = m_Masks.count();

  if (pos >= 0 && pos < len) {
    return pos;
  } else if (pos < 0 && pos >= -len) {
    return len-pos;
  } else {
    return -1;
  }
}

int QxrdDataProcessorBase::newMaskWidth() const
{
  QxrdDoubleImageDataPtr d = data();

  int w=0;

  if (d) {
    w = d->get_Width();
  } else if (mask()) {
    w = mask()->get_Width();
  }

  return w;
}

int QxrdDataProcessorBase::newMaskHeight() const
{
  QxrdDoubleImageDataPtr d = data();

  int h=0;

  if (d) {
    h = d->get_Height();
  } else if (mask()) {
    h = mask()->get_Height();
  }

  return h;
}

void QxrdDataProcessorBase::newMaskStack()
{

  QxrdMaskDataPtr m = QxrdAllocator::newMask(m_Allocator,
                                             QxrdAllocator::WaitTillAvailable,
                                             newMaskWidth(), newMaskHeight());

  m_Masks.push_front(m);

  printMessage(tr("new mask, %1 on stack").arg(m_Masks.count()));

  m_Masks.changed();

  newMask();
}

void QxrdDataProcessorBase::pushMaskStack(QxrdMaskDataPtr m)
{
  if (m == NULL) {
    m =  QxrdAllocator::newMask(m_Allocator,
                                QxrdAllocator::WaitTillAvailable,
                                newMaskWidth(), newMaskHeight());

    if (mask()) {
      mask()->copyMaskTo(m);
    }
  }

  m_Masks.push_front(m);

//  m_Mask = mask;

  printMessage(tr("dup mask, %1 on stack").arg(m_Masks.count()));

  m_Masks.changed();

  newMask();
}

void QxrdDataProcessorBase::popMaskStack(int amount)
{
  if (amount == 0) {
    return;
  } else if (amount > 1) {
    for (int i=0; i<amount; i++) {
      popMaskStack(1);
    }
  } else if (amount < -1) {
    for (int i=0; i<(-amount); i++) {
      popMaskStack(-1);
    }
  } else if (amount == 1) {
    if (m_Masks.count() >= 1) {
//      m_Mask = m_Masks.first();
      m_Masks.pop_front();
    }
  } else if (amount == -1) {
    if (m_Masks.count() >= 1) {
//      m_Mask = m_Masks.last();
      m_Masks.pop_back();
    }
  }

  m_Masks.changed();

  newMask();
}

void QxrdDataProcessorBase::clearMaskStack()
{
  m_Masks.clear();

  m_Masks.changed();

  newMask();
}

void QxrdDataProcessorBase::clearMaskStackTop()
{
  popMaskStack();
}

void QxrdDataProcessorBase::rollMaskStack(int amount)
{
  if (amount == 0) {
    return;
  } else if (amount > 1) {
    for (int i=0; i<amount; i++) {
      rollMaskStack(1);
    }
  } else if (amount < -1) {
    for (int i=0; i<(-amount); i++) {
      rollMaskStack(-1);
    }
  } else if (amount == 1) {
    QxrdMaskDataPtr m = m_Masks.first();
    m_Masks.push_back(m);
    m_Masks.pop_front();
  } else if (amount == -1) {
    QxrdMaskDataPtr m = m_Masks.last();
    m_Masks.push_front(m);
    m_Masks.pop_back();
  }

  m_Masks.changed();

  newMask();
}

void QxrdDataProcessorBase::exchangeMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  if ((p0 >= 0) && (p1 >= 0) && (p0 != p1)) {
    QxrdMaskDataPtr pm=m_Masks[p0];
    m_Masks[p0] = m_Masks[p1];
    m_Masks[p1] = pm;

    m_Masks.changed();

    newMask();
  }
}

void QxrdDataProcessorBase::andMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  if ((p0 >= 0) && (p1 >= 0)) {
    QxrdMaskDataPtr pm = m_Masks[p1];

    m_Masks[p0] -> andMask(pm);

    m_Masks.changed();

    newMask();
  }
}

void QxrdDataProcessorBase::orMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  if ((p0 >= 0) && (p1 >= 0)) {
    QxrdMaskDataPtr pm = m_Masks[p1];

    m_Masks[p0] -> orMask(pm);

    m_Masks.changed();

    newMask();
  }
}

void QxrdDataProcessorBase::xorMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  if ((p0 >= 0) && (p1 >= 0)) {
    QxrdMaskDataPtr pm = m_Masks[p1];

    m_Masks[p0] -> xorMask(pm);

    m_Masks.changed();

    newMask();
  }
}

void QxrdDataProcessorBase::andNotMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  if ((p0 >= 0) && (p1 >= 0)) {
    QxrdMaskDataPtr pm = m_Masks[p1];

    m_Masks[p0] -> andNotMask(pm);

    m_Masks.changed();

    newMask();
  }
}

void QxrdDataProcessorBase::orNotMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  if ((p0 >= 0) && (p1 >= 0)) {
    QxrdMaskDataPtr pm = m_Masks[p1];

    m_Masks[p0] -> orNotMask(pm);

    m_Masks.changed();

    newMask();
  }
}

void QxrdDataProcessorBase::xorNotMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  if ((p0 >= 0) && (p1 >= 0)) {
    QxrdMaskDataPtr pm = m_Masks[p1];

    m_Masks[p0] -> xorNotMask(pm);

    m_Masks.changed();

    newMask();
  }
}

void QxrdDataProcessorBase::invertMaskStack(int pos)
{
  int p = maskStackPosition(pos);

  if (p >= 0) {
    m_Masks[p] -> invertMask();

    m_Masks.changed();

    newMask();
  }
}

void QxrdDataProcessorBase::hideMaskAllStack(int pos)
{
  int p = maskStackPosition(pos);

  if (p >= 0) {
    m_Masks[p] -> hideMaskAll();

    m_Masks.changed();

    newMask();
  }
}

void QxrdDataProcessorBase::showMaskAllStack(int pos)
{
  int p = maskStackPosition(pos);

  if (p >= 0) {
    m_Masks[p] -> showMaskAll();

    m_Masks.changed();

    newMask();
  }
}

void QxrdDataProcessorBase::hideMaskRangeStack(int pos)
{
  int p = maskStackPosition(pos);

  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  if (m_Data && p >= 0) {
    m_Masks[p] -> hideMaskRange(QSharedPointer< QcepImageData<double> >(m_Data), min, max);

    m_Masks.changed();

    newMask();
  }
}

void QxrdDataProcessorBase::showMaskRangeStack(int pos)
{
  int p = maskStackPosition(pos);

  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  if (m_Data && p >= 0) {
    m_Masks[p] -> showMaskRange(QSharedPointer< QcepImageData<double> >(m_Data), min, max);

    m_Masks.changed();

    newMask();
  }
}

void QxrdDataProcessorBase::loadMask(QString name)
{
  if (qcepDebug(DEBUG_FILES)) {
    printMessage(tr("QxrdDataProcessorBase::loadMask(%1)").arg(name));
  }

  QxrdMaskDataPtr res = QxrdAllocator::newMask(m_Allocator, QxrdAllocator::WaitTillAvailable, 0,0);

  QString path = filePathInCurrentDirectory(name);

  if (res -> readImage(path)) {

    //  printf("Read %d x %d image\n", res->get_Width(), res->get_Height());

    res -> loadMetaData();
    res -> set_DataType(QxrdMaskData::MaskData);

    //    res -> copyMaskTo(m_Mask);

    pushMaskStack(res);

    m_Masks.changed();

    newMask();

    set_MaskPath(mask() -> get_FileName());
  }
}

void QxrdDataProcessorBase::saveMask(QString name, int canOverwrite)
{
  QString path = filePathInCurrentDirectory(name);

  if (mask()) {
    saveNamedMaskData(path, mask(), canOverwrite);

    set_MaskPath(mask() -> get_FileName());
  }
}

void QxrdDataProcessorBase::clearMask()
{
  clearMaskStack();

  set_MaskPath("");
}

void QxrdDataProcessorBase::saveNamedImageData(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(m_FileSaver);

  if (f) {
    f -> saveData(name, image, overflow, canOverwrite);
  }
}

void QxrdDataProcessorBase::saveNamedImageData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(m_FileSaver);

  if (f) {
    f -> saveData(name, image, overflow, canOverwrite);
  }
}

void QxrdDataProcessorBase::saveNamedRawImageData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(m_FileSaver);

  if (f) {
    f -> saveRawData(name, image, overflow, canOverwrite);
  }
}

void QxrdDataProcessorBase::saveNamedImageData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(m_FileSaver);

  if (f) {
    f -> saveData(name, image, overflow, canOverwrite);
  }
}

void QxrdDataProcessorBase::saveNamedRawImageData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(m_FileSaver);

  if (f) {
    f -> saveRawData(name, image, overflow, canOverwrite);
  }
}

void QxrdDataProcessorBase::saveNamedMaskData(QString name, QxrdMaskDataPtr image, int canOverwrite)
{
  QxrdFileSaverPtr f(m_FileSaver);

  if (f) {
    f -> saveData(name, image, canOverwrite);
  }
}

void QxrdDataProcessorBase::saveNamedImageDataAsText(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  QxrdFileSaverPtr f(m_FileSaver);

  if (f) {
    f -> saveTextData(name, image, overflow, canOverwrite);
  }
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

QxrdDoubleImageDataPtr QxrdDataProcessorBase::processAcquiredInt16Image(
    QxrdDoubleImageDataPtr corrected,
    QxrdInt16ImageDataPtr img,
    QxrdDoubleImageDataPtr dark,
    QxrdMaskDataPtr mask,
    QxrdMaskDataPtr overflow)
{
  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("processing acquired 16 bit image, %1 remaining")
                 .arg(getAcquiredCount()));
  }

  if (img) {
    if (get_SaveRawImages()) {
      if (img->get_ImageSaved()) {
        printMessage(tr("Image \"%1\" is already saved").arg(img->rawFileName()));
      } else {
        saveNamedRawImageData(img->rawFileName(), img, overflow, QxrdDataProcessorBase::NoOverwrite);
      }
    }

    corrected -> copyFrom(img);
    corrected -> set_DateTime(QDateTime::currentDateTime());

    processAcquiredImage(corrected, corrected, dark, mask, overflow);

    return corrected;
  } else {
    return QxrdDoubleImageDataPtr();
  }
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::processAcquiredInt32Image(
    QxrdDoubleImageDataPtr corrected,
    QxrdInt32ImageDataPtr img,
    QxrdDoubleImageDataPtr dark,
    QxrdMaskDataPtr mask,
    QxrdMaskDataPtr overflow)
{
  if (qcepDebug(DEBUG_PROCESS)) {
    printMessage(tr("processing acquired 32 bit image, %1 remaining")
                 .arg(getAcquiredCount()));
  }

  if (img) {
    if (get_SaveRawImages()) {
      if (img->get_ImageSaved()) {
        printMessage(tr("Image \"%1\" is already saved").arg(img->rawFileName()));
      } else {
        saveNamedRawImageData(img->rawFileName(), img, overflow, QxrdDataProcessorBase::NoOverwrite);
      }
    }

    corrected -> copyFrom(img);
    corrected -> set_DateTime(QDateTime::currentDateTime());

    processAcquiredImage(corrected, corrected, dark, mask, overflow);

    return corrected;
  } else {
    return QxrdDoubleImageDataPtr();
  }
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::processAcquiredDoubleImage(
    QxrdDoubleImageDataPtr processed,
    QxrdDoubleImageDataPtr dimg,
    QxrdDoubleImageDataPtr dark,
    QxrdMaskDataPtr mask,
    QxrdMaskDataPtr overflow)
{
  return processAcquiredImage(processed, dimg, dark, mask, overflow);
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::processAcquiredDoubleImage(
    QxrdDoubleImageDataPtr processed,
    QxrdDoubleImageDataPtr dimg,
    QxrdDoubleImageDataPtr dark,
    QxrdMaskDataPtr mask,
    QxrdMaskDataPtr overflow,
    QcepDoubleList v)
{
  return processAcquiredImage(processed, dimg, dark, mask, overflow, v);
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::processAcquiredImage(
    QxrdDoubleImageDataPtr processed,
    QxrdDoubleImageDataPtr img,
    QxrdDoubleImageDataPtr dark,
    QxrdMaskDataPtr /*mask*/,
    QxrdMaskDataPtr overflow,
    QcepDoubleList v)
{
  if (processed && img) {
    processed->copyFrom(img);

    statusMessage(tr("Processing Image \"%1\"").arg(processed->get_FileName()));

    QTime tic;
    tic.start();

//    processed->set_Normalization(v);

    if (qcepDebug(DEBUG_PROCESS)) {
      printMessage(tr("Processing Image \"%1\", count %2")
                   .arg(processed->get_FileName()).arg(getAcquiredCount()));
    }

    if (get_PerformDarkSubtraction()) {
      subtractDarkImage(processed, dark);
      processed -> set_ImageSaved(false);

      int subTime = tic.restart();

      updateEstimatedTime(prop_PerformDarkSubtractionTime(), subTime);

      if (qcepDebug(DEBUG_PROCESS)) {
        printMessage(tr("Dark subtraction took %1 msec").arg(subTime));
      }
    }

    if (get_PerformBadPixels()) {
      correctBadPixels(processed);
      processed -> set_ImageSaved(false);

      int badPxlTime = tic.restart();

      updateEstimatedTime(prop_PerformBadPixelsTime(), badPxlTime);

      if (qcepDebug(DEBUG_PROCESS)) {
        printMessage(tr("Bad Pixel correction took %1 msec").arg(badPxlTime));
      }
    }

    if (get_PerformGainCorrection()) {
      correctImageGains(processed);
      processed -> set_ImageSaved(false);

      int gainTime = tic.restart();

      updateEstimatedTime(prop_PerformGainCorrectionTime(), gainTime);

      if (qcepDebug(DEBUG_PROCESS)) {
        printMessage(tr("Gain correction took %1 msec").arg(gainTime));
      }
    }

    if (get_SaveSubtracted()) {
      if (processed->get_ImageSaved()) {
        printMessage(tr("Image \"%1\" is already saved").arg(processed->rawFileName()));
      } else {
        saveNamedImageData(QDir(subtractedOutputDirectory()).filePath(processed->get_FileBase()), processed, overflow);
      }
    }

    if (get_SaveAsText()) {
      saveNamedImageDataAsText(processed->get_FileName(), processed, overflow);

      updateEstimatedTime(prop_SaveAsTextTime(), tic.elapsed());
    }

    newData(processed, overflow);

    if (qcepDebug(DEBUG_PROCESS)) {
      printMessage(tr("Processing took %1 msec").arg(tic.restart()));
    }

    statusMessage(tr("Completed Processing Image \"%1\"").arg(processed->get_FileName()));
  }

  return processed;
}

void QxrdDataProcessorBase::updateEstimatedTime(QcepDoubleProperty *prop, int msec)
{
  double newVal = prop -> value() * (1.0 - get_AveragingRatio()) + ((double) msec)/1000.0* get_AveragingRatio();

  prop -> setValue(newVal);
}

void QxrdDataProcessorBase::subtractDarkImage(QxrdDoubleImageDataPtr image, QxrdDoubleImageDataPtr dark)
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

    if (!(image->get_DataType() == QxrdDoubleImageData::Raw16Data ||
          image->get_DataType() == QxrdDoubleImageData::Raw32Data)) {
      printMessage("Acquired data is not a raw image, skipping background subtraction");
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
  //  }
}

void QxrdDataProcessorBase::unsubtractDarkImage(QxrdDoubleImageDataPtr image, QxrdDoubleImageDataPtr dark)
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

    if (!(image->get_DataType() == QxrdDoubleImageData::Raw16Data ||
          image->get_DataType() == QxrdDoubleImageData::Raw32Data)) {
      printMessage("Acquired data is not a raw image, skipping background subtraction");
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
      result[i] = result[i]+ratio*dk[i];
    }

    //      set_AverageDark(avgdark/npixels/ndrk);
    set_AverageRaw(avgraw/npixels/nres);
    set_Average(get_AverageRaw() - get_AverageDark());

    image -> set_DataType(QxrdDoubleImageData::SubtractedData);
  }
  //  }
}

void QxrdDataProcessorBase::correctBadPixels(QxrdDoubleImageDataPtr /*image*/)
{
}

void QxrdDataProcessorBase::correctImageGains(QxrdDoubleImageDataPtr image)
{
  if (image) {
    QxrdDoubleImageDataPtr gains = gainMap();

    if (gains) {
      image -> multiply(gains);
    }
  }
}

void QxrdDataProcessorBase::updateEstimatedProcessingTime()
{
  double estSerialTime = 0, estParallelTime = 0;

  QxrdAcquisitionPtr acq(m_Acquisition);

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

double QxrdDataProcessorBase::estimatedProcessingTime(double estSerialTime, double estParallelTime)
{
  return estSerialTime + estParallelTime;
}

void QxrdDataProcessorBase::showMaskRange(/*double min, double max*/)
{
  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  if (m_Data && mask()) {
    mask() -> showMaskRange(QSharedPointer< QcepImageData<double> >(m_Data), min, max);

    newMask();
  }
}

void QxrdDataProcessorBase::hideMaskAll()
{
  if (mask()) {
    mask() -> hideMaskAll();

    newMask();
  }
}

void QxrdDataProcessorBase::showMaskAll()
{
  if (mask()) {
    mask() -> showMaskAll();

    newMask();
  }
}

void QxrdDataProcessorBase::hideMaskRange(/*double min, double max*/)
{
  double min = get_MaskMinimumValue();
  double max = get_MaskMaximumValue();

  if (m_Data && mask()) {
    mask() -> hideMaskRange(QSharedPointer< QcepImageData<double> >(m_Data), min, max);

    newMask();
  }
}

void QxrdDataProcessorBase::invertMask()
{
  if (mask()) {
    mask() -> invertMask();

    newMask();
  }
}

void QxrdDataProcessorBase::maskCircle(QwtDoubleRect rect)
{ 
  if (mask()) {
    if ((rect.left() == rect.right()) && (rect.bottom() == rect.top())) {
      mask() -> maskCircle(rect.left(), rect.top(), get_MaskCircleRadius(), get_MaskSetPixels());
    } else {
      double cx = rect.center().x();
      double cy = rect.center().y();
      double rad = rect.width()/2;

      mask() -> maskCircle(cx, cy, rad, get_MaskSetPixels());
    }

    newMask();
  }
}

void QxrdDataProcessorBase::maskPolygon(QVector<QwtDoublePoint> poly)
{
  if (mask()) {
    //  printf("QxrdDataProcessorBase::maskPolygon(%d points ...)\n", poly.size());

    int nRows = mask() -> get_Height();
    int nCols = mask() -> get_Width();

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
          mask() -> setMaskValue(i, j, newval);
        }
      }
    }

    newMask();
  }
}

void QxrdDataProcessorBase::measurePolygon(QVector<QwtDoublePoint> poly)
{
  foreach(QwtDoublePoint pt, poly) {
    printMessage(tr("Measure pt (%1,%2) = %3").arg(pt.x()).arg(pt.y())
                      .arg(m_Data -> value(pt.x(),pt.y())));
  }

  summarizeMeasuredPolygon(poly);
}

void QxrdDataProcessorBase::printMeasuredPolygon(QVector<QwtDoublePoint> poly)
{
  foreach(QwtDoublePoint pt, poly) {
    printMessage(tr("Measure pt (%1,%2)").arg(pt.x()).arg(pt.y()));
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

QxrdDoubleImageDataPtr QxrdDataProcessorBase::data() const
{
  return m_Data;
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::darkImage() const
{
  return m_DarkFrame;
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::gainMap() const
{
  return m_GainMap;
}

QxrdDoubleImageDataPtr QxrdDataProcessorBase::badPixels() const
{
  return m_BadPixels;
}

QxrdMaskDataPtr QxrdDataProcessorBase::mask() const
{
  if (m_Masks.isEmpty()) {
    return QxrdMaskDataPtr();
  } else {
    return m_Masks.first();
  }
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
    printMessage("Problem QxrdDataProcessorBase::centerFinder == NULL");
  }

  return m_CenterFinder;
}

QxrdIntegratorPtr QxrdDataProcessorBase::integrator() const
{
  if (m_Integrator == NULL) {
    printMessage("Problem QxrdDataProcessorBase::integrator == NULL");
  }

  return m_Integrator;
}

QxrdRingSetFitParametersPtr QxrdDataProcessorBase::initialRingSetFitParameters() const
{
  if (m_InitialRingSetFitParameters == NULL) {
    printMessage("Problem QxrdDataProcessorBase::initialRingSetFitParameters == NULL");
  }

  return m_InitialRingSetFitParameters;
}

QxrdRingSetSampledDataPtr QxrdDataProcessorBase::initialRingSetData() const
{
  if (m_InitialRingSetData == NULL) {
    printMessage("Problem QxrdDataProcessorBase::initialRingSetData == NULL");
  }

  return m_InitialRingSetData;
}

QxrdRingSetFitParametersPtr QxrdDataProcessorBase::refinedRingSetFitParameters() const
{
  if (m_RefinedRingSetFitParameters == NULL) {
    printMessage("Problem QxrdDataProcessorBase::refinedRingSetFitParameters == NULL");
  }

  return m_RefinedRingSetFitParameters;
}

QxrdRingSetSampledDataPtr QxrdDataProcessorBase::refinedRingSetData() const
{
  if (m_RefinedRingSetData == NULL) {
    printMessage("Problem QxrdDataProcessorBase::refinedRingSetData == NULL");
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

void QxrdDataProcessorBase::writeOutputScan(QxrdIntegratedDataPtr data)
{
  QxrdFileSaverPtr f(m_FileSaver);

  if (f) {
    if (this->get_SaveIntegratedData()) {
      QxrdExperimentPtr expt(m_Experiment);

      if (expt) {
        f->writeOutputScan(expt->scanFile(), data);
      }
    }

    if (this->get_SaveIntegratedInSeparateFiles()) {
      f->writeOutputScan(integratedOutputDirectory(), data);
    }
  }
}

void QxrdDataProcessorBase::displayIntegratedData(QxrdIntegratedDataPtr data)
{
  if (this->get_DisplayIntegratedData()) {
    emit newIntegrationAvailable(data);
  }
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
      printMessage(tr("file %1 written in %2 msec").arg(fileName).arg(dt));
    }
  }

  printMessage(tr("average write speed %1 MB/sec")
                    .arg(((double) sz)*40.0*1000.0/(1e6*((double) totalt))));
  delete [] buff;
}

void QxrdDataProcessorBase::calculateROI()
{
  QTime tic;
  tic.start();

  if (m_Data && mask()) {
    double *res = new double[65536];
    long npixels = (m_Data -> get_Height()) * (m_Data -> get_Width());
    double *data = m_Data -> data();
    short  *maskp = mask() -> data();

    for (int i=0; i<65536; i++) {
      res[i] = 0;
    }

    for (long i=0; i<npixels; i++) {
      double v = data[i];
      short bin = maskp[i];
      bin &= 255;
      res[bin] += v;
    }

    int dt = tic.restart();

    printMessage(tr("ROI calculated in %1 msec").arg(dt));

    delete [] res;
  }
}

void QxrdDataProcessorBase::calculateHistogram()
{
  QTime tic;
  tic.start();

  if (m_Data && mask()) {
    double *res = new double[65536];
    long npixels = m_Data -> get_Height() * m_Data -> get_Width();
    double *data = m_Data -> data();
    short  *maskp = mask() -> data();

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
      short bin = maskp[i];
      res[bin] += v;
    }

    int dt = tic.restart();

    printMessage(tr("Histogram calculated in %1 msec").arg(dt));

    delete [] res;
  }
}

QxrdGenerateTestImageWPtr QxrdDataProcessorBase::generateTestImage() const
{
  return m_GenerateTestImage;
}

void QxrdDataProcessorBase::newOutputScan(QString title)
{
  m_OutputScan = QxrdAllocator::newIntegratedData(m_Allocator, QxrdAllocator::AlwaysAllocate, data());

  m_OutputScan -> set_Title(title);
}

void QxrdDataProcessorBase::appendToOutputScan(double x, double y)
{
  if (m_OutputScan) {
    m_OutputScan->append(x,y);
  }
}

void QxrdDataProcessorBase::plotOutputScan()
{
  if (m_OutputScan) {
    emit newIntegrationAvailable(m_OutputScan);
  }
}

void QxrdDataProcessorBase::saveOutputScan(QString fileName)
{
  if (m_OutputScan) {

  }
}

QStringList QxrdDataProcessorBase::integrateRectangle(int x0, int y0, int x1, int y1)
{
  double sum = 0;
  double npx = 0;

  QxrdDoubleImageDataPtr dat = m_Data;
  QxrdMaskDataPtr        msk = mask();
  QStringList            res;

  if (dat) {
    if (msk) {
      for (int y=y0; y<y1; y++) {
        for (int x=x0; x<x1; x++) {
          if (msk->value(x,y)) {
            sum += dat->value(x,y);
            npx += 1;
          }
        }
      }
    } else {
      for (int y=y0; y<y1; y++) {
        for (int x=x0; x<x1; x++) {
          sum += dat->value(x,y);
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

void QxrdDataProcessorBase::subtractDark()
{
  QxrdDoubleImageDataPtr dat = m_Data;
  QxrdDoubleImageDataPtr dark = m_DarkFrame;

  subtractDarkImage(dat, dark);
}

void QxrdDataProcessorBase::unsubtractDark()
{
  QxrdDoubleImageDataPtr dat = m_Data;
  QxrdDoubleImageDataPtr dark = m_DarkFrame;

  unsubtractDarkImage(dat, dark);
}

void QxrdDataProcessorBase::multiplyData(double scalar)
{
  QxrdDoubleImageDataPtr dat = m_Data;

  int wid = dat->get_Width();
  int ht  = dat->get_Height();

  for (int y=0; y<ht; y++) {
    for (int x=0; x<wid; x++) {
      dat->setValue(x, y, dat->value(x, y) * scalar);
    }
  }
}

void QxrdDataProcessorBase::offsetData(double offset)
{
  QxrdDoubleImageDataPtr dat = m_Data;

  int wid = dat->get_Width();
  int ht  = dat->get_Height();

  for (int y=0; y<ht; y++) {
    for (int x=0; x<wid; x++) {
      dat->setValue(x, y, dat->value(x, y) + offset);
    }
  }
}
