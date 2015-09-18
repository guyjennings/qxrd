#include "qxrdroicalculator.h"
#include "qxrddebug.h"
#include <stdio.h>
#include "qcepmutexlocker.h"

QxrdROICalculator::QxrdROICalculator(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp, QxrdDetectorProcessorWPtr proc)
  : QcepObject("ROIcalculator", NULL),
    m_Saver(saver),
    m_Experiment(exp),
    m_Processor(proc),
    m_RoiCount(saver, this, "roiCount", 0, "Number of ROIs")
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdROICalculator::QxrdROICalculator(%p)\n", this);
  }
}

QxrdROICalculator::~QxrdROICalculator()
{
#ifndef QT_NO_DEBUG
  printf("Deleting ROI Calculator\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdROICalculator::~QxrdROICalculator(%p)\n", this);
  }
}

QScriptValue QxrdROICalculator::toScriptValue(QScriptEngine *engine, const QxrdROICalculatorPtr &proc)
{
  return engine->newQObject(proc.data());
}

void QxrdROICalculator::fromScriptValue(const QScriptValue &obj, QxrdROICalculatorPtr &proc)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdROICalculator *f = qobject_cast<QxrdROICalculator*>(qobj);

    if (f) {
      proc = QxrdROICalculatorPtr(f);
    }
  }
}

void QxrdROICalculator::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings, section);

//  if (m_Processor) {
//    m_Processor->readSettings(settings, section+"/processor");
//  }
}

void QxrdROICalculator::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);

//  if (m_Processor) {
//    m_Processor->writeSettings(settings, section+"/processor");
//  }
}
