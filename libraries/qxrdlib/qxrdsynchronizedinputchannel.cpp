#include "qxrdsynchronizedinputchannel.h"

QxrdSynchronizedInputChannel::QxrdSynchronizedInputChannel(QString name)
  : inherited(name),
  m_ChannelNumber(this, "channelNumber", -1, "Extra Input Channel Number"),
  m_ChannelName(this, "channelName", "", "NIDAQ name of channel"),
  m_Mode(this, "mode", 0, "Channel Mode (0 = summed, 1 = averaged, 2 = maximum, 3 = minimum)"),
  m_SaveWave(this, "saveWave", 0, "Save entire waveform (0 = no, 1 = yes)"),
  m_Min(this, "min", -10.0, "Minimum Input value for Analog Channel (in Volts)"),
  m_Max(this, "max", 10.0, "Maximum Input Value for Analog Channel (in Volts)"),
  m_Waveform(this, "waveform", QcepDoubleVector(), "Waveform on Channel")
{
}
