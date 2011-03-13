#include "qxrdfilebrowserview.h"
#include <QTime>
#include <stdio.h>

QxrdFileBrowserView::QxrdFileBrowserView(QWidget *parent) :
    QTreeView(parent)
{
  setSortingEnabled(true);
  sortByColumn(0, Qt::AscendingOrder);
}

void QxrdFileBrowserView::paintEvent(QPaintEvent *event)
{
  QTime tic;
  tic.start();

  QTreeView::paintEvent(event);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    printf("QxrdFileBrowserView::paintEvent took %d msec\n", elapsed);
  }
}

void	QxrdFileBrowserView::dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight )
{
  QTime tic;
  tic.start();

  QTreeView::dataChanged(topLeft,bottomRight);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    printf("QxrdFileBrowserView::dataChanged took %d msec\n", elapsed);
  }
}

QModelIndex	QxrdFileBrowserView::indexAt ( const QPoint & point ) const
{
  QTime tic;
  tic.start();

  QModelIndex result = QTreeView::indexAt(point);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    printf("QxrdFileBrowserView::indexAt took %d msec\n", elapsed);
  }

  return result;
}

void	QxrdFileBrowserView::keyboardSearch ( const QString & search )
{
  QTime tic;
  tic.start();

  QTreeView::keyboardSearch(search);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    printf("QxrdFileBrowserView::keyboardSearch took %d msec\n", elapsed);
  }
}

void	QxrdFileBrowserView::reset ()
{
  QTime tic;
  tic.start();

  QTreeView::reset();

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    printf("QxrdFileBrowserView::reset took %d msec\n", elapsed);
  }
}

void	QxrdFileBrowserView::scrollTo ( const QModelIndex & index, ScrollHint hint )
{
  QTime tic;
  tic.start();

  QTreeView::scrollTo(index, hint);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    printf("QxrdFileBrowserView::scrollTo took %d msec\n", elapsed);
  }
}

void	QxrdFileBrowserView::selectAll ()
{
  QTime tic;
  tic.start();

  QTreeView::selectAll();

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    printf("QxrdFileBrowserView::selectAll took %d msec\n", elapsed);
  }
}

void	QxrdFileBrowserView::setModel ( QAbstractItemModel * model )
{
  QTime tic;
  tic.start();

  QTreeView::setModel(model);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    printf("QxrdFileBrowserView::setModel took %d msec\n", elapsed);
  }
}

void	QxrdFileBrowserView::setRootIndex ( const QModelIndex & index )
{
  QTime tic;
  tic.start();

  QTreeView::setRootIndex(index);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    printf("QxrdFileBrowserView::setRootIndex took %d msec\n", elapsed);
  }
}

void	QxrdFileBrowserView::setSelectionModel ( QItemSelectionModel * selectionModel )
{
  QTime tic;
  tic.start();

  QTreeView::setSelectionModel(selectionModel);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    printf("QxrdFileBrowserView::setSelectionModel took %d msec\n", elapsed);
  }
}

QRect	QxrdFileBrowserView::visualRect ( const QModelIndex & index ) const
{
  QTime tic;
  tic.start();

  QRect result = QTreeView::visualRect(index);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    printf("QxrdFileBrowserView::visualRect took %d msec\n", elapsed);
  }

  return result;
}

