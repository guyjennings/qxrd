#include "qxrdacquisitionextrainputschannel.h"
#include "qxrdacquisitionextrainputs.h"
#include "qcepmutexlocker.h"
#include "qxrdnidaq.h"

QxrdAcquisitionExtraInputsChannelPtr QxrdAcquisitionExtraInputsChannel::newAcquisitionExtraInputsChannel(int chnum)
{
  QxrdAcquisitionExtraInputsChannelPtr chan(new QxrdAcquisitionExtraInputsChannel(tr("extraChannel")/*.arg(chnum)*/));

  chan -> set_ChannelNumber(chnum);

  return chan;
}

QxrdAcquisitionExtraInputsChannel::QxrdAcquisitionExtraInputsChannel(QString name) :
  inherited(name),
  m_ChannelNumber(this, "channelNumber", -1, "Extra Input Channel Number"),
  m_Enabled(this, "enabled", 1, "Enabled?"),
  m_Plotted(this, "plotted", 1, "Plotted?"),
  m_ChannelName(this, "channelName", "", "NIDAQ name of channel"),
  //  m_Kind(this, "kind", 0, "Channel Kind (0 = none, 1 = Analog In, 2 = Counter In)"),
  m_Mode(this, "mode", 0, "Channel Mode (0 = summed, 1 = averaged, 2 = maximum, 3 = minimum)"),
  m_SaveWave(this, "saveWave", 0, "Save entire waveform (0 = no, 1 = yes)"),
  m_Min(this, "min", -10.0, "Minimum Input value for Analog Channel (in Volts)"),
  m_Max(this, "max", 10.0, "Maximum Input Value for Analog Channel (in Volts)"),
  m_Start(this, "start", 0.0, "Start Offset for Channel (in sec after notional exposure start)\n"
                              "i.e. Negative values mean times before start of exposure"),
  m_End(this, "end", 0.0, "End Offset for Channel (in sec before notional exposure end)\n"
                          "i.e. Negative values mean times after end of exposure"),
  m_TriggerMode(this, "triggerMode", 0, "Trigger Mode (0 = None, 1 = +Edge, 2 = -Edge, 3 = +Level, 4 = -Level)"),
  m_TriggerLevel(this, "triggerLevel", 0.0, "Trigger Level (in Volts)"),
  m_TriggerHysteresis(this, "triggerHysteresis", 0.0, "Trigger Hysteresis (in Volts)"),
  m_PhysicalChannel(this, "physicalChannel", 0, "Physical Channel Number"),
  m_Value(this, "value", 0.0, "Current Value of Channel"),
  m_Triggered(this, "triggered", 0, "Was channel triggered?"),
  m_NLow(this, "nLow", 0, "Number of untriggered data points"),
  m_NHigh(this, "nHigh", 0, "Number of triggered data points"),
  m_Waveform(this, "waveform", QcepDoubleVector(), "Waveform on Channel")
{
#ifndef QT_NO_DEBUG
  printf("Constructing acquisition extra inputs channel\n");
#endif

  connect(prop_ChannelName(), &QcepStringProperty::valueChanged,
          this, &QxrdAcquisitionExtraInputsChannel::reinitiateNeeded);

  connect(prop_Min(),         &QcepDoubleProperty::valueChanged,
          this, &QxrdAcquisitionExtraInputsChannel::reinitiateNeeded);

  connect(prop_Max(),         &QcepDoubleProperty::valueChanged,
          this, &QxrdAcquisitionExtraInputsChannel::reinitiateNeeded);

  connect(prop_Enabled(),     &QcepBoolProperty::valueChanged,
          this, &QxrdAcquisitionExtraInputsChannel::reinitiateNeeded);
}

QxrdAcquisitionExtraInputsWPtr QxrdAcquisitionExtraInputsChannel::extraInputs()
{
  if (parentPtr() == NULL) {
    printMessage("QxrdAcquisitionExtraInputsChannel Parent == NULL");
    return QxrdAcquisitionExtraInputsPtr();
  } else {
    return qSharedPointerDynamicCast<QxrdAcquisitionExtraInputs>(parentPtr());
  }
}

void QxrdAcquisitionExtraInputsChannel::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);
}

void QxrdAcquisitionExtraInputsChannel::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);
}

QVector<double> QxrdAcquisitionExtraInputsChannel::readChannel()
{
  QxrdAcquisitionExtraInputsPtr xtra(extraInputs());

  if (xtra) {
    QxrdNIDAQPtr nidaq(xtra->nidaqPlugin());

    if (nidaq) {
      return nidaq->readContinuousInputChannel(get_PhysicalChannel());
    }
  }

  return QVector<double>();
}

int QxrdAcquisitionExtraInputsChannel::startIndex()
{
  int res = 0;

  QxrdAcquisitionExtraInputsPtr xtra(extraInputs());

  if (xtra) {
    QxrdNIDAQPtr nidaq(xtra->nidaqPlugin());

    if (nidaq) {
      int nSamples = nidaq->countContinuousInput();
      int startOffset = xtra->get_SampleRate()*(0.1 + get_Start());

      if (startOffset > nSamples) {
        res = nSamples;
      } else if (startOffset > 0) {
        res = startOffset;
      }
    }
  }

  return res;
}

int QxrdAcquisitionExtraInputsChannel::endIndex()
{
  int res = 0;

  QxrdAcquisitionExtraInputsPtr xtra(extraInputs());

  if (xtra) {
    QxrdNIDAQPtr nidaq(xtra->nidaqPlugin());

    if (nidaq) {
      int nSamples = nidaq->countContinuousInput();
      int endOffset = xtra->get_SampleRate()*(0.1 + xtra->get_ExposureTime() - get_End());

      if (endOffset > nSamples) {
        res = nSamples;
      } else if (endOffset > 0) {
        res = endOffset;
      }
    }
  }

  return res;
}

double          QxrdAcquisitionExtraInputsChannel::evaluateChannel()
{
  evaluateTrigger();

  int mode = get_Mode();

  switch (mode) {
  case ModeSummed:
  default:
    return sumChannel();

  case ModeAveraged:
    return averageChannel();

  case ModeMaximum:
    return maximumChannel();

  case ModeMinimum:
    return minimumChannel();
  }
}

double          QxrdAcquisitionExtraInputsChannel::sumChannel()
{
  QVector<double> res = readChannel();

  double sum=0;

  int i0 = startIndex();
  int i1 = endIndex();

  for(int i=i0; i<i1; i++) {
    sum += res[i];
  }

  return sum;
}

double          QxrdAcquisitionExtraInputsChannel::averageChannel()
{
  QVector<double> res = readChannel();

  double n=0;
  double sum=0;

  int i0 = startIndex();
  int i1 = endIndex();

  for(int i=i0; i<i1; i++) {
    sum += res[i];
    n   += 1;
  }

  return (n>0 ? sum/n : 0);
}

double          QxrdAcquisitionExtraInputsChannel::maximumChannel()
{
  QVector<double> res = readChannel();

  double max=0;

  int i0 = startIndex();
  int i1 = endIndex();

  for(int i=i0; i<i1; i++) {
    double v=res[i];

    if (i == i0) {
      max = v;
    } else {
      if (v > max) {
        max = v;
      }
    }
  }

  return max;
}

double          QxrdAcquisitionExtraInputsChannel::minimumChannel()
{
  QVector<double> res = readChannel();

  double min=0;

  int i0 = startIndex();
  int i1 = endIndex();

  for(int i=i0; i<i1; i++) {
    double v=res[i];

    if (i == i0) {
      min = v;
    } else {
      if (v < min) {
        min = v;
      }
    }
  }

  return min;
}

bool QxrdAcquisitionExtraInputsChannel::evaluateTrigger()
{
  int trigMode = get_TriggerMode();
  bool res;

  switch (trigMode) {
  case TriggerModeNone:
  default:
    res = false;
    break;

  case TriggerModeEdgePos:
    res = evalTrig(+1, true);
    break;

  case TriggerModeEdgeNeg:
    res = evalTrig(-1, true);
    break;

  case TriggerModeLevelPos:
    res = evalTrig(+1, false);
    break;

  case TriggerModeLevelNeg:
    res = evalTrig(-1, false);
    break;
  }

  set_Triggered(res);

  return res;
}

bool QxrdAcquisitionExtraInputsChannel::evalTrig(int polarity, bool edgeTrig)
{
  double level = get_TriggerLevel();
  double hyst  = get_TriggerHysteresis();
  bool tres;

  QVector<double> res = readChannel();

  int nlow=0, nhigh=0;
  double tlevel = level*polarity;
  double lowlevel = tlevel-fabs(hyst);
  double highlevel = tlevel+fabs(hyst);

  int i0 = startIndex();
  int i1 = endIndex();

  for(int i=i0; i<i1; i++) {
    double v=res[i]*polarity;

    if (v < lowlevel)  nlow  += 1;

    if (edgeTrig) {
      if (nlow && (v > highlevel)) nhigh += 1;
    } else {
      if (v > highlevel) nhigh += 1;
    }
  }

  set_NLow(nlow);
  set_NHigh(nhigh);

  if (edgeTrig) {
    tres = ((nlow > 0) && (nhigh > 0));
  } else {
    tres = (nhigh > 0);
  }

  return tres;
}
