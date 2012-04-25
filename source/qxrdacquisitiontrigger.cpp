#include "qxrddebug.h"
#include "qxrdacquisitiontrigger.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdmutexlocker.h"
#include "qxrdacquisition.h"
#include "qwt_math.h"
#include "qxrdsettingssaver.h"
#include "qxrdexperiment.h"

QxrdAcquisitionTrigger::QxrdAcquisitionTrigger(QxrdSettingsSaverPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QObject(),
  m_ObjectNamer(this, "trigger"),
  m_TriggerSync(saver, this, "triggerSync", TriggerSyncNone, "Trigger Synchronization (0 = None, 1 = Sync, 2 = Async)"),
  m_TriggerAMode(saver, this,"triggerAMode", TriggerModeNone, "Trigger A Mode (0 = None, 1 = +Edge, 2 = -Edge, 3 = +Level, 4 = -Level)"),
  m_TriggerACard(saver, this,"triggerACard", TriggerCardNone, "Trigger A Card Number"),
  m_TriggerAChannel(saver, this,"triggerAChannel", TriggerChannelNone, "Trigger A Channel"),
  m_TriggerALevel(saver, this,"triggerALevel", 1.5, "Trigger A Level (in Volts)"),
  m_TriggerAHysteresis(saver, this,"triggerAHysteresis", 0.5, "Trigger A Hysteresis (in Volts)"),
  m_TriggerAValue(QxrdSettingsSaverPtr(), this,"triggerAValue", 0, "Trigger A Value (in Volts)"),
  m_TriggerAPrevValue(QxrdSettingsSaverPtr(), this,"triggerAPrevValue", 0, "Trigger A Previous Value (in Volts)"),
  m_TriggerATriggered(QxrdSettingsSaverPtr(), this,"triggerATriggered", false, "Trigger A Triggered?"),
  m_TriggerAChannelName(saver, this, "triggerAChannelName", "", "Trigger A NIDAQ Channel Name"),
  m_TriggerBMode(saver, this,"triggerBMode", TriggerModeNone, "Trigger B Mode (0 = None, 1 = +Edge, 2 = -Edge, 3 = +Level, 4 = -Level)"),
  m_TriggerBCard(saver, this,"triggerBCard", TriggerCardNone, "Trigger B Card Number"),
  m_TriggerBChannel(saver, this,"triggerBChannel", TriggerChannelNone, "Trigger B Channel"),
  m_TriggerBLevel(saver, this,"triggerBLevel", 1.5, "Trigger B Level (in Volts)"),
  m_TriggerBHysteresis(saver, this,"triggerBHysteresis", 0.5, "Trigger B Hysteresis (in Volts)"),
  m_TriggerBValue(QxrdSettingsSaverPtr(), this,"triggerBValue", 0, "Trigger B Value (in Volts)"),
  m_TriggerBPrevValue(QxrdSettingsSaverPtr(), this,"triggerBPrevValue", 0, "Trigger B Previous Value (in Volts)"),
  m_TriggerBTriggered(QxrdSettingsSaverPtr(), this,"triggerBTriggered", false, "Trigger B Triggered?"),
  m_TriggerBChannelName(saver, this, "triggerBChannelName", "", "Trigger B NIDAQ Channel Name"),
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

  set_TriggerATriggered(
        evaluateTrigger(
          get_TriggerATriggered(),
          get_TriggerAMode(),
          get_TriggerALevel(),
          get_TriggerAHysteresis(),
          get_TriggerAValue(),
          get_TriggerAPrevValue()));

  set_TriggerBTriggered(
        evaluateTrigger(
          get_TriggerBTriggered(),
          get_TriggerBMode(),
          get_TriggerBLevel(),
          get_TriggerBHysteresis(),
          get_TriggerBValue(),
          get_TriggerBPrevValue()));

  set_TriggerAPrevValue(get_TriggerAValue());
  set_TriggerBPrevValue(get_TriggerBValue());
}

static int firstTime = true;

int QxrdAcquisitionTrigger::evaluateTrigger(
    int wasTriggered, int mode, double level, double hysteresis, double value, double prevvalue)
{

  int res = true;

  switch (mode) {
  case TriggerModeNone:
    res = true;
    break;

  case TriggerModeEdgePos:
    res = value > level;
    break;

  case TriggerModeEdgeNeg:
    res = value < level;
    break;

  case TriggerModeLevelPos:
    res = value > level;
    break;

  case TriggerModeLevelNeg:
    res = value < level;
    break;

  default:
    break;
  }

  if (qcepDebug(DEBUG_TRIGGER)) {
    if (firstTime || (res != wasTriggered)) {
      firstTime = false;

      QxrdExperimentPtr expt(m_Experiment);

      if (expt) {
        expt->printMessage(tr("Trig: %1<-%2 M:%3 L:%4 H:%5 V:%6 P:%7")
                           .arg(res).arg(wasTriggered)
                           .arg(mode).arg(level).arg(hysteresis)
                           .arg(value).arg(prevvalue));
      }
    }
  }

  return res;
}

int QxrdAcquisitionTrigger::checkTriggerA()
{
  return get_TriggerATriggered();
}

int QxrdAcquisitionTrigger::checkTriggerB()
{
  return get_TriggerBTriggered();
}

