#include "qxrdsynchronizeddetectorchannel.h"

QxrdSynchronizedDetectorChannel::QxrdSynchronizedDetectorChannel(QString name)
  : inherited(name),
    m_ChannelNumber(this, "channelNumber", -1, "Channel Number of Output"),
    m_DetectorNumber(this, "detectorNumber", -1, "Detector Number to synchronize"),
    m_DetectorType(this, "detectorType", -1, "Detector Type to synchronize")
{
}
