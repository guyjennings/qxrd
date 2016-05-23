#ifndef QXRDDETECTORDRIVER_H
#define QXRDDETECTORDRIVER_H

#include "qcepobject.h"

class QxrdDetectorDriver : public QcepObject
{
  Q_OBJECT

public:
  QxrdDetectorDriver(QString name);

public slots:
  virtual bool startDetectorDriver();
  virtual bool stopDetectorDriver();
};

#endif // QXRDDETECTORDRIVER_H
