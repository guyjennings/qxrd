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
};

#endif // QXRDFILEBROWSERVIEW_H
