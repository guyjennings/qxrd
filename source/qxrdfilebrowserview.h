#ifndef QXRDFILEBROWSERVIEW_H
#define QXRDFILEBROWSERVIEW_H

#include <QTreeView>

class QxrdFileBrowserView : public QTreeView
{
    Q_OBJECT
public:
    explicit QxrdFileBrowserView(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

public:
    virtual void	dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight );
    virtual QModelIndex	indexAt ( const QPoint & point ) const;
    virtual void	keyboardSearch ( const QString & search );
    virtual void	reset ();
    virtual void	scrollTo ( const QModelIndex & index, ScrollHint hint = EnsureVisible );
    virtual void	selectAll ();
    virtual void	setModel ( QAbstractItemModel * model );
    virtual void	setRootIndex ( const QModelIndex & index );
    virtual void	setSelectionModel ( QItemSelectionModel * selectionModel );
    virtual QRect	visualRect ( const QModelIndex & index ) const;
};

#endif // QXRDFILEBROWSERVIEW_H
