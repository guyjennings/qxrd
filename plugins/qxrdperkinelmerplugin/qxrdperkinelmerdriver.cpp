#include "qxrdperkinelmerdriver.h"

QxrdPerkinElmerDriver::QxrdPerkinElmerDriver(QString name,
                                             QxrdDetectorSettingsPtr det,
                                             QxrdExperimentPtr expt,
                                             QxrdAcquisitionPtr acq)
: QxrdDetectorDriver(name, det, expt, acq)
{

}
