#include "qxrdnidaqsyncwaveformoutput.h"

QxrdNIDAQSyncWaveformOutput::QxrdNIDAQSyncWaveformOutput(int style, double v0, double v1)
  : inherited("waveformOutput"),
    m_WaveformStyle (this, "waveformStyle", style, "Waveform Style"),
    m_WaveformStartV(this, "waveformStartV", v0, "Waveform Start Voltage"),
    m_WaveformEndV  (this, "waveformEndV", v1, "Waveform End Voltage")
{

}
