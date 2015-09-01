#include "qxrddetectorpilatus.h"
#include "qxrddetectorthread.h"

QxrdDetectorPilatus::QxrdDetectorPilatus(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QxrdDetector(saver, expt, acq, QxrdDetectorThread::PilatusDetector)
{
}
