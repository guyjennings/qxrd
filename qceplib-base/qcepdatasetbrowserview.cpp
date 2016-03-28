#include "qcepdatasetbrowserview.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include "qcepdataobject.h"
#include <QMimeData>

QcepDatasetBrowserView::QcepDatasetBrowserView(QWidget *parent)
  : QTreeView(parent)
{

}

void QcepDatasetBrowserView::dragEnterEvent(QDragEnterEvent *event)
{
//  if (event->mimeData()->hasFormat(QcepDataObject::mimeType())) {
//    event->acceptProposedAction();
//  }

  const QMimeData *mimeData = event->mimeData();

  if (mimeData) {
    foreach (QString fmt, mimeData->formats()) {
      printf("Mime data has format %s\n", qPrintable(fmt));
    }
  }

  QTreeView::dragEnterEvent(event);
}

void QcepDatasetBrowserView::dropEvent(QDropEvent *event)
{
  QTreeView::dropEvent(event);
}
