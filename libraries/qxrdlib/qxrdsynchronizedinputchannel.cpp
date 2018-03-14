#include "qxrdsynchronizedinputchannel.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdnidaq.h"

QxrdSynchronizedInputChannel::QxrdSynchronizedInputChannel(QString name)
  : inherited(name),
  m_ChannelNumber(this, "channelNumber", -1, "Extra Input Channel Number"),
  m_SignalName(this, "signalName", "", "NIDAQ name of channel"),
  m_Mode(this, "mode", ModeAveraged, "Channel Mode (0 = summed, 1 = averaged, 2 = maximum, 3 = minimum)"),
  m_SaveWave(this, "saveWave", 0, "Save entire waveform (0 = no, 1 = yes)"),
  m_Min(this, "min", -10.0, "Minimum Input value for Analog Channel (in Volts)"),
  m_Max(this, "max", 10.0, "Maximum Input Value for Analog Channel (in Volts)"),
  m_SampleRate(this, "sampleRate", 1000, "Sampling rate of analog channel (in Hz)"),
  m_Waveform(this, "waveform", QcepDoubleVector(), "Waveform on Channel")
{
}

void QxrdSynchronizedInputChannel::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  QxrdSynchronizedAcquisitionPtr acq(
        qSharedPointerDynamicCast<QxrdSynchronizedAcquisition>(parent));

  if (acq) {
    m_NIDAQ = acq->nidaqPlugin();
  } else {
    printMessage("QxrdSynchronizedInputChannel::initialize parent not QxrdSynchronizedAcquisition");
  }
}

QString QxrdSynchronizedInputChannel::channelName()
{
  QString      res = "Unknown Input";
  QxrdNIDAQPtr nidaq(m_NIDAQ);

  if (nidaq) {
    res = nidaq->inputDeviceName(get_ChannelNumber());
  }

  return res;
}

QString QxrdSynchronizedInputChannel::channelMode(int n)
{
  QString      res = "Unknown Mode";

  switch(n) {
  case ModeSummed:
    res = "Summed";
    break;

  case ModeAveraged:
    res = "Averaged";
    break;

  case ModeMaximum:
    res = "Maximum Value";
    break;

  case ModeMinimum:
    res = "Minimum Value";
    break;
  }

  return res;
}

int QxrdSynchronizedInputChannel::channelModeCount()
{
  return ModeCount;
}

//TODO: implement
double QxrdSynchronizedInputChannel::evaluateInput()
{
  return 0;
}
