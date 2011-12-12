#include "qxrdexperimentsimulatedacquisition.h"
#include "qxrdexperimentthread.h"

QxrdExperimentSimulatedAcquisition::QxrdExperimentSimulatedAcquisition(QString path, QxrdApplication *app, QSettings *settings, QObject *parent) :
    QxrdExperiment(path, app, settings, parent)
{
  set_ExperimentKind(QxrdExperimentThread::SimulatedAcquisition);
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
