#include "qxrddetector.h"

QxrdDetector::QxrdDetector(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QObject(),
  m_Experiment(expt),
  m_Acquisition(acq)
{
}

QxrdDetector::~QxrdDetector()
{
}
