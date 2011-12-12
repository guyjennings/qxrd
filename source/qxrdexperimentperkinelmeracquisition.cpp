#include "qxrdexperimentperkinelmeracquisition.h"
#include "qxrdexperimentthread.h"

QxrdExperimentPerkinElmerAcquisition::QxrdExperimentPerkinElmerAcquisition(QString path,
                                                                           QxrdApplication *app,
                                                                           QSettings *settings,
                                                                           QObject *parent) :
    QxrdExperiment(path, app, settings, parent)
{
  set_ExperimentKind(QxrdExperimentThread::PerkinElmerAcquisition);
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
