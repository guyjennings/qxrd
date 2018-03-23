#include "qxrdsynchronizedinputchannel.h"
#include "qxrdacqcommon.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizer.h"
#include "qxrdacquisitionparameterpack.h"
#include "qxrddarkacquisitionparameterpack.h"

QxrdSynchronizedInputChannel::QxrdSynchronizedInputChannel(QString name)
  : inherited(name),
  m_ChannelNumber(this, "channelNumber", -1, "Extra Input Channel Number"),
  m_SignalName(this, "signalName", "", "NIDAQ name of channel"),
  m_Mode(this, "mode", ModeAveraged, "Channel Mode (0 = summed, 1 = averaged, 2 = maximum, 3 = minimum)"),
  m_SaveWave(this, "saveWave", 0, "Save entire waveform (0 = no, 1 = yes)"),
  m_Min(this, "min", -10.0, "Minimum Input value for Analog Channel (in Volts)"),
  m_Max(this, "max", 10.0, "Maximum Input Value for Analog Channel (in Volts)"),
  m_NSamples(this, "nSamples", 0, "Number of samples in waveform"),
  m_TimeValues(this, "timeValues", QcepDoubleVector(), "Time Values on Channel"),
  m_Waveform(this, "waveform", QcepDoubleVector(), "Waveform on Channel")
{
}

QxrdSynchronizedInputChannel::~QxrdSynchronizedInputChannel()
{
  printMessage("QxrdSynchronizedInputChannel::~QxrdSynchronizedInputChannel");
}

void QxrdSynchronizedInputChannel::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_AcqCommon = QxrdAcqCommon::findAcquisition(parent);

  QxrdAcqCommonPtr acq(m_AcqCommon);

  if (acq) {
    m_SynchronizedAcquisition = acq->synchronizedAcquisition();
    m_Synchronizer            = acq->synchronizer();
  }
}

QString QxrdSynchronizedInputChannel::channelName()
{
  QString             res = "Unknown Input";
  QxrdSynchronizerPtr sync(m_Synchronizer);

  if (sync) {
    res = sync->inputDeviceName(get_ChannelNumber());
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

void QxrdSynchronizedInputChannel::prepareForInput(QxrdSynchronizedAcquisitionWPtr  s,
                                                   QxrdAcquisitionParameterPackWPtr p)
{
  QxrdAcquisitionParameterPackPtr parms(p);
  QxrdSynchronizedAcquisitionPtr  sync(s);

  if (parms && sync) {
    double exposureTime = parms->exposure();
    int    nPhases      = parms->nphases();
    double cycleTime    = exposureTime*nPhases;
    double sampleRate   = sync -> get_InputSampleRate();
    double nSamples     = cycleTime*sampleRate;

//    if (nSamples > 1000000) {
//      while (nSamples > 1000000) {
//        sampleRate /= 10;
//        nSamples    = cycleTime*sampleRate;
//      }

//      printMessage(tr("Too many samples - sampleRate reduced to %1 Hz, now %2 samples").arg(sampleRate).arg(nSamples));
//    }

    int iSamples = (int) nSamples;

    QVector<double> inputTimes(iSamples+1);
    QVector<double> inputValues(iSamples+1);

    for (int i=0; i<=iSamples; i++) {
      inputTimes[i] = ((double)i)/((double) sampleRate);
      inputValues[i] = 0.0;
    }

    set_TimeValues(inputTimes);
    set_Waveform(inputValues);
    set_NSamples(iSamples);
  }
}

void QxrdSynchronizedInputChannel::prepareForDarkInput(QxrdSynchronizedAcquisitionWPtr      s,
                                                       QxrdDarkAcquisitionParameterPackWPtr p)
{
//  QxrdDarkAcquisitionParameterPackPtr parms(p);
//  QxrdSynchronizedAcquisitionPtr      sync(s);

//  if (parms && sync) {
//    set_ActualSampleRate(sync -> get_InputSampleRate());
//  }
}

//QVector<double> QxrdAcquisitionExtraInputsChannel::readChannel()
//{
//  QxrdAcquisitionExtraInputsPtr xtra(extraInputs());

//  if (xtra) {
//    QxrdNIDAQPtr nidaq(xtra->nidaqPlugin());

//    if (nidaq) {
//      return nidaq->readContinuousInputChannel(get_PhysicalChannel());
//    }
//  }

//  return QVector<double>();
//}

//int QxrdAcquisitionExtraInputsChannel::startIndex()
//{
//  int res = 0;

//  QxrdAcquisitionExtraInputsPtr xtra(extraInputs());

//  if (xtra) {
//    QxrdNIDAQPtr nidaq(xtra->nidaqPlugin());

//    if (nidaq) {
//      int nSamples = nidaq->countContinuousInput();
//      int startOffset = xtra->get_SampleRate()*(0.1 + get_Start());

//      if (startOffset > nSamples) {
//        res = nSamples;
//      } else if (startOffset > 0) {
//        res = startOffset;
//      }
//    }
//  }

//  return res;
//}

//int QxrdAcquisitionExtraInputsChannel::endIndex()
//{
//  int res = 0;

//  QxrdAcquisitionExtraInputsPtr xtra(extraInputs());

//  if (xtra) {
//    QxrdNIDAQPtr nidaq(xtra->nidaqPlugin());

//    if (nidaq) {
//      int nSamples = nidaq->countContinuousInput();
//      int endOffset = xtra->get_SampleRate()*(0.1 + xtra->get_ExposureTime() - get_End());

//      if (endOffset > nSamples) {
//        res = nSamples;
//      } else if (endOffset > 0) {
//        res = endOffset;
//      }
//    }
//  }

//  return res;
//}

//double          QxrdAcquisitionExtraInputsChannel::evaluateChannel()
//{
//  evaluateTrigger();

//  int mode = get_Mode();

//  switch (mode) {
//  case ModeSummed:
//  default:
//    return sumChannel();

//  case ModeAveraged:
//    return averageChannel();

//  case ModeMaximum:
//    return maximumChannel();

//  case ModeMinimum:
//    return minimumChannel();
//  }
//}

//double          QxrdAcquisitionExtraInputsChannel::sumChannel()
//{
//  QVector<double> res = readChannel();

//  double sum=0;

//  int i0 = startIndex();
//  int i1 = endIndex();

//  for(int i=i0; i<i1; i++) {
//    sum += res[i];
//  }

//  return sum;
//}

//double          QxrdAcquisitionExtraInputsChannel::averageChannel()
//{
//  QVector<double> res = readChannel();

//  double n=0;
//  double sum=0;

//  int i0 = startIndex();
//  int i1 = endIndex();

//  for(int i=i0; i<i1; i++) {
//    sum += res[i];
//    n   += 1;
//  }

//  return (n>0 ? sum/n : 0);
//}

//double          QxrdAcquisitionExtraInputsChannel::maximumChannel()
//{
//  QVector<double> res = readChannel();

//  double max=0;

//  int i0 = startIndex();
//  int i1 = endIndex();

//  for(int i=i0; i<i1; i++) {
//    double v=res[i];

//    if (i == i0) {
//      max = v;
//    } else {
//      if (v > max) {
//        max = v;
//      }
//    }
//  }

//  return max;
//}

//double          QxrdAcquisitionExtraInputsChannel::minimumChannel()
//{
//  QVector<double> res = readChannel();

//  double min=0;

//  int i0 = startIndex();
//  int i1 = endIndex();

//  for(int i=i0; i<i1; i++) {
//    double v=res[i];

//    if (i == i0) {
//      min = v;
//    } else {
//      if (v < min) {
//        min = v;
//      }
//    }
//  }

//  return min;
//}

//bool QxrdAcquisitionExtraInputsChannel::evaluateTrigger()
//{
//  int trigMode = get_TriggerMode();
//  bool res;

//  switch (trigMode) {
//  case TriggerModeNone:
//  default:
//    res = false;
//    break;

//  case TriggerModeEdgePos:
//    res = evalTrig(+1, true);
//    break;

//  case TriggerModeEdgeNeg:
//    res = evalTrig(-1, true);
//    break;

//  case TriggerModeLevelPos:
//    res = evalTrig(+1, false);
//    break;

//  case TriggerModeLevelNeg:
//    res = evalTrig(-1, false);
//    break;
//  }

//  set_Triggered(res);

//  return res;
//}

//bool QxrdAcquisitionExtraInputsChannel::evalTrig(int polarity, bool edgeTrig)
//{
//  double level = get_TriggerLevel();
//  double hyst  = get_TriggerHysteresis();
//  bool tres;

//  QVector<double> res = readChannel();

//  int nlow=0, nhigh=0;
//  double tlevel = level*polarity;
//  double lowlevel = tlevel-fabs(hyst);
//  double highlevel = tlevel+fabs(hyst);

//  int i0 = startIndex();
//  int i1 = endIndex();

//  for(int i=i0; i<i1; i++) {
//    double v=res[i]*polarity;

//    if (v < lowlevel)  nlow  += 1;

//    if (edgeTrig) {
//      if (nlow && (v > highlevel)) nhigh += 1;
//    } else {
//      if (v > highlevel) nhigh += 1;
//    }
//  }

//  set_NLow(nlow);
//  set_NHigh(nhigh);

//  if (edgeTrig) {
//    tres = ((nlow > 0) && (nhigh > 0));
//  } else {
//    tres = (nhigh > 0);
//  }

//  return tres;
//}
