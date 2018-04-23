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
    SimulatedPlugin,
    PerkinElmerPlugin,
    DexelaPlugin,
    PilatusPlugin,
    AreaDetectorPlugin,
    AlliedVisionPlugin,
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

  void appendEntry(QString path, QString file, QString className, int loaded, quint64 address, QString errorString);

private:
  QVector<QString>    m_Paths;
  QVector<QString>    m_Files;
  QVector<QString>    m_ClassNames;
  QVector<int>        m_Loaded;
  QVector<quint64>    m_Addresses;
  QVector<QString>    m_ErrorStrings;
};

#endif // QXRDPLUGININFOMODEL_H
