#include "qxrdacquisitiontrigger.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdmutexlocker.h"
#include "qxrdacquisition.h"
#include "qwt_math.h"
#include "qxrdsettingssaver.h"
#include "qxrdexperiment.h"

QxrdAcquisitionTrigger::QxrdAcquisitionTrigger(QxrdSettingsSaverPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QObject(),
  m_TriggerSync(saver, this, "triggerSync", TriggerSyncNone),
  m_TriggerAMode(saver, this,"triggerAMode", TriggerModeNone),
  m_TriggerACard(saver, this,"triggerACard", TriggerCardNone),
  m_TriggerAChannel(saver, this,"triggerAChannel", TriggerChannelNone),
  m_TriggerALevel(saver, this,"triggerALevel", 1.5),
  m_TriggerAHysteresis(saver, this,"triggerAHysteresis", 0.5),
  m_TriggerAValue(QxrdSettingsSaverPtr(), this,"triggerAValue", 0),
  m_TriggerAPrevValue(QxrdSettingsSaverPtr(), this,"triggerAPrevValue", 0),
  m_TriggerATriggered(QxrdSettingsSaverPtr(), this,"triggerATriggered", false),
  m_TriggerAChannelName(saver, this, "triggerAChannelName", ""),
  m_TriggerBMode(saver, this,"triggerBMode", TriggerModeNone),
  m_TriggerBCard(saver, this,"triggerBCard", TriggerCardNone),
  m_TriggerBChannel(saver, this,"triggerBChannel", TriggerChannelNone),
  m_TriggerBLevel(saver, this,"triggerBLevel", 1.5),
  m_TriggerBHysteresis(saver, this,"triggerBHysteresis", 0.5),
  m_TriggerBValue(QxrdSettingsSaverPtr(), this,"triggerBValue", 0),
  m_TriggerBPrevValue(QxrdSettingsSaverPtr(), this,"triggerBPrevValue", 0),
  m_TriggerBTriggered(QxrdSettingsSaverPtr(), this,"triggerBTriggered", false),
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

