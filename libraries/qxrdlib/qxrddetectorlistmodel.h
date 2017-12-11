#ifndef QXRDDETECTORLISTMODEL_H
#define QXRDDETECTORLISTMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractListModel>
#include "qxrddetectorsettings-ptr.h"

class QXRD_EXPORT QxrdDetectorListModel : public QAbstractListModel
{
  Q_OBJECT

public:
  QxrdDetectorListModel();

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;

  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;

  bool setData(const QModelIndex &index, const QVariant &value, int role);

  void append(QxrdDetectorSettingsPtr det);
  void removeDetector(int row);

  void moveDetectorDown(int row);
  void moveDetectorUp(int row);

  void configureDetector(int row);

  QxrdDetectorSettingsPtr detector(int i);

private:
  void renumberDetectors();

private:
  QVector<QxrdDetectorSettingsPtr> m_Detectors;
};

#endif // QXRDDETECTORLISTMODEL_H
