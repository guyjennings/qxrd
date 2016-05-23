#ifndef QXRDDETECTORDRIVEREPICSAREA_H
#define QXRDDETECTORDRIVEREPICSAREA_H

#include "qxrddetectordriver.h"

class QxrdDetectorDriverEpicsArea : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverEpicsArea(QString name);
};

#endif // QXRDDETECTORDRIVEREPICSAREA_H
