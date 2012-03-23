#include "qxrdexperimentsimulatedacquisition.h"
#include "qxrdexperimentthread.h"
#include "qxrdacquisition.h"

QxrdExperimentSimulatedAcquisition::QxrdExperimentSimulatedAcquisition
  (QxrdExperimentThreadWPtr expthrd, QString path, QxrdApplication* app) :
    QxrdExperiment(expthrd, path, app)
{
  set_ExperimentKind(QxrdExperimentThread::SimulatedAcquisition);
  set_DetectorType(QxrdAcquisition::SimulatedDetector);
}

void QxrdExperimentSimulatedAcquisition::readSettings(QSettings *settings, QString section)
{
  QxrdExperiment::readSettings(settings, section);
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdExperimentSimulatedAcquisition::writeSettings(QSettings *settings, QString section)
{
  QxrdExperiment::writeSettings(settings, section);
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}
