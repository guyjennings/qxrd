#include "qxrdexperimentpilatusanalysis.h"
#include "qxrdexperimentthread.h"

QxrdExperimentPilatusAnalysis::QxrdExperimentPilatusAnalysis(QString path, QxrdApplication* app) :
    QxrdExperiment(path, app)
{
  set_ExperimentKind(QxrdExperimentThread::PilatusAnalysis);
}
