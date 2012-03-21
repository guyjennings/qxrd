#include "qxrdexperimentpilatusanalysis.h"
#include "qxrdexperimentthread.h"

QxrdExperimentPilatusAnalysis::QxrdExperimentPilatusAnalysis(QString path, QxrdApplication* app) :
    QxrdExperiment(path, app)
{
  set_ExperimentKind(QxrdExperimentThread::PilatusAnalysis);
}

void QxrdExperimentPilatusAnalysis::readSettings(QSettings *settings, QString section)
{
  QxrdExperiment::readSettings(settings, section);
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdExperimentPilatusAnalysis::writeSettings(QSettings *settings, QString section)
{
  QxrdExperiment::writeSettings(settings, section);
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}
