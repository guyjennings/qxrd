#include "qxrdexperimentperkinelmeracquisition.h"
#include "qxrdexperimentthread.h"
#include "qxrddetectorthread.h"

QxrdExperimentPerkinElmerAcquisition::QxrdExperimentPerkinElmerAcquisition(QxrdExperimentThreadWPtr expthrd,
                                                                           QString path,
                                                                           QxrdApplicationWPtr app) :
  QxrdExperiment(expthrd, path, app)
{
  set_ExperimentKind(QxrdExperimentThread::PerkinElmerAcquisition);
  set_DetectorType(QxrdDetectorThread::PerkinElmerDetector);
}
