#include "qxrdacquisitiontrigger.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdmutexlocker.h"
#include "qxrdacquisition.h"
#include "qwt_math.h"
#include "qxrdsettingssaver.h"

QxrdAcquisitionTrigger::QxrdAcquisitionTrigger(QxrdSettingsSaverPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QObject(),
  m_TriggerSync(saver, this, "triggerSync", TriggerSyncNone),
  m_TriggerAMode(saver, this,"triggerAMode", TriggerModeNone),
  m_TriggerACard(saver, this,"triggerACard", TriggerCardNone),
  m_TriggerAChannel(saver, this,"triggerAChannel", TriggerChannelNone),
  m_TriggerASlope(saver, this,"triggerASlope", TriggerSlopePositive),
  m_TriggerALevel(saver, this,"triggerALevel", 1.5),
  m_TriggerAHysteresis(saver, this,"triggerAHysteresis", 0.5),
  m_TriggerAValue(QxrdSettingsSaverPtr(), this,"triggerAValue", 0),
  m_TriggerAPrevValue(QxrdSettingsSaverPtr(), this,"triggerAPrevValue", 0),
  m_TriggerATriggered(QxrdSettingsSaverPtr(), this,"triggerATriggered", 0),
  m_TriggerAChannelName(saver, this, "triggerAChannelName", ""),
  m_TriggerBMode(saver, this,"triggerBMode", TriggerModeNone),
  m_TriggerBCard(saver, this,"triggerBCard", TriggerCardNone),
  m_TriggerBChannel(saver, this,"triggerBChannel", TriggerChannelNone),
  m_TriggerBSlope(saver, this,"triggerBSlope", TriggerSlopePositive),
  m_TriggerBLevel(saver, this,"triggerBLevel", 1.5),
  m_TriggerBHysteresis(saver, this,"triggerBHysteresis", 0.5),
  m_TriggerBValue(QxrdSettingsSaverPtr(), this,"triggerBValue", 0),
  m_TriggerBPrevValue(QxrdSettingsSaverPtr(), this,"triggerBPrevValue", 0),
  m_TriggerBTriggered(QxrdSettingsSaverPtr(), this,"triggerBTriggered", 0),
  m_TriggerBChannelName(saver, this, "triggerBChannelName", ""),
  m_Experiment(expt),
  m_Acquisition(acq)
{
  startTimer(200);
}

void QxrdAcquisitionTrigger::readSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdAcquisitionTrigger::writeSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdAcquisitionTrigger::timerEvent(QTimerEvent *)
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    QxrdNIDAQPluginInterfacePtr nidaq(acqp->nidaqPlugin());

    if (nidaq) {
      QString aChanName = get_TriggerAChannelName();
      QString bChanName = get_TriggerBChannelName();

      if (aChanName.length()) {
        set_TriggerAValue(nidaq->getAnalogInput(aChanName));
      }

      if (bChanName.length()) {
        set_TriggerBValue(nidaq->getAnalogInput(bChanName));
      }
    }
  }
}

int QxrdAcquisitionTrigger::checkTriggerA()
{
  return get_TriggerATriggered();
}

int QxrdAcquisitionTrigger::checkTriggerB()
{
  return get_TriggerBTriggered();
}

