#ifndef QXRDTESTGENERATOR_H
#define QXRDTESTGENERATOR_H

#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepserializableobject.h"
#include "qcepdatacolumnscan-ptr.h"
#include <QTimer>

class QxrdTestGenerator : public QcepSerializableObject
{
public:
  QxrdTestGenerator(QString name);

public slots:
  void repRateZero();
  void repRate10Hz();
  void repRate1Sec();
  void repRate10Sec();
  void generateOne();

protected slots:
  virtual void generateTest() = 0;

private:
  QTimer m_Timer;
};

#endif // QXRDTESTGENERATOR_H
