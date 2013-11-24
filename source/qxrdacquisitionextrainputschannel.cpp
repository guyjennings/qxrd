#include "qxrdacquisitionextrainputschannel.h"
#include "qxrdacquisitionextrainputs.h"
#include "qxrdmutexlocker.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdsettingssaver.h"

QxrdAcquisitionExtraInputsChannel::QxrdAcquisitionExtraInputsChannel(
    int chnum, QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QxrdAcquisitionExtraInputsWPtr xtra) :
  QObject(),
  m_ObjectNamer(this, tr("extraChannel(%1)").arg(chnum)),
  m_ChannelNumber(QxrdSettingsSaverPtr(), this, "channelNumber", chnum, "Extra Input Channel Number"),
  m_Enabled(saver, this, "enabled", 1, "Enabled?"),
  m_Plotted(saver, this, "plotted", 1, "Plotted?"),
  m_ChannelName(saver, this, "channelName", "", "NIDAQ name of channel"),
//  m_Kind(saver, this, "kind", 0, "Channel Kind (0 = none, 1 = Analog In, 2 = Counter In)"),
  m_Mode(saver, this, "mode", 0, "Channel Mode (0 = summed, 1 = averaged, 2 = maximum, 3 = minimum)"),
  m_SaveWave(saver, this, "saveWave", 0, "Save entire waveform (0 = no, 1 = yes)"),
  m_Min(saver, this, "min", -10.0, "Minimum Input value for Analog Channel (in Volts)"),
  m_Max(saver, this, "max", 10.0, "Maximum Input Value for Analog Channel (in Volts)"),
  m_Start(saver, this, "start", 0.0, "Start Offset for Channel (in sec after notional exposure start)\n"
                                 "i.e. Negative values mean times before start of exposure"),
  m_End(saver, this, "end", 0.0, "End Offset for Channel (in sec before notional exposure end)\n"
                                 "i.e. Negative values mean times after end of exposure"),
  m_TriggerMode(saver, this, "triggerMode", 0, "Trigger Mode (0 = None, 1 = +Edge, 2 = -Edge, 3 = +Level, 4 = -Level)"),
  m_TriggerLevel(saver, this, "triggerLevel", 0.0, "Trigger Level (in Volts)"),
  m_TriggerHysteresis(saver, this, "triggerHysteresis", 0.0, "Trigger Hysteresis (in Volts)"),
  m_PhysicalChannel(QxrdSettingsSaverPtr(), this, "physicalChannel", 0, "Physical Channel Number"),
  m_Value(QxrdSettingsSaverWPtr(), this, "value", 0.0, "Current Value of Channel"),
  m_Triggered(QxrdSettingsSaverWPtr(), this, "triggered", 0, "Was channel triggered?"),
  m_NLow(QxrdSettingsSaverWPtr(), this, "nLow", 0, "Number of untriggered data points"),
  m_NHigh(QxrdSettingsSaverWPtr(), this, "nHigh", 0, "Number of triggered data points"),
  m_Waveform(QxrdSettingsSaverWPtr(), this, "waveform", QcepDoubleVector(), "Waveform on Channel"),
  m_Experiment(doc),
  m_ExtraInputs(xtra)
{
  connect(prop_ChannelName(), SIGNAL(valueChanged(QString,int)), this, SIGNAL(reinitiateNeeded()));
  connect(prop_Min(),         SIGNAL(valueChanged(double,int)),  this, SIGNAL(reinitiateNeeded()));
  connect(prop_Max(),         SIGNAL(valueChanged(double,int)),  this, SIGNAL(reinitiateNeeded()));
  connect(prop_Enabled(),     SIGNAL(valueChanged(bool,int)),    this, SIGNAL(reinitiateNeeded()));
}

void QxrdAcquisitionExtraInputsChannel::readSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdAcquisitionExtraInputsChannel::writeSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

QVector<double> QxrdAcquisitionExtraInputsChannel::readChannel()
{
  QxrdAcquisitionExtraInputsPtr xtra(m_ExtraInputs);

  if (xtra) {
    QxrdNIDAQPluginInterfacePtr nidaq = xtra->nidaqPlugin();

    if (nidaq) {
      return nidaq->readContinuousInputChannel(get_PhysicalChannel());
    }
  }

  return QVector<double>();
}

int QxrdAcquisitionExtraInputsChannel::startIndex()
{
  int res = 0;

  QxrdAcquisitionExtraInputsPtr xtra(m_ExtraInputs);

  if (xtra) {
    QxrdNIDAQPluginInterfacePtr nidaq = xtra->nidaqPlugin();

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

  QxrdAcquisitionExtraInputsPtr xtra(m_ExtraInputs);

  if (xtra) {
    QxrdNIDAQPluginInterfacePtr nidaq = xtra->nidaqPlugin();

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
