#ifndef QXRDACQUISITIONEVENTLOGMODEL_H
#define QXRDACQUISITIONEVENTLOGMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractTableModel>
#include "qxrdacquisitioneventlog-ptr.h"

class QXRD_EXPORT QxrdAcquisitionEventLogModel : public QAbstractTableModel
{
  Q_OBJECT

private:
  typedef QAbstractTableModel inherited;

public:
  QxrdAcquisitionEventLogModel(QxrdAcquisitionEventLogWPtr l);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  void onEventLogChanged();

  void clearEventLog();
  void pauseEventLog();
  void resumeEventLog();
  bool isPaused();

  enum {
    EventNumberColumn,
    EventTimeColumn,
    EventDeltaColumn,
    EventCodeColumn,
    EventArg1Column,
    EventArg2Column,
    EventColumnCount
  };

private:
  QxrdAcquisitionEventLogWPtr m_AcquisitionEventLog;
};

#endif // QXRDAQUISITIONEVENTLOGMODEL_H
