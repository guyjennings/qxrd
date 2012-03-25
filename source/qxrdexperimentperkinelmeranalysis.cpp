#include "qxrdexperimentperkinelmeranalysis.h"
#include "qxrdexperimentthread.h"

QxrdExperimentPerkinElmerAnalysis::QxrdExperimentPerkinElmerAnalysis(QString path, QxrdApplication *app) :
    QxrdExperiment(path, app)
{
  set_ExperimentKind(QxrdExperimentThread::PerkinElmerAnalysis);
}
