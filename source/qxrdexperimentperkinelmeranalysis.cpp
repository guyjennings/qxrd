#include "qxrdexperimentperkinelmeranalysis.h"

QxrdExperimentPerkinElmerAnalysis::QxrdExperimentPerkinElmerAnalysis(QString path, QxrdApplication *app, QSettings *settings, QObject *parent) :
    QxrdExperiment(path, app, settings, parent)
{
}

void QxrdExperimentPerkinElmerAnalysis::readSettings(QSettings *settings, QString section)
{
  QxrdExperiment::readSettings(settings, section);
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdExperimentPerkinElmerAnalysis::writeSettings(QSettings *settings, QString section)
{
  QxrdExperiment::writeSettings(settings, section);
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}
