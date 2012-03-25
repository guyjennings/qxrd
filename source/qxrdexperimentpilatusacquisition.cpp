#include "qxrdexperimentpilatusacquisition.h"
#include "qxrdexperimentthread.h"
#include "qxrddetectorthread.h"

QxrdExperimentPilatusAcquisition::QxrdExperimentPilatusAcquisition(QString path, QxrdApplication* app) :
    QxrdExperiment(path, app)
{
  set_ExperimentKind(QxrdExperimentThread::PilatusAcquisition);
  set_DetectorType(QxrdDetectorThread::PilatusDetector);
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
