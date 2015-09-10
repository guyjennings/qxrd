#include "qxrddetectorproxy.h"
#include "qxrdacquisition.h"

QxrdDetectorProxy::QxrdDetectorProxy(QxrdDetectorWPtr det, QxrdAcquisitionWPtr acq)
  : QcepObject("detectorProxy", acq.data()),
    m_Detector(det)
{
}

