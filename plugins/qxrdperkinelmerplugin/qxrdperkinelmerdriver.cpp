#include "qxrdperkinelmerdriver.h"

QxrdPerkinElmerDriver::QxrdPerkinElmerDriver(QString name,
                                             QxrdDetectorSettingsWPtr det,
                                             QxrdExperimentWPtr expt,
                                             QxrdAcquisitionWPtr acq)
: QxrdDetectorDriver(name, det, expt, acq)
{

}
