#include "qxrdexperimentperkinelmeranalysis.h"
#include "qxrdexperimentthread.h"

QxrdExperimentPerkinElmerAnalysis::QxrdExperimentPerkinElmerAnalysis(QxrdExperimentThreadWPtr expthrd, QString path, QxrdApplicationWPtr app) :
    QxrdExperiment(expthrd, path, app)
{
  set_ExperimentKind(QxrdExperimentThread::PerkinElmerAnalysis);
}
