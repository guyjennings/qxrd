#include "qxrddetectordriver.h"

QxrdDetectorDriver::QxrdDetectorDriver(QString name,
                                       QxrdDetectorSettingsPtr det,
                                       QxrdExperimentPtr expt,
                                       QxrdAcquisitionPtr acq) :
  QcepObject(name),
  m_Detector(det),
  m_Experiment(expt),
  m_Acquisition(acq)
{
}
