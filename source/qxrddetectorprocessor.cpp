#include "qxrddetectorprocessor.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrator.h"
#include "qcepmutexlocker.h"
#include "qxrdroicalculator.h"
#include "qxrddebug.h"
#include "qxrdexperiment.h"
#include "qxrddetectorcontrolwindow.h"

QxrdDetectorProcessor::QxrdDetectorProcessor(
    QcepSettingsSaverWPtr saver,
    QxrdExperimentWPtr    doc,
    QxrdFileSaverWPtr     fsav,
    QxrdDetectorWPtr      det)
  : QcepObject("acquisitionProcessor", doc.data()),
    m_Saver(saver),
    m_DetectorDisplayMode(saver, this, "detectorDisplayMode", ImageDisplayMode, "Detector Display Mode"),
    m_PerformDarkSubtraction(saver, this, "performDarkSubtraction", true, "Perform Dark Subtraction?"),
    m_SaveRawImages(saver, this, "saveRawImages", true, "Save Raw Images?"),
    m_SaveDarkImages(saver, this, "saveDarkImages", true, "Save Dark Images?"),
    m_PerformBadPixels(saver, this, "performBadPixels", true, "Perform Bad Pixel Correction?"),
    m_PerformGainCorrection(saver, this, "performGainCorrection", true, "Perform Gain Correction?"),
    m_SaveSubtracted(saver, this, "saveSubtracted", true, "Save Dark Subtracted Data?"),
    m_DarkImagePath(saver, this, "darkImagePath", "", "Dark Images Path"),
    m_RawDataSubdir(saver, this, "rawDataSubdir", "", "Raw Data Subdirectory"),
    m_DarkDataSubdir(saver, this, "darkDataSubdir", "", "Dark Data Subdirectory"),
    m_BadPixelsPath(saver, this, "badPixelsPath", "", "Bad Pixels Path"),
    m_GainMapPath(saver, this, "gainMapPath", "", "Gain Map Path"),
    m_SubtractedSubdir(saver, this, "subtractedSubdir", "", "Subtracted Data Subdirectory"),
    m_MaskPath(saver, this, "maskPath", "", "Mask Path"),

    m_PerformIntegration(saver, this, "performIntegration", true, "Perform Circular Integration?"),
    m_DisplayIntegratedData(saver, this, "displayIntegratedData", true, "Display Integrated Data?"),
    m_SaveIntegratedData(saver, this, "saveIntegratedData", true, "Save Integrated Data?"),
    m_IntegratedDataFile(saver, this, "integratedDataFile", "", "Integrated Data File"),
    m_SaveIntegratedDataSeparate(saver, this, "saveIntegratedDataSeparate", false, "Save Integrated Data in Separate Files?"),
    m_IntegratedDataSubdir(saver, this, "integratedDataSubdir", "", "Integrated Data Subdir"),

    m_AccumulateIntegrated2D(saver, this, "accumulateIntegrated2D", 0, "Accumulate integrated data in 2-d dataset"),
    m_AccumulateIntegratedName(saver, this, "accumulateIntegratedName", "", "Dataset name for accumulated data"),

    m_CalculateROICounts(saver, this, "calculateROICounts", true, "Calculate ROI Counts"),
    m_DisplayROIBorders(saver, this, "displayROIBorders", true, "Display ROIs in image"),

    m_RoiCounts(QcepSettingsSaverWPtr(), this, "roiCounts", QcepDoubleVector(), "ROI Counts"),

    m_Experiment(doc),
    m_FileSaver(fsav),
    m_Detector(det),
    m_CenterFinder(),
    m_Integrator(),
    m_ROICalculator(),
    m_ControlWindow(),
    m_ImagePlotSettings(new QxrdImagePlotSettings(saver, this))
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorProcessor::QxrdDetectorProcessor(%p)\n", this);
  }
}

QxrdDetectorProcessor::~QxrdDetectorProcessor()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorProcessor::~QxrdDetectorProcessor(%p)\n", this);
  }
}

void QxrdDetectorProcessor::initialize()
{
  m_CenterFinder  = QxrdCenterFinderPtr(new QxrdCenterFinder(m_Saver, m_Experiment));
  m_Integrator    = QxrdIntegratorPtr(new QxrdIntegrator(m_Saver, m_Experiment, m_CenterFinder));
  m_ROICalculator = QxrdROICalculatorPtr(new QxrdROICalculator(m_Saver, m_Experiment, sharedFromThis()));
}

void QxrdDetectorProcessor::setControlWindow(QxrdDetectorControlWindowWPtr ctrl)
{
  m_ControlWindow = ctrl;
}

void QxrdDetectorProcessor::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings, section);

  if (m_CenterFinder) {
    m_CenterFinder->readSettings(settings, section+"/centerFinder");
  }

  if (m_Integrator) {
    m_Integrator->readSettings(settings, section+"/integrator");
  }

  if (m_ROICalculator) {
    m_ROICalculator->readSettings(settings, section+"/roiCalculator");
  }

  if (m_ImagePlotSettings) {
    m_ImagePlotSettings->readSettings(settings, section+"/imagePlotSettings");
  }
}

void QxrdDetectorProcessor::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);

  if (m_CenterFinder) {
    m_CenterFinder->writeSettings(settings, section+"/centerFinder");
  }

  if (m_Integrator) {
    m_Integrator->writeSettings(settings, section+"/integrator");
  }

  if (m_ROICalculator) {
    m_ROICalculator->writeSettings(settings, section+"/roiCalculator");
  }

  if (m_ImagePlotSettings) {
    m_ImagePlotSettings->writeSettings(settings, section+"/imagePlotSettings");
  }
}

QScriptValue QxrdDetectorProcessor::toScriptValue(QScriptEngine *engine, const QxrdDetectorProcessorPtr &proc)
{
  return engine->newQObject(proc.data());
}

void QxrdDetectorProcessor::fromScriptValue(const QScriptValue &obj, QxrdDetectorProcessorPtr &proc)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdDetectorProcessor *qproc = qobject_cast<QxrdDetectorProcessor*>(qobj);

    if (qproc) {
      proc = QxrdDetectorProcessorPtr(qproc);
    }
  }
}

QxrdCenterFinderPtr QxrdDetectorProcessor::centerFinder()
{
  return m_CenterFinder;
}

QxrdIntegratorPtr QxrdDetectorProcessor::integrator()
{
  return m_Integrator;
}

QxrdROICalculatorPtr QxrdDetectorProcessor::roiCalculator()
{
  return m_ROICalculator;
}

void QxrdDetectorProcessor::processAcquiredImage(QString filePattern,
                                                 int fileIndex,
                                                 int phase,
                                                 int nPhases,
                                                 bool trig,
                                                 QcepInt32ImageDataPtr image,
                                                 QcepMaskDataPtr overflow)
{
  if (image) {
    QcepImageDataBasePtr img = image;

    if (qcepDebug(DEBUG_ACQUIRE)) {
      printMessage(tr("QxrdDetectorProcessor::processAcquiredImage(\"%1\",...")
                   .arg(img->get_FileName()));
    }

    QxrdDetectorControlWindowPtr ctrl(m_ControlWindow);

    if (get_SaveRawImages()) {
      doSaveRawImage(img);
    }

    if (ctrl && get_DetectorDisplayMode() == ImageDisplayMode) {
      ctrl->displayNewData(image, overflow);
    }

    if (get_PerformDarkSubtraction()) {
      img = doDarkSubtraction(img);
    }

    if (get_PerformBadPixels()) {
      img = doBadPixels(img);
    }

    if (get_PerformGainCorrection()) {
      img = doGainCorrection(img);
    }

    if (get_CalculateROICounts()) {
      set_RoiCounts(doCalculateROICounts(img));
    }

    if (get_SaveSubtracted()) {
      doSaveSubtractedImage(img);
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

void QxrdDetectorProcessor::processDarkImage(QString filePattern,
                                             int fileIndex,
                                             QcepInt32ImageDataPtr image,
                                             QcepMaskDataPtr overflow)
{
  if (image) {
    if (qcepDebug(DEBUG_ACQUIRE)) {
      printMessage(tr("QxrdDetectorProcessor::processDarkImage(\"%1\",...")
                   .arg(image->get_FileName()));
    }

    if (get_SaveDarkImages()) {
      doSaveDarkImage(image);
    }
  }
}

void QxrdDetectorProcessor::processIdleImage(QcepImageDataBasePtr image)
{
  if (image) {
    printMessage(tr("QxrdDetectorProcessor::processIdleImage(\"%1\"")
               .arg(image->get_FileName()));
  }
}

QxrdImagePlotSettingsWPtr QxrdDetectorProcessor::imagePlotSettings()
{
  return m_ImagePlotSettings;
}

QcepImageDataBasePtr QxrdDetectorProcessor::doDarkSubtraction(QcepImageDataBasePtr img)
{
  printMessage("Dark Subtraction not yet implemented");

  return img;
}

QcepImageDataBasePtr QxrdDetectorProcessor::doBadPixels(QcepImageDataBasePtr img)
{
  printMessage("Bad Pixel Correction not yet implemented");

  return img;
}

QcepImageDataBasePtr QxrdDetectorProcessor::doGainCorrection(QcepImageDataBasePtr img)
{
  printMessage("Gain Correction not yet implemented");

  return img;
}

QcepDoubleVector QxrdDetectorProcessor::doCalculateROICounts(QcepImageDataBasePtr img)
{
  printMessage("ROI Calculation not yet implemented");

  return QcepDoubleVector();
}

void QxrdDetectorProcessor::doSaveRawImage(QcepImageDataBasePtr img)
{
  printMessage("Save Raw Image not yet implemented");
}

void QxrdDetectorProcessor::doSaveDarkImage(QcepImageDataBasePtr img)
{
  printMessage("Save Dark Image not yet implemented");
}

void QxrdDetectorProcessor::doSaveSubtractedImage(QcepImageDataBasePtr img)
{
  printMessage("Save Subtracted Image not yet implemented");
}
