#ifndef QXRDFILEBROWSERMODEL_H
#define QXRDFILEBROWSERMODEL_H

#include <QFileSystemModel>

class QxrdFileBrowserModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit QxrdFileBrowserModel(QObject *parent = 0);

signals:

public slots:

public:
  virtual bool	canFetchMore ( const QModelIndex & parent ) const;
  virtual int	columnCount ( const QModelIndex & parent = QModelIndex() ) const;
  virtual QVariant	data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
  virtual bool	dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent );
  virtual void	fetchMore ( const QModelIndex & parent );
  virtual Qt::ItemFlags	flags ( const QModelIndex & index ) const;
  virtual bool	hasChildren ( const QModelIndex & parent = QModelIndex() ) const;
  virtual QVariant	headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
  QModelIndex	index ( const QString & path, int column = 0 ) const;
  virtual QModelIndex	index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
  virtual QMimeData *	mimeData ( const QModelIndexList & indexes ) const;
  virtual QStringList	mimeTypes () const;
  virtual QModelIndex	parent ( const QModelIndex & index ) const;
  virtual int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;
  virtual bool	setData ( const QModelIndex & idx, const QVariant & value, int role = Qt::EditRole );
  virtual void	sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );
  virtual Qt::DropActions	supportedDropActions () const;
};

#endif // QXRDFILEBROWSERMODEL_H
