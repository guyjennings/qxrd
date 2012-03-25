#include "qxrdexperimentperkinelmeracquisition.h"
#include "qxrdexperimentthread.h"
#include "qxrddetectorthread.h"

QxrdExperimentPerkinElmerAcquisition::QxrdExperimentPerkinElmerAcquisition(QString path,
                                                                           QxrdApplication *app) :
  QxrdExperiment(path, app)
{
  set_ExperimentKind(QxrdExperimentThread::PerkinElmerAcquisition);
  set_DetectorType(QxrdDetectorThread::PerkinElmerDetector);
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
