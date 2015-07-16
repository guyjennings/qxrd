#include "qxrdexperimentsimulatedacquisition.h"
#include "qxrdexperimentthread.h"
#include "qxrddetectorthread.h"

QxrdExperimentSimulatedAcquisition::QxrdExperimentSimulatedAcquisition
  (QxrdExperimentThreadWPtr expthrd, QString path, QxrdApplicationWPtr app) :
    QxrdExperiment(expthrd, path, app)
{
  set_ExperimentKind(QxrdExperimentThread::SimulatedAcquisition);
  set_DetectorType(QxrdDetectorThread::SimulatedDetector);
}
