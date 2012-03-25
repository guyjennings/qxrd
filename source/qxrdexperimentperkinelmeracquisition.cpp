#include "qxrdexperimentperkinelmeracquisition.h"
#include "qxrdexperimentthread.h"
#include "qxrddetectorthread.h"

QxrdExperimentPerkinElmerAcquisition::QxrdExperimentPerkinElmerAcquisition(QString path,
                                                                           QxrdApplication *app) :
  QxrdExperiment(path, app)
{
  set_ExperimentKind(QxrdExperimentThread::PerkinElmerAcquisition);
  set_DetectorType(QxrdDetectorThread::PerkinElmerDetector);
}
