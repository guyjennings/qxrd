#include "qxrddetectorprocessor.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrator.h"
#include "qcepmutexlocker.h"

QxrdDetectorProcessor::QxrdDetectorProcessor(
    QcepSettingsSaverWPtr saver,
    QxrdExperimentWPtr    doc,
    QxrdDetectorWPtr      det)
  : QcepObject("acquisitionProcessor", NULL),
    m_Saver(saver),
    m_Experiment(doc),
    m_Detector(det),
    m_CenterFinder(new QxrdCenterFinder(saver, doc)),
    m_Integrator(new QxrdIntegrator(saver, doc, m_CenterFinder))
{
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

