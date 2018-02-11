#include "qxrddetectordriver.h"

QxrdDetectorDriver::QxrdDetectorDriver(QString name,
                                       QxrdDetectorSettingsWPtr det,
                                       QxrdExperimentWPtr expt,
                                       QxrdAcqCommonWPtr acq) :
  QcepObject(name),
  m_Detector(det),
  m_Experiment(expt),
  m_Acquisition(acq)
{
#ifndef QT_NO_DEBUG
  printf("Detector driver constructed \"%s\": Settings: %p Expt: %p Acq: %p\n",
         qPrintable(name), det.data(), expt.data(), acq.data());
#endif
}
