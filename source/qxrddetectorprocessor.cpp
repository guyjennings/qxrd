#include "qxrddetectorprocessor.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrator.h"
#include "qcepmutexlocker.h"
#include "qxrdroicalculator.h"
#include "qxrddebug.h"
#include "qxrdexperiment.h"

QxrdDetectorProcessor::QxrdDetectorProcessor(
    QcepSettingsSaverWPtr saver,
    QxrdExperimentWPtr    doc,
    QxrdDetectorWPtr      det)
  : QcepObject("acquisitionProcessor", doc.data()),
    m_Saver(saver),
    m_PerformDarkSubtraction(saver, this, "performDarkSubtraction", true, "Perform Dark Subtraction?"),
    m_SaveRawImages(saver, this, "saveRawImages", true, "Save Raw Images?"),
    m_SaveDarkImages(saver, this, "saveDarkImages", true, "Save Dark Images?"),
    m_PerformBadPixels(saver, this, "performBadPixels", true, "Perform Bad Pixel Correction?"),
    m_PerformGainCorrection(saver, this, "performGainCorrection", true, "Perform Gain Correction?"),
    m_SaveSubtracted(saver, this, "saveSubtracted", true, "Save Dark Subtracted Data?"),
    m_PerformIntegration(saver, this, "performIntegration", true, "Perform Circular Integration?"),
    m_DisplayIntegratedData(saver, this, "displayIntegratedData", true, "Display Integrated Data?"),
    m_SaveIntegratedData(saver, this, "saveIntegratedData", true, "Save Integrated Data?"),
    m_AccumulateIntegrated2D(saver, this, "accumulateIntegrated2D", 0, "Accumulate integrated data in 2-d dataset"),
    m_CalculateROICounts(saver, this, "calculateROICounts", true, "Calculate ROI Counts"),
    m_Experiment(doc),
    m_Detector(det),
    m_CenterFinder(),
    m_Integrator(),
    m_ROICalculator()
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
    printMessage(tr("QxrdDetectorProcessor::processAcquiredImage(\"%1\",...")
                 .arg(image->get_FileName()));
  } else {
    printMessage(tr("QxrdDetectorProcessor::processAcquiredImage(NULL,..."));
  }
}

void QxrdDetectorProcessor::processDarkImage(QString filePattern,
                                             int fileIndex,
                                             QcepInt32ImageDataPtr image,
                                             QcepMaskDataPtr overflow)
{
  if (image) {
    printMessage(tr("QxrdDetectorProcessor::processDarkImage(\"%1\",...")
                 .arg(image->get_FileName()));
  } else {
    printMessage(tr("QxrdDetectorProcessor::processDarkImage(NULL,..."));
  }
}

void QxrdDetectorProcessor::processIdleImage(QcepInt16ImageDataPtr image)
{
  if (image) {
    printMessage(tr("QxrdDetectorProcessor::processIdleImage(\"%1\"")
               .arg(image->get_FileName()));
  }
}
