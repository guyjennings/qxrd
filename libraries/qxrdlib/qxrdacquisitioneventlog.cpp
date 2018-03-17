#include "qxrdacquisitioneventlog.h"
#include "qxrdacqcommon.h"

QxrdAcquisitionEventLog::QxrdAcquisitionEventLog(QString name)
  : inherited(name),
    m_IsPaused(0),
    m_EventCount(0),
    m_EventMaxCount(1000),
    m_EventCodes(m_EventMaxCount),
    m_EventArgs(m_EventMaxCount),
    m_EventTimes(m_EventMaxCount)
{
}

void QxrdAcquisitionEventLog::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

void QxrdAcquisitionEventLog::clearEventLog()
{
  m_EventCount.store(0);

  emit eventLogChanged();
}

void QxrdAcquisitionEventLog::pauseEventLog()
{
  m_IsPaused = true;
}

void QxrdAcquisitionEventLog::resumeEventLog()
{
  m_IsPaused = false;
}

bool QxrdAcquisitionEventLog::isPaused()
{
  return m_IsPaused;
}

void QxrdAcquisitionEventLog::appendEvent(int eventCode, int eventArg, QDateTime eventTime)
{
  if (!m_IsPaused) {
    int n = m_EventCount.fetchAndAddOrdered(1);

    if (n < m_EventMaxCount) {
      m_EventCodes[n] = eventCode;
      m_EventArgs[n]  = eventArg;
      m_EventTimes[n] = eventTime;

      emit eventLogChanged();
    }
  }
}

int QxrdAcquisitionEventLog::eventCount()
{
  int n = m_EventCount.load();

  if (n < m_EventMaxCount) {
    return n;
  } else {
    return m_EventMaxCount;
  }
}

int QxrdAcquisitionEventLog::eventCode(int i)
{
  return m_EventCodes.value(i);
}

int QxrdAcquisitionEventLog::eventArg(int i)
{
  return m_EventArgs.value(i);
}

QDateTime QxrdAcquisitionEventLog::eventTime(int i)
{
  return m_EventTimes.value(i);
}

QString QxrdAcquisitionEventLog::eventCodeName(int i)
{
  int n = m_EventCodes.value(i);
  QString res = QString();

  switch (n) {
  case QxrdAcqCommon::StartEvent:
    res = "Start";
    break;

  case QxrdAcqCommon::StartAcquireEvent:
    res = "Start Acquire";
    break;

  case QxrdAcqCommon::StartAcquireOnceEvent:
    res = "Start Acquire Once";
    break;

  case QxrdAcqCommon::StartAcquireDarkEvent:
    res = "Start Acquire Dark";
    break;

  case QxrdAcqCommon::StartAcquireIdleEvent:
    res = "Start Acquire Idle";
    break;

  case QxrdAcqCommon::NIDAQStartEvent:
    res = "NIDAQ Start";
    break;

  case QxrdAcqCommon::NIDAQSyncEvent:
    res = "NIDAQ Sync";
    break;

  case QxrdAcqCommon::NIDAQAnalogInputEvent:
    res = "NIDAQ Analog In";
    break;

  case QxrdAcqCommon::NIDAQAnalogPostEvent:
    res = "NIDAQ Analog In Posted";
    break;

  case QxrdAcqCommon::DetectorFrameEvent:
    res = "Detector Frame";
    break;

  case QxrdAcqCommon::DetectorFramePostedEvent:
    res = "Detector Frame Posted";
  }

  return res;
}
