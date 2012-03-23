#include "qxrdexperimentperkinelmeranalysis.h"
#include "qxrdexperimentthread.h"

QxrdExperimentPerkinElmerAnalysis::QxrdExperimentPerkinElmerAnalysis(QxrdExperimentThreadWPtr expthrd, QString path, QxrdApplication* app) :
    QxrdExperiment(expthrd, path, app)
{
  set_ExperimentKind(QxrdExperimentThread::PerkinElmerAnalysis);
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
