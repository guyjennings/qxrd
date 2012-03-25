#include "qxrdexperimentsimulatedacquisition.h"
#include "qxrdexperimentthread.h"
#include "qxrddetectorthread.h".h"

QxrdExperimentSimulatedAcquisition::QxrdExperimentSimulatedAcquisition
  (QString path, QxrdApplication *app) :
    QxrdExperiment(path, app)
{
  set_ExperimentKind(QxrdExperimentThread::SimulatedAcquisition);
  set_DetectorType(QxrdDetectorThread::SimulatedDetector);
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
