#ifndef QXRDDETECTORDRIVERPILATUS_H
#define QXRDDETECTORDRIVERPILATUS_H

#include "qxrddetectordriver.h"

class QxrdDetectorDriverPilatus : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverPilatus(QString name);
};

#endif // QXRDDETECTORDRIVERPILATUS_H
