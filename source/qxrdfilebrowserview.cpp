#include "qxrdfilebrowserview.h"
#include <QTime>
#include <stdio.h>
#include "qxrdapplication.h"

QxrdFileBrowserView::QxrdFileBrowserView(QWidget *parent) :
    inherited(parent)
{
  setSortingEnabled(true);
  sortByColumn(0, Qt::AscendingOrder);
}

void QxrdFileBrowserView::paintEvent(QPaintEvent *event)
{
  QTime tic;
  tic.start();

  inherited::paintEvent(event);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    g_Application->printMessage(
          tr("QxrdFileBrowserView::paintEvent took %1 msec").arg(elapsed));
  }
}

void	QxrdFileBrowserView::dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight )
{
  QTime tic;
  tic.start();

  inherited::dataChanged(topLeft,bottomRight);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    g_Application->printMessage(
          tr("QxrdFileBrowserView::dataChanged took %1 msec").arg(elapsed));
  }
}

QModelIndex	QxrdFileBrowserView::indexAt ( const QPoint & point ) const
{
  QTime tic;
  tic.start();

  QModelIndex result = inherited::indexAt(point);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    g_Application->printMessage(
          tr("QxrdFileBrowserView::indexAt took %1 msec").arg(elapsed));
  }

  return result;
}

void	QxrdFileBrowserView::keyboardSearch ( const QString & search )
{
  QTime tic;
  tic.start();

  inherited::keyboardSearch(search);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    g_Application->printMessage(
          tr("QxrdFileBrowserView::keyboardSearch took %1 msec").arg(elapsed));
  }
}

void	QxrdFileBrowserView::reset ()
{
  QTime tic;
  tic.start();

  inherited::reset();

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    g_Application->printMessage(
          tr("QxrdFileBrowserView::reset took %1 msec").arg(elapsed));
  }
}

void	QxrdFileBrowserView::scrollTo ( const QModelIndex & index, ScrollHint hint )
{
  QTime tic;
  tic.start();

  inherited::scrollTo(index, hint);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    g_Application->printMessage(
          tr("QxrdFileBrowserView::scrollTo took %1 msec").arg(elapsed));
  }
}

void	QxrdFileBrowserView::selectAll ()
{
  QTime tic;
  tic.start();

  inherited::selectAll();

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    g_Application->printMessage(
          tr("QxrdFileBrowserView::selectAll took %1 msec").arg(elapsed));
  }
}

void	QxrdFileBrowserView::setModel ( QAbstractItemModel * model )
{
  QTime tic;
  tic.start();

  inherited::setModel(model);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    g_Application->printMessage(
          tr("QxrdFileBrowserView::setModel took %1 msec").arg(elapsed));
  }
}

void	QxrdFileBrowserView::setRootPath ( const QString & index )
{
}

void	QxrdFileBrowserView::setSelectionModel ( QItemSelectionModel * selectionModel )
{
  QTime tic;
  tic.start();

  inherited::setSelectionModel(selectionModel);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    g_Application->printMessage(
          tr("QxrdFileBrowserView::setSelectionModel took %1 msec").arg(elapsed));
  }
}

QRect	QxrdFileBrowserView::visualRect ( const QModelIndex & index ) const
{
  QTime tic;
  tic.start();

  QRect result = inherited::visualRect(index);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    g_Application->printMessage(
          tr("QxrdFileBrowserView::visualRect took %1 msec").arg(elapsed));
  }

  return result;
}

