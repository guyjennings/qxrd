#include "qxrddetectordriverepicsarea.h"
#include "qxrddetectorsettingsepicsarea.h"

QxrdDetectorDriverEpicsArea::QxrdDetectorDriverEpicsArea(
    QString name,
    QxrdDetectorSettingsEpicsAreaPtr det,
    QxrdExperimentPtr expt,
    QxrdAcquisitionPtr acq) :
  QxrdDetectorDriver(name, det, expt, acq)
{
#ifndef QT_NO_DEBUG
  printf("Epics Area Detector Driver \"%s\" Constructed (Non-Implemented)\n", qPrintable(name));
#endif
}

QxrdDetectorDriverEpicsArea::~QxrdDetectorDriverEpicsArea()
{
#ifndef QT_NO_DEBUG
  printf("Epics Area Detector Driver \"%s\" Destroyed\n", qPrintable(get_Name()));
#endif
}

bool QxrdDetectorDriverEpicsArea::startDetectorDriver()
{
  return false;
}

bool QxrdDetectorDriverEpicsArea::stopDetectorDriver()
{
  return false;
}

bool QxrdDetectorDriverEpicsArea::changeExposureTime(double exposure)
{
  return false;
}

bool QxrdDetectorDriverEpicsArea::beginAcquisition(double exposure)
{
  return false;
}

bool QxrdDetectorDriverEpicsArea::endAcquisition()
{
  return false;
}

bool QxrdDetectorDriverEpicsArea::shutdownAcquisition()
{
  return false;
}

void QxrdDetectorDriverEpicsArea::beginFrame()
{
}
