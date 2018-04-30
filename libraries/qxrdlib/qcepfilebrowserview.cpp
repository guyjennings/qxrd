#include "qcepfilebrowserview.h"
#include <QTime>
#include <stdio.h>
#include "qcepapplication.h"

QcepFileBrowserView::QcepFileBrowserView(QWidget *parent) :
  inherited(parent)
{
  setSortingEnabled(true);
  sortByColumn(0, Qt::AscendingOrder);
}

void QcepFileBrowserView::paintEvent(QPaintEvent *event)
{
  QTime tic;
  tic.start();

  inherited::paintEvent(event);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    if (g_Application) {
      g_Application->printMessage(
            tr("QcepFileBrowserView::paintEvent took %1 msec").arg(elapsed));
    }
  }
}

void	QcepFileBrowserView::dataChanged (const QModelIndex & topLeft, const QModelIndex & bottomRight , const QVector<int> &roles)
{
  QTime tic;
  tic.start();

  inherited::dataChanged(topLeft,bottomRight,roles);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    if (g_Application) {
      g_Application->printMessage(
            tr("QcepFileBrowserView::dataChanged took %1 msec").arg(elapsed));
    }
  }
}

QModelIndex	QcepFileBrowserView::indexAt ( const QPoint & point ) const
{
  QTime tic;
  tic.start();

  QModelIndex result = inherited::indexAt(point);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    if (g_Application) {
      g_Application->printMessage(
            tr("QcepFileBrowserView::indexAt took %1 msec").arg(elapsed));
    }
  }

  return result;
}

void	QcepFileBrowserView::keyboardSearch ( const QString & search )
{
  QTime tic;
  tic.start();

  inherited::keyboardSearch(search);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    if (g_Application) {
      g_Application->printMessage(
            tr("QcepFileBrowserView::keyboardSearch took %1 msec").arg(elapsed));
    }
  }
}

void	QcepFileBrowserView::reset ()
{
  QTime tic;
  tic.start();

  inherited::reset();

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    if (g_Application) {
      g_Application->printMessage(
            tr("QcepFileBrowserView::reset took %1 msec").arg(elapsed));
    }
  }
}

void	QcepFileBrowserView::scrollTo ( const QModelIndex & index, ScrollHint hint )
{
  QTime tic;
  tic.start();

  inherited::scrollTo(index, hint);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    if (g_Application) {
      g_Application->printMessage(
            tr("QcepFileBrowserView::scrollTo took %1 msec").arg(elapsed));
    }
  }
}

void	QcepFileBrowserView::selectAll ()
{
  QTime tic;
  tic.start();

  inherited::selectAll();

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    if (g_Application) {
      g_Application->printMessage(
            tr("QcepFileBrowserView::selectAll took %1 msec").arg(elapsed));
    }
  }
}

void	QcepFileBrowserView::setModel ( QAbstractItemModel * model )
{
  QTime tic;
  tic.start();

  inherited::setModel(model);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    if (g_Application) {
      g_Application->printMessage(
            tr("QcepFileBrowserView::setModel took %1 msec").arg(elapsed));
    }
  }
}

void	QcepFileBrowserView::setSelectionModel ( QItemSelectionModel * selectionModel )
{
  QTime tic;
  tic.start();

  inherited::setSelectionModel(selectionModel);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    if (g_Application) {
      g_Application->printMessage(
            tr("QcepFileBrowserView::setSelectionModel took %1 msec").arg(elapsed));
    }
  }
}

QRect	QcepFileBrowserView::visualRect ( const QModelIndex & index ) const
{
  QTime tic;
  tic.start();

  QRect result = inherited::visualRect(index);

  int elapsed = tic.elapsed();

  if (elapsed > 1000) {
    if (g_Application) {
      g_Application->printMessage(
            tr("QcepFileBrowserView::visualRect took %1 msec").arg(elapsed));
    }
  }

  return result;
}

