#include "qxrdsynchronizedoutputchannel.h"
#include "qxrdacqcommon.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdacquisitionparameterpack.h"
#include "qxrddarkacquisitionparameterpack.h"
#include "qwt_math.h"
#include "qxrdsynchronizer.h"

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
  m_NSamples(this, "nSamples", 0, "Number of samples in waveform"),
  m_TimeValues(this, "timeValues", QcepDoubleVector(), "Time Values on Channel"),
  m_Waveform(this, "waveform", QcepDoubleVector(), "Waveform on Channel"),
  m_Enabled(this, "enabled", false, "Enable Output Waveform?")
{
}

void QxrdSynchronizedOutputChannel::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_AcqCommon = QxrdAcqCommon::findAcquisition(parent);

  QxrdAcqCommonPtr acq(m_AcqCommon);

  if (acq) {
    m_SynchronizedAcquisition = acq->synchronizedAcquisition();
    m_Synchronizer            = acq->synchronizer();
  }
}

QString QxrdSynchronizedOutputChannel::channelName()
{
  QString      res = "Unknown Input";
  QxrdSynchronizerPtr sync(m_Synchronizer);

  if (sync) {
    res = sync->outputDeviceName(get_ChannelNumber());
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

void QxrdSynchronizedOutputChannel::disableWaveform()
{
  set_Enabled(false);
}

void QxrdSynchronizedOutputChannel::recalculateWaveform(
    QxrdSynchronizedAcquisitionWPtr  s,
    QxrdAcquisitionParameterPackWPtr p)
{
  QxrdAcquisitionParameterPackPtr parms(p);
  QxrdSynchronizedAcquisitionPtr  sync(s);

  if (parms && sync) {
    double exposureTime = parms->exposure();
    int    nPhases      = parms->nphases();
    double cycleTime    = exposureTime*nPhases;
    double sampleRate   = sync -> get_OutputSampleRate();
    double nSamples     = cycleTime*sampleRate;
    double minVal       = get_StartV();
    double maxVal       = get_EndV();
    double symm         = get_Symmetry();
    double phase        = get_PhaseShift();
    int    wfm          = get_WaveformMode();

    if (symm > 1.0) {
      symm = 1.0;
    } else if (symm < -1.0) {
      symm = -1.0;
    } else if (symm != symm) { //NaN?
      symm = 0.0;
    }

    if (nPhases <= 0) {
      printMessage(tr("Output Channel nPhases (%1) <= 0").arg(nPhases));
    } else if (exposureTime <= 0) {
      printMessage(tr("Output Channel exposureTime (%1 sec) <= 0.0").arg(exposureTime));
    } else if (sampleRate <= 0) {
      printMessage(tr("Output Channel sampleRate (%1 Hz) <= 0").arg(sampleRate));
    } else if (minVal < -10.0 || minVal > 10.0) {
      printMessage(tr("Output Channel startV (%1 V) outside [-10.0..10.0]").arg(minVal));
    } else if (maxVal < -10.0 || maxVal > 10.0) {
      printMessage(tr("Output Channel endV (%1 V) outside [-10.0 V .. 10.0 V]").arg(minVal));
    } else if (phase < -450 || phase > 450) {
      printMessage(tr("Output Channel phase (%1 deg) outside [-450.0 deg .. 450.0 deg]"));
    } else if (wfm < 0 || wfm >= WaveformCount) {
      printMessage(tr("Output Channel unknown waveform (%1)").arg(wfm));
    } else if (nSamples <= 0) {
      printMessage(tr("Output Channel nSamples (%1) <= 0").arg(nSamples));
    } else {
//      if (nSamples > 1000000) {
//        while (nSamples > 1000000) {
//          sampleRate /= 10;
//          nSamples    = cycleTime*sampleRate;
//        }

//        printMessage(tr("Too many samples - sampleRate reduced to %1 Hz, now %2 samples").arg(sampleRate).arg(nSamples));
//      }

      int iSamples = (int) nSamples;
      double divide = iSamples * (0.5 + symm/2.0);
      double divideBy2 = divide/2.0;
      int shift = (int)((double) phase*iSamples/360.0 /*+ nPhases*/) % iSamples;

      QVector<double> outputTimes(iSamples+1);
      QVector<double> outputVoltage(iSamples+1);

      for (int i=0; i<=iSamples; i++) {
        outputTimes[i] = ((double)i)/((double) sampleRate);
      }

      switch (wfm) {
      case WaveformSquare:
        for (int ii=0; ii<iSamples; ii++) {
          int i = (ii+iSamples-shift) % iSamples;
          if (i<divide) {
            outputVoltage[ii] = minVal;
          } else {
            outputVoltage[ii] = maxVal;
          }
        }
        break;

      case WaveformSine:
        for (int ii=0; ii<iSamples; ii++) {
          int i = (ii+iSamples-shift) % iSamples;
          double x;
          if (i<divide) {
            x = M_PI*i/divide;
          } else {
            x = M_PI+M_PI*(i-divide)/(iSamples-divide);
          }
          outputVoltage[ii] = minVal + (maxVal-minVal)*(1.0 - cos(x))/2.0;
        }
        break;

      case WaveformTriangle:
        for (int ii=0; ii<iSamples; ii++) {
          int i = (ii+iSamples-shift) % iSamples;
          if (i<divide) {
            outputVoltage[ii] = minVal + i*(maxVal-minVal)/divide;
          } else {
            outputVoltage[ii] = maxVal - (i-divide)*(maxVal-minVal)/(iSamples-divide);
          }
        }
        break;

      case WaveformBipolarTriangle:
        for (int ii=0; ii<iSamples; ii++) {
          int i = (ii+iSamples-shift) % iSamples;
          if (i < divideBy2) {
            outputVoltage[ii] = minVal + i*(maxVal-minVal)/divideBy2;
          } else if (i < (iSamples-divideBy2)) {
            outputVoltage[ii] = maxVal - (i-divideBy2)*(maxVal-minVal)/((iSamples-divide)/2);
          } else {
            outputVoltage[ii] = minVal - (iSamples-i)*(maxVal-minVal)/divideBy2;
          }
        }
        break;

      case WaveformRamp:
      default:
        for (int ii=0; ii<iSamples; ii++) {
          int i = (ii+iSamples-shift) % iSamples;
          outputVoltage[ii] = minVal + i*(maxVal-minVal)/iSamples;
        }
        break;
      }

      outputVoltage[iSamples] = outputVoltage[0];

      set_Enabled(true);

      set_TimeValues(outputTimes);
      set_Waveform(outputVoltage);
      set_NSamples(iSamples);
    }
  }
}
