#include "qxrdplugininfomodel.h"
#include <QIcon>

QxrdPluginInfoModel::QxrdPluginInfoModel()
{
  m_ClassNames.resize(LastPlugin);
  m_Loaded.resize(LastPlugin);
  m_Addresses.resize(LastPlugin);
  m_Paths.resize(LastPlugin);
  m_Files.resize(LastPlugin);
  m_ErrorStrings.resize(LastPlugin);

  m_ClassNames.replace(AreaDetectorPlugin,    "QxrdAreaDetectorPlugin");
  m_ClassNames.replace(DexelaPlugin,          "QxrdDexelaPlugin");
  m_ClassNames.replace(NIDAQPlugin,           "QxrdNIDAQPlugin");
  m_ClassNames.replace(PerkinElmerPlugin,     "QxrdPerkinElmerPlugin");
  m_ClassNames.replace(PilatusPlugin,         "QxrdPilatusPlugin");
  m_ClassNames.replace(SimulatedPlugin,       "QxrdSimulatedPlugin");
}

int QxrdPluginInfoModel::rowCount(const QModelIndex &parent) const
{
  return LastPlugin;
}

int QxrdPluginInfoModel::columnCount(const QModelIndex &parent) const
{
  return LastColumn;
}

QVariant QxrdPluginInfoModel::data(const QModelIndex &index, int role) const
{
  int r = index.row();
  int c = index.column();

  if (role == Qt::DisplayRole) {
    switch (c) {
    case IndexColumn:
      return r;
      break;

    case FileNameColumn:
      return m_Files.value(r);
      break;

    case ClassNameColumn:
      return m_ClassNames.value(r);
      break;

    case AddressColumn:
      if (m_Addresses.value(r)) {
        return tr("0x%1").arg(m_Addresses.value(r),2*sizeof(void*), 16, QChar('0'));
      } else {
        return "- N/A -";
      }
      break;
    }
  }

  if (role == Qt::DecorationRole) {
    if (c == LoadedColumn) {
      if (m_Loaded.value(r)) {
        return QIcon(":/images/button_ok.png");
      }
    }
  }

  if (role == Qt::ToolTipRole) {
    QString msg = tr("%1 plugin").arg(m_ClassNames.value(r));

    if (m_Addresses.value(r)) {
      msg.append(tr(" loaded at 0x%1").arg(m_Addresses.value(r),2*sizeof(void*), 16, QChar('0')));
    } else {
      msg.append(" not loaded");
    }

    if (m_Paths.value(r).length()) {
      msg.append(tr("\nFrom %1").arg(m_Paths.value(r)));
    }

    if (m_ErrorStrings.value(r).length()) {
      msg.append(tr("\nBecause: %1").arg(m_ErrorStrings.value(r)));
    }

    return msg;
  }

  if (role == Qt::TextAlignmentRole) {
    if (c == AddressColumn) {
      return Qt::AlignCenter;
    }
  }

  return QVariant();
}

QVariant QxrdPluginInfoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      switch (section) {
      case IndexColumn:
        return "#";
        break;

      case FileNameColumn:
        return "File";
        break;

      case ClassNameColumn:
        return "Class";
        break;

      case LoadedColumn:
        return "LD?";
        break;

      case AddressColumn:
        return "Address";
        break;
      }
    }
  }

  return QVariant();
}

Qt::ItemFlags QxrdPluginInfoModel::flags(const QModelIndex &index) const
{
  return QAbstractTableModel::flags(index);
}

void QxrdPluginInfoModel::appendEntry(QString path,
                                      QString file,
                                      QString className,
                                      int     loaded,
                                      quint64 address,
                                      QString errorString)
{
  int index = -1;

  if (className == "QxrdAreaDetectorPlugin") {
    index = AreaDetectorPlugin;
  } else if (className == "QxrdDexelaPlugin") {
    index = DexelaPlugin;
  } else if (className == "QxrdNIDAQPlugin") {
    index = NIDAQPlugin;
  } else if (className == "QxrdPerkinElmerPlugin") {
    index = PerkinElmerPlugin;
  } else if (className == "QxrdPilatusPlugin") {
    index = PilatusPlugin;
  } else if (className == "QxrdSimulatedPlugin") {
    index = SimulatedPlugin;
  }

  if (index >= 0) {
    m_Paths.replace(index, path);
    m_Files.replace(index, file);
    m_ClassNames.replace(index, className);
    m_Loaded.replace(index, loaded);
    m_Addresses.replace(index, address);
    m_ErrorStrings.replace(index, errorString);
  }
}
