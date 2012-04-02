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
  m_PhysicalChannel(QxrdSettingsSaverPtr(), this, "physicalChannel", 0, "Physical Channel Number"),
  m_Value(QxrdSettingsSaverWPtr(), this, "value", 0.0, "Current Value of Channel"),
  m_Waveform(QxrdSettingsSaverWPtr(), this, "waveform", QcepDoubleVector(), "Waveform on Channel"),
  m_Experiment(doc),
  m_ExtraInputs(xtra)
{
  connect(prop_ChannelName(), SIGNAL(valueChanged(QString,int)), this, SIGNAL(reinitializeNeeded()));
  connect(prop_Min(),         SIGNAL(valueChanged(double,int)),  this, SIGNAL(reinitializeNeeded()));
  connect(prop_Max(),         SIGNAL(valueChanged(double,int)),  this, SIGNAL(reinitializeNeeded()));
  connect(prop_Enabled(),     SIGNAL(valueChanged(bool,int)),    this, SIGNAL(reinitializeNeeded()));
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

