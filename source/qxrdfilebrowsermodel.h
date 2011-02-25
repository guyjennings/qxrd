#ifndef QXRDFILEBROWSERMODEL_H
#define QXRDFILEBROWSERMODEL_H

#include <QFileSystemModel>

class QxrdFileBrowserModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit QxrdFileBrowserModel(QObject *parent = 0);

signals:

public slots:

};

#endif // QXRDFILEBROWSERMODEL_H
