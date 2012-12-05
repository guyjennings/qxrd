#include "qxrdexperimentperkinelmeranalysis.h"
#include "qxrdexperimentthread.h"

QxrdExperimentPerkinElmerAnalysis::QxrdExperimentPerkinElmerAnalysis(QString path, QxrdApplicationWPtr app) :
    QxrdExperiment(path, app)
{
  set_ExperimentKind(QxrdExperimentThread::PerkinElmerAnalysis);
}
