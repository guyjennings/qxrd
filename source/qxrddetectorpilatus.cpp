#include "qxrddetectorpilatus.h"
#include "qxrddetectorthread.h"

QxrdDetectorPilatus::QxrdDetectorPilatus(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QxrdDetector(expt, acq)
{
}

int QxrdDetectorPilatus::detectorType() const
{
  return QxrdDetectorThread::PilatusDetector;
}

