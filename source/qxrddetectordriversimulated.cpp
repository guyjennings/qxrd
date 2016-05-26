#include "qxrddetectordriversimulated.h"
#include "qxrddetectorsettingssimulated.h"

QxrdDetectorDriverSimulated::QxrdDetectorDriverSimulated(
    QString name,
    QxrdDetectorSettingsSimulatedPtr sim,
    QxrdExperimentPtr expt,
    QxrdAcquisitionPtr acq) :
  QxrdDetectorDriver(name, sim, expt, acq),
  m_Simulated(sim)
{
}
