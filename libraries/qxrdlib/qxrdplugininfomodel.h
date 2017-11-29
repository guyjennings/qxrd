#ifndef QXRDPLUGININFOMODEL_H
#define QXRDPLUGININFOMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractTableModel>
#include <QString>

class QXRD_EXPORT QxrdPluginInfoModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit QxrdPluginInfoModel();

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  enum {
    NIDAQPlugin,
    CudaPlugin,
    SimulatedDetectorPlugin,
    PerkinElmerDetectorPlugin,
    DexelaDetectorPlugin,
    PilatusDetectorPlugin,
    AreaDetectorPlugin,
    LastPlugin
  };

  enum {
    IndexColumn,
    LoadedColumn,
    ClassNameColumn,
    AddressColumn,
    FileNameColumn,
    LastColumn
  };

  void appendEntry(QString path, QString className, int loaded, quint64 address);

private:
  QVector<QString>    m_Paths;
  QVector<QString>    m_ClassNames;
  QVector<int>        m_Loaded;
  QVector<quint64>    m_Addresses;
};

#endif // QXRDPLUGININFOMODEL_H
