#include "qxrdexperimentpilatusacquisition.h"
#include "qxrdexperimentthread.h"
#include "qxrddetectorthread.h"

QxrdExperimentPilatusAcquisition::QxrdExperimentPilatusAcquisition(QString path, QxrdApplicationWPtr app) :
    QxrdExperiment(path, app)
{
  set_ExperimentKind(QxrdExperimentThread::PilatusAcquisition);
  set_DetectorType(QxrdDetectorThread::PilatusDetector);
}
