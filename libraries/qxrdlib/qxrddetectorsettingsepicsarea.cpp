#include "qxrddetectorsettingsepicsarea.h"

QxrdDetectorSettingsEpicsArea::QxrdDetectorSettingsEpicsArea(QxrdApplicationWPtr app,
                                                             QxrdExperimentWPtr expt,
                                                             QxrdAcquisitionWPtr acq,
                                                             int detNum) :
  QxrdDetectorSettings(app, expt, acq, EpicsAreaDetector, detNum)
{
}

QxrdDetectorSettingsEpicsArea::~QxrdDetectorSettingsEpicsArea()
{
}

void QxrdDetectorSettingsEpicsArea::pushDefaultsToProxy(QxrdDetectorProxyPtr proxy)
{
}
