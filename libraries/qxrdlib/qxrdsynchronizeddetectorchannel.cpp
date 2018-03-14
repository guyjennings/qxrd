#include "qxrdsynchronizeddetectorchannel.h"
#include "qxrddetectorsettings.h"

QxrdSynchronizedDetectorChannel::QxrdSynchronizedDetectorChannel(QString name)
  : inherited(name),
    m_ChannelNumber(this, "channelNumber", -1, "Channel Number of Output"),
    m_DetectorNumber(this, "detectorNumber", -1, "Detector Number to synchronize"),
    m_DetectorType(this, "detectorType", -1, "Detector Type to synchronize")
{
}

QString QxrdSynchronizedDetectorChannel::channelName()
{
  return tr("ctr%1").arg(get_ChannelNumber());
}

QString QxrdSynchronizedDetectorChannel::detectorName()
{
  return tr("det%1").arg(get_DetectorNumber());
}

QString QxrdSynchronizedDetectorChannel::detectorTypeName()
{
  return QxrdDetectorSettings::detectorTypeName(get_DetectorType());
}
