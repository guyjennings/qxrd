#include "qxrdexperimentgenericanalysis.h"
#include "qxrdexperimentthread.h"

QxrdExperimentGenericAnalysis::QxrdExperimentGenericAnalysis(QString path, QxrdApplication *app) :
    QxrdExperiment(path, app)
{
  set_ExperimentKind(QxrdExperimentThread::GenericAnalysis);
}
