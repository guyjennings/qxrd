#include "qxrdsynchronizedoutputchannel.h"

QxrdSynchronizedOutputChannel::QxrdSynchronizedOutputChannel(QString name)
  : inherited(name),
  m_ChannelNumber(this, "channelNumber", -1, "NIDAQ channel number"),
  m_ChannelName(this, "channelName", "", "NIDAQ name of channel"),
  m_Mode(this, "mode", 0, "Channel Mode (0 = summed, 1 = averaged, 2 = maximum, 3 = minimum)"),
  m_StartV(this, "startV", -10.0, "Starting value for output (in Volts)"),
  m_EndV(this, "endV", 10.0, "Ending value for output (in Volts)"),
  m_Waveform(this, "waveform", QcepDoubleVector(), "Waveform on Channel")
{
}
