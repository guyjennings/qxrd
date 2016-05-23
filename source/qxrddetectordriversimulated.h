#ifndef QXRDDETECTORDRIVERSIMULATED_H
#define QXRDDETECTORDRIVERSIMULATED_H

#include "qxrddetectordriver.h"

class QxrdDetectorDriverSimulated : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverSimulated(QString name);
};

#endif // QXRDDETECTORDRIVERSIMULATED_H
