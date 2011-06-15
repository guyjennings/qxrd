#ifndef QXRDFILEBROWSERMODELUPDATER_H
#define QXRDFILEBROWSERMODELUPDATER_H

#include <QObject>
#include "qxrdfilebrowsermodel.h"

class QxrdFileBrowserModelUpdater : public QObject
{
    Q_OBJECT
public:
    explicit QxrdFileBrowserModelUpdater(QxrdFileBrowserModel *browser, QObject *parent = 0);

signals:

public slots:
  void shutdown();
};

#endif // QXRDFILEBROWSERMODELUPDATER_H
