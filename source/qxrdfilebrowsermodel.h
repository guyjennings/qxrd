#ifndef QXRDFILEBROWSERMODEL_H
#define QXRDFILEBROWSERMODEL_H

////#define OLD_FILE_BROWSER

//#ifdef OLD_FILE_BROWSER
//#include <QFileSystemModel>

//class QxrdFileBrowserModel : public QFileSystemModel
//{
//    Q_OBJECT
//public:
//    explicit QxrdFileBrowserModel(QObject *parent = 0);

//signals:

//public slots:

//public:
//  virtual bool	canFetchMore ( const QModelIndex & parent ) const;
//  virtual int	columnCount ( const QModelIndex & parent = QModelIndex() ) const;
//  virtual QVariant	data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
//  virtual bool	dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent );
//  virtual void	fetchMore ( const QModelIndex & parent );
//  virtual Qt::ItemFlags	flags ( const QModelIndex & index ) const;
//  virtual bool	hasChildren ( const QModelIndex & parent = QModelIndex() ) const;
//  virtual QVariant	headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
//  QModelIndex	index ( const QString & path, int column = 0 ) const;
//  virtual QModelIndex	index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
//  virtual QMimeData *	mimeData ( const QModelIndexList & indexes ) const;
//  virtual QStringList	mimeTypes () const;
//  virtual QModelIndex	parent ( const QModelIndex & index ) const;
//  virtual int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;
//  virtual bool	setData ( const QModelIndex & idx, const QVariant & value, int role = Qt::EditRole );
//  virtual void	sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );
//  virtual Qt::DropActions	supportedDropActions () const;
//};

//#else

#include <QAbstractTableModel>
#include <QVector>
#include <QFileInfo>

class QxrdFileBrowserModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit QxrdFileBrowserModel(QObject *parent=0);
  typedef QAbstractTableModel inherited;

  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  QModelIndex index(const QString &path) const;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  void setNameFilters(QStringList filters);
  void setNameFilterDisables(bool disables);

  QString fileName(const QModelIndex &index);
  QString filePath(const QModelIndex &index) const;

  void setRootPath(QString path);

  bool isDir(const QModelIndex &index) const;
  virtual bool hasChildren ( const QModelIndex &index) const;

  virtual void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );

private:
  QFileInfo          m_RootPath;
  QVector<QFileInfo> m_FileList;
};

//#endif

#endif // QXRDFILEBROWSERMODEL_H
