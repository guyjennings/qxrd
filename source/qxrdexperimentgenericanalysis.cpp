#include "qxrdexperimentgenericanalysis.h"
#include "qxrdexperimentthread.h"

QxrdExperimentGenericAnalysis::QxrdExperimentGenericAnalysis(QxrdExperimentThreadWPtr expthrd, QString path, QxrdApplicationWPtr app) :
    QxrdExperiment(expthrd, path, app)
{
  set_ExperimentKind(QxrdExperimentThread::GenericAnalysis);
}
