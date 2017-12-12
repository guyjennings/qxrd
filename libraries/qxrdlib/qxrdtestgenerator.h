#ifndef QXRDTESTGENERATOR_H
#define QXRDTESTGENERATOR_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepobject.h"
#include "qcepdatacolumnscan-ptr.h"
#include <QTimer>

//TODO: eliminate?
class QXRD_EXPORT QxrdTestGenerator : public QcepObject
{
  Q_OBJECT

public:
  QxrdTestGenerator(QString name);

  static void registerMetaTypes();

public slots:
  void repRateZero();
  void repRate10Hz();
  void repRate1Sec();
  void repRate10Sec();
  void generateOne();

protected slots:
  virtual void generateTest() = 0;

public:
  Q_PROPERTY(int testIndex READ get_TestIndex WRITE set_TestIndex STORED false)
  QCEP_INTEGER_PROPERTY(TestIndex)

private:
  QTimer m_Timer;
};

#endif // QXRDTESTGENERATOR_H