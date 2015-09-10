#include "qxrddetectorproxy.h"
#include "qxrdacquisition.h"
#include "qxrddetector.h"
#include "qxrddetectorthread.h"

QxrdDetectorProxy::QxrdDetectorProxy(QxrdDetectorThreadPtr thr,
                                     QxrdDetectorPtr       det,
                                     QxrdAcquisitionPtr    acq)
  : QcepObject("detectorProxy", acq.data()),
    m_Acquisition(acq),
    m_DetectorThread(thr),
    m_Detector(det)
{
  if (det) {
    m_DetectorType = det->get_DetectorType();
  }
}

QxrdDetectorProxy::QxrdDetectorProxy(int detectorType, QxrdAcquisitionPtr acq)
  : QcepObject("detectorProxy", acq.data()),
    m_Acquisition(acq),
    m_DetectorThread(),
    m_Detector(),
    m_DetectorType(detectorType)
{
}

int QxrdDetectorProxy::detectorType()
{
  return m_DetectorType;
}

QString QxrdDetectorProxy::detectorTypeName()
{
  return QxrdDetectorThread::detectorTypeName(m_DetectorType);
}

QxrdDetectorThreadPtr QxrdDetectorProxy::detectorThread()
{
  return m_DetectorThread;
}

QxrdDetectorPtr QxrdDetectorProxy::detector()
{
  return m_Detector;
}
