#include "qxrddetectordriver.h"

QxrdDetectorDriver::QxrdDetectorDriver(QString name,
                                       QxrdDetectorSettingsWPtr det,
                                       QxrdExperimentWPtr expt,
                                       QxrdAcqCommonWPtr acq) :
  QcepObject(name),
  m_Detector(det),
  m_Experiment(expt),
  m_Acquisition(acq),
  m_ExposureTime(0.0),
  m_ExposureFactor(1),
  m_FrameCounter(0),
  m_SubframeCounter(0)
{
#ifndef QT_NO_DEBUG
  printf("Detector driver constructed \"%s\": Settings: %p Expt: %p Acq: %p\n",
         qPrintable(name), det.data(), expt.data(), acq.data());
#endif
}
