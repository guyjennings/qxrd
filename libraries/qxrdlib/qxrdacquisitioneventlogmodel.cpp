#include "qxrdacquisitioneventlogmodel.h"
#include "qxrdacquisitioneventlog.h"

QxrdAcquisitionEventLogModel::QxrdAcquisitionEventLogModel(QxrdAcquisitionEventLogWPtr l)
  : m_AcquisitionEventLog(l)
{
  QxrdAcquisitionEventLogPtr log(m_AcquisitionEventLog);

  if (log) {
    connect(log.data(), &QxrdAcquisitionEventLog::eventLogChanged,
            this,       &QxrdAcquisitionEventLogModel::onEventLogChanged);
  }
}

void QxrdAcquisitionEventLogModel::onEventLogChanged()
{
  beginResetModel();

  endResetModel();
}

void QxrdAcquisitionEventLogModel::clearEventLog()
{
  QxrdAcquisitionEventLogPtr log(m_AcquisitionEventLog);

  if (log) {
    log->clearEventLog();
  }
}

void QxrdAcquisitionEventLogModel::pauseEventLog()
{
  QxrdAcquisitionEventLogPtr log(m_AcquisitionEventLog);

  if (log) {
    log->pauseEventLog();
  }
}

void QxrdAcquisitionEventLogModel::resumeEventLog()
{
  QxrdAcquisitionEventLogPtr log(m_AcquisitionEventLog);

  if (log) {
    log->resumeEventLog();
  }
}

bool QxrdAcquisitionEventLogModel::isPaused()
{
  bool res = false;

  QxrdAcquisitionEventLogPtr log(m_AcquisitionEventLog);

  if (log) {
    res = log->isPaused();
  }

  return res;
}

int QxrdAcquisitionEventLogModel::rowCount(const QModelIndex &parent) const
{
  int res = 0;

  QxrdAcquisitionEventLogPtr log(m_AcquisitionEventLog);

  if (log) {
    res = log->eventCount();
  }

  return res;
}

int QxrdAcquisitionEventLogModel::columnCount(const QModelIndex &parent) const
{
  return EventColumnCount;
}

QVariant QxrdAcquisitionEventLogModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();
  QxrdAcquisitionEventLogPtr log(m_AcquisitionEventLog);
  int arg1, arg2;

  if (log && index.isValid() && role == Qt::DisplayRole) {
    int row = index.row();
    int col = index.column();

    switch (col) {
    case EventNumberColumn:
      res = row;
      break;

    case EventCodeColumn:
      res = log->eventCodeName(row);
      break;

    case EventArg1Column:
      arg1 = log->eventArg1(row);

      if (arg1 >= 0) {
        res = arg1;
      }
      break;

    case EventArg2Column:
      arg2 = log->eventArg2(row);

      if (arg2 >= 0) {
        res = arg2;
      }
      break;

    case EventTimeColumn:
      res = (log->eventTime(row).toMSecsSinceEpoch() - log->eventTime(0).toMSecsSinceEpoch())/1000.0;
      break;

    case EventDeltaColumn:
      if (row >= 1) {
        res = (log->eventTime(row).toMSecsSinceEpoch() - log->eventTime(row-1).toMSecsSinceEpoch())/1000.0;
      }
      break;
    }
  }

  return res;
}

QVariant QxrdAcquisitionEventLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant res = QVariant();

  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      switch (section) {
      case EventNumberColumn:
        res = "#";
        break;

      case EventCodeColumn:
        res = "Code";
        break;

      case EventArg1Column:
        res = "Arg 1";
        break;

      case EventArg2Column:
        res = "Arg 2";
        break;

      case EventTimeColumn:
        res = "Time";
        break;

      case EventDeltaColumn:
        res = "Delta Time";
        break;
      }
    }
  }

  return res;
}
