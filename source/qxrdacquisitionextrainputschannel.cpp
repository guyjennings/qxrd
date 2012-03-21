#include "qxrdacquisitionextrainputschannel.h"
#include "qxrdacquisitionextrainputs.h"
#include "qxrdmutexlocker.h"

QxrdAcquisitionExtraInputsChannel::QxrdAcquisitionExtraInputsChannel(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QxrdAcquisitionExtraInputsWPtr xtra) :
  QObject(),
  m_Experiment(doc),
  m_ExtraInputs(xtra),
  m_Enabled(saver, this, "enabled", 0, "Enabled?"),
  m_Channel(saver, this, "channel", "", "NIDAQ name of channel"),
  m_Kind(saver, this, "kind", 0, "Channel Kind (0 = none, 1 = Analog In, 2 = Counter In)"),
  m_Mode(saver, this, "mode", 0, "Channel Mode (0 = summed, 1 = averaged, 2 = maximum, 3 = minimum)"),
  m_SaveWave(saver, this, "saveWave", 0, "Save entire waveform (0 = no, 1 = yes)"),
  m_Min(saver, this, "min", -10.0, "Minimum Input value for Analog Channel (in Volts)"),
  m_Max(saver, this, "max", 10.0, "Maximum Input Value for Analog Channel (in Volts)"),
  m_Start(saver, this, "start", 0.0, "Start Offset for Channel (in sec after notional exposure start)\n"
                                 "i.e. Negative values mean times before start of exposure"),
  m_End(saver, this, "end", 0.0, "End Offset for Channel (in sec before notional exposure end)\n"
                                 "i.e. Negative values mean times after end of exposure"),
  m_Value(QxrdSettingsSaverWPtr(), this, "value", 0.0, "Current Value of Channel"),
  m_Waveform(QxrdSettingsSaverWPtr(), this, "waveform", QcepDoubleVector(), "Waveform on Channel")
{
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

