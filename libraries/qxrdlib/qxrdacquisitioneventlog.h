#ifndef QXRDACQUISITIONEVENTLOG_H
#define QXRDACQUISITIONEVENTLOG_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include <QDateTime>
#include <QAtomicInt>
#include <QVector>

class QXRD_EXPORT QxrdAcquisitionEventLog : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdAcquisitionEventLog(QString name);
  void initialize(QcepObjectWPtr parent);

  void clearEventLog();
  void pauseEventLog();
  void resumeEventLog();
  bool isPaused();

  void appendEvent(int eventCode,
                   int eventArg1 = -1,
                   int eventArg2 = -1,
                   QDateTime eventTime=QDateTime::currentDateTime());

  int  eventCount();
  int  eventCode(int i);
  QString eventCodeName(int i);
  int  eventArg1(int i);
  int  eventArg2(int i);
  QDateTime eventTime(int i);

signals:
  void eventLogChanged();

private:
  bool               m_IsPaused;
  QAtomicInt         m_EventCount;
  int                m_EventMaxCount;
  QVector<int>       m_EventCodes;
  QVector<int>       m_EventArg1s;
  QVector<int>       m_EventArg2s;
  QVector<QDateTime> m_EventTimes;
};

#endif // QXRDACQUISITIONEVENTLOG_H
