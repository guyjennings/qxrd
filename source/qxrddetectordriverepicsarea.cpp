#include "qxrddetectordriverepicsarea.h"
#include "qxrddetectorsettingsepicsarea.h"

QxrdDetectorDriverEpicsArea::QxrdDetectorDriverEpicsArea(
    QString name,
    QxrdDetectorSettingsEpicsAreaPtr det,
    QxrdExperimentPtr expt,
    QxrdAcquisitionPtr acq) :
  QxrdDetectorDriver(name, det, expt, acq)
{
}
