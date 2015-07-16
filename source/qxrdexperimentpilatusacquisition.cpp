#include "qxrdexperimentpilatusacquisition.h"
#include "qxrdexperimentthread.h"
#include "qxrddetectorthread.h"

QxrdExperimentPilatusAcquisition::QxrdExperimentPilatusAcquisition(QxrdExperimentThreadWPtr expthrd, QString path, QxrdApplicationWPtr app) :
    QxrdExperiment(expthrd, path, app)
{
  set_ExperimentKind(QxrdExperimentThread::PilatusAcquisition);
  set_DetectorType(QxrdDetectorThread::PilatusDetector);
}
