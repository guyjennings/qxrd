#include "qxrdacquisitionprocessor.h"

QxrdAcquisitionProcessor::QxrdAcquisitionProcessor(
    QcepSettingsSaverWPtr saver,
    QxrdExperimentWPtr    doc,
    QxrdDetectorWPtr      det)
  : QcepObject("acquisitionProcessor", NULL),
    m_Saver(saver),
    m_Experiment(doc),
    m_Detector(det)
{
}

QScriptValue QxrdAcquisitionProcessor::toScriptValue(QScriptEngine *engine, const QxrdAcquisitionProcessorPtr &proc)
{
  return engine->newQObject(proc.data());
}

void QxrdAcquisitionProcessor::fromScriptValue(const QScriptValue &obj, QxrdAcquisitionProcessorPtr &proc)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdAcquisitionProcessor *qproc = qobject_cast<QxrdAcquisitionProcessor*>(qobj);

    if (qproc) {
      proc = QxrdAcquisitionProcessorPtr(qproc);
    }
  }
}

