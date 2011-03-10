#include "qxrdfilebrowserview.h"
#include <QTime>
#include <stdio.h>

QxrdFileBrowserView::QxrdFileBrowserView(QWidget *parent) :
    QTreeView(parent)
{
  setSortingEnabled(true);
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
