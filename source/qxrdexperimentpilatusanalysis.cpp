#include "qxrdexperimentpilatusanalysis.h"
#include "qxrdexperimentthread.h"

QxrdExperimentPilatusAnalysis::QxrdExperimentPilatusAnalysis(QxrdExperimentThreadWPtr expthrd, QString path, QxrdApplicationWPtr app) :
    QxrdExperiment(expthrd, path, app)
{
  set_ExperimentKind(QxrdExperimentThread::PilatusAnalysis);
}
