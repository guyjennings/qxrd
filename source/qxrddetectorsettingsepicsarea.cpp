#include "qxrddetectorsettingsepicsarea.h"
#include "qxrddetectorthread.h"

QxrdDetectorSettingsEpicsArea::QxrdDetectorSettingsEpicsArea(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum) :
  QxrdDetectorSettings(expt, acq, QxrdDetectorThread::EpicsAreaDetector, detNum)
{
}

QxrdDetectorSettingsEpicsArea::~QxrdDetectorSettingsEpicsArea()
{
}
