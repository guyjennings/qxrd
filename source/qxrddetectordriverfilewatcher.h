#ifndef QXRDDETECTORDRIVERFILEWATCHER_H
#define QXRDDETECTORDRIVERFILEWATCHER_H

#include "qxrddetectordriver.h"

class QxrdDetectorDriverFileWatcher : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverFileWatcher(QString name);
};

#endif // QXRDDETECTORDRIVERFILEWATCHER_H
