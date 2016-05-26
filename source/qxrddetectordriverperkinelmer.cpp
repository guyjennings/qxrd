#include "qxrddetectordriverperkinelmer.h"
#include "qxrddetectorsettingsperkinelmer.h"

QxrdDetectorDriverPerkinElmer::QxrdDetectorDriverPerkinElmer(
    QString name,
    QxrdDetectorSettingsPerkinElmerPtr perkinElmer,
    QxrdExperimentPtr expt,
    QxrdAcquisitionPtr acq) :
  QxrdDetectorDriver(name, perkinElmer, expt, acq),
  m_PerkinElmer(perkinElmer)
{
}
