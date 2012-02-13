#include "qxrdexperimentpilatusacquisition.h"
#include "qxrdexperimentthread.h"
#include "qxrdacquisition.h"

QxrdExperimentPilatusAcquisition::QxrdExperimentPilatusAcquisition(QString path, QxrdApplication* app, QSettings *settings) :
    QxrdExperiment(path, app, settings)
{
  set_ExperimentKind(QxrdExperimentThread::PilatusAcquisition);
  set_DetectorType(QxrdAcquisition::PilatusDetector);
}

void QxrdExperimentPilatusAcquisition::readSettings(QSettings *settings, QString section)
{
  QxrdExperiment::readSettings(settings, section);
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdExperimentPilatusAcquisition::writeSettings(QSettings *settings, QString section)
{
  QxrdExperiment::writeSettings(settings, section);
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}
