#include "qxrdexperimentpilatusacquisition.h"
#include "qxrdexperimentthread.h"

QxrdExperimentPilatusAcquisition::QxrdExperimentPilatusAcquisition(QString path, QxrdApplication *app, QSettings *settings, QObject *parent) :
    QxrdExperiment(path, app, settings, parent)
{
  set_ExperimentKind(QxrdExperimentThread::PilatusAcquisition);
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
