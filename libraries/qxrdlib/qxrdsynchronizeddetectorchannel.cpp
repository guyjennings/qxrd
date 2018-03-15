#include "qxrdsynchronizeddetectorchannel.h"
#include "qxrddetectorsettings.h"
#include "qxrdacqcommon.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdnidaq.h"
#include "qxrdexperiment.h"

QxrdSynchronizedDetectorChannel::QxrdSynchronizedDetectorChannel(QString name)
  : inherited(name),
    m_ChannelNumber(this, "channelNumber", -1, "Channel Number of Output"),
    m_DetectorNumber(this, "detectorNumber", -1, "Detector Number to synchronize"),
    m_DetectorType(this, "detectorType", -1, "Detector Type to synchronize")
{
}

void QxrdSynchronizedDetectorChannel::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_AcqCommon = QxrdAcqCommon::findAcquisition(parent);

  QxrdAcqCommonPtr acq(m_AcqCommon);

  if (acq) {
    m_SynchronizedAcquisition = acq->synchronizedAcquisition();
  }

  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

  if (sync) {
    m_NIDAQ = sync->nidaqPlugin();
  } else {
    printMessage("QxrdSynchronizedDetectorChannel::initialize parent not QxrdSynchronizedAcquisition");
  }
}

QString QxrdSynchronizedDetectorChannel::channelName()
{
  QString      res = "Unknown Channel";
  QxrdNIDAQPtr nidaq(m_NIDAQ);

  if (nidaq) {
    res = nidaq->detectorDeviceName(get_ChannelNumber());
  }

  return res;
}

QString QxrdSynchronizedDetectorChannel::detectorName()
{
  QString          res = "Unknown Detector";
  QxrdAcqCommonPtr acq(m_AcqCommon);

  if (acq) {
    QxrdDetectorSettingsPtr det = acq->detector(get_DetectorNumber());

    if (det) {
      res = det->get_DetectorName();
    }
  }

  return res;
}

int QxrdSynchronizedDetectorChannel::detectorTypeCount()
{
  return QxrdDetectorSettings::detectorTypeCount();
}

QString QxrdSynchronizedDetectorChannel::detectorTypeName(int n)
{
  return QxrdDetectorSettings::detectorTypeName(n);
}
