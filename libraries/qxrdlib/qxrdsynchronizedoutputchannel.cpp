#include "qxrdsynchronizedoutputchannel.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdnidaq.h"

QxrdSynchronizedOutputChannel::QxrdSynchronizedOutputChannel(QString name)
  : inherited(name),
  m_ChannelNumber(this, "channelNumber", -1, "NIDAQ channel number"),
  m_SignalName(this, "signalName", "", "NIDAQ name of channel"),
  m_Mode(this, "mode", ModeContinuous, "Channel Mode (0 = fixed, 1 = stepped, 2 = continuous)"),
  m_WaveformMode(this, "waveformMode", WaveformSquare,
                 "Waveform Type (0 = Square, 1 = Sine, 2 = Triangle, 3 = Ramp, 4 = Bipolar Triangle)"),
  m_StartV(this, "startV", -10.0, "Starting value for output (in Volts)"),
  m_EndV(this, "endV", 10.0, "Ending value for output (in Volts)"),
  m_Symmetry(this, "symmetry", 0.0, "Waveform Symmetry (0 = symmetric)"),
  m_PhaseShift(this, "phaseShift", 0.0, "Waveform Phase Shift (deg)"),
  m_SampleRate(this, "sampleRate", 1000, "Sampling rate of analog channel (in Hz)"),
  m_Waveform(this, "waveform", QcepDoubleVector(), "Waveform on Channel")
{
}

void QxrdSynchronizedOutputChannel::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  QxrdSynchronizedAcquisitionPtr acq(
        qSharedPointerDynamicCast<QxrdSynchronizedAcquisition>(parent));

  if (acq) {
    m_NIDAQ = acq->nidaqPlugin();
  } else {
    printMessage("QxrdSynchronizedOutputChannel::initialize parent not QxrdSynchronizedAcquisition");
  }
}

QString QxrdSynchronizedOutputChannel::channelName()
{
  QString      res = "Unknown Input";
  QxrdNIDAQPtr nidaq(m_NIDAQ);

  if (nidaq) {
    res = nidaq->outputDeviceName(get_ChannelNumber());
  }

  return res;
}

int QxrdSynchronizedOutputChannel::channelModeCount()
{
  return ModeCount;
}

QString QxrdSynchronizedOutputChannel::channelMode(int n)
{
  QString res = "Unknown Mode";

  switch (n) {
  case ModeNone:
    res = "Constant Output";
    break;

  case ModeStepped:
    res = "Stepped Output";
    break;

  case ModeContinuous:
    res = "Continuous Output";
    break;
  }

  return res;
}

int QxrdSynchronizedOutputChannel::waveformModeCount()
{
  return WaveformCount;
}

QString QxrdSynchronizedOutputChannel::waveformMode(int n)
{
  QString res = "Unknown Waveform";

  switch (n) {
  case WaveformSquare:
    res = "Square Wave";
    break;

  case WaveformSine:
    res = "Sine Wave";
    break;

  case WaveformRamp:
    res = "Ramp Wave";
    break;

  case WaveformTriangle:
    res = "Triangle Wave";
    break;

  case WaveformBipolarTriangle:
    res = "Bipolar Triangle Wave";
    break;
  }

  return res;
}
