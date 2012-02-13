#include "qxrdexperimentperkinelmeracquisition.h"
#include "qxrdexperimentthread.h"
#include "qxrdacquisition.h"

QxrdExperimentPerkinElmerAcquisition::QxrdExperimentPerkinElmerAcquisition(QString path,
                                                                           QxrdApplication* app,
                                                                           QSettings *settings) :
    QxrdExperiment(path, app, settings)
{
  set_ExperimentKind(QxrdExperimentThread::PerkinElmerAcquisition);
  set_DetectorType(QxrdAcquisition::PerkinElmerDetector);
}

void QxrdExperimentPerkinElmerAcquisition::readSettings(QSettings *settings, QString section)
{
  QxrdExperiment::readSettings(settings, section);
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdExperimentPerkinElmerAcquisition::writeSettings(QSettings *settings, QString section)
{
  QxrdExperiment::writeSettings(settings, section);
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}
