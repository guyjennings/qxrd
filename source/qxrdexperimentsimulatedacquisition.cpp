#include "qxrdexperimentsimulatedacquisition.h"
#include "qxrdexperimentthread.h"
#include "qxrddetectorthread.h"

QxrdExperimentSimulatedAcquisition::QxrdExperimentSimulatedAcquisition
  (QString path, QxrdApplicationWPtr app) :
    QxrdExperiment(path, app)
{
  set_ExperimentKind(QxrdExperimentThread::SimulatedAcquisition);
  set_DetectorType(QxrdDetectorThread::SimulatedDetector);
}
