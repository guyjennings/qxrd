#include "qxrdexperimentgenericanalysis.h"
#include "qxrdexperimentthread.h"

QxrdExperimentGenericAnalysis::QxrdExperimentGenericAnalysis(QString path, QxrdApplication* app, QSettings *settings) :
    QxrdExperiment(path, app, settings)
{
  set_ExperimentKind(QxrdExperimentThread::GenericAnalysis);
}

void QxrdExperimentGenericAnalysis::readSettings(QSettings *settings, QString section)
{
  QxrdExperiment::readSettings(settings, section);
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdExperimentGenericAnalysis::writeSettings(QSettings *settings, QString section)
{
  QxrdExperiment::writeSettings(settings, section);
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}
