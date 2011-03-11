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
//  QTime tic;
//  tic.start();

  QTreeView::paintEvent(event);

//  printf("QxrdFileBrowserView::paintEvent took %d msec\n", tic.elapsed());
}
