#ifndef QCEPDATASETBROWSERVIEW_H
#define QCEPDATASETBROWSERVIEW_H

#include <QTreeView>

class QcepDatasetBrowserView : public QTreeView
{
  Q_OBJECT

public:
  QcepDatasetBrowserView(QWidget *parent = 0);

  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
};

#endif // QCEPDATASETBROWSERVIEW_H
