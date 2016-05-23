#include "qxrddetectorsettingsepicsarea.h"

QxrdDetectorSettingsEpicsArea::QxrdDetectorSettingsEpicsArea(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum) :
  QxrdDetectorSettings(expt, acq, EpicsAreaDetector, detNum)
{
}

QxrdDetectorSettingsEpicsArea::~QxrdDetectorSettingsEpicsArea()
{
}

void QxrdDetectorSettingsEpicsArea::pushDefaultsToProxy(QxrdDetectorProxyPtr proxy)
{
}
