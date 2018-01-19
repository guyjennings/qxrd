#ifndef QXRDZINGERFINDER_H
#define QXRDZINGERFINDER_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qcepproperty.h"

class QXRD_EXPORT QxrdZingerFinder : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdZingerFinder(QString name);
  virtual ~QxrdZingerFinder();

  enum {
    ZingerNoMask = -2,
    ZingerDataMask = -1,
    ZingerTopStack = -1
  } ZingerConstants;

  void findZingers();

public:
  Q_PROPERTY(int zingerAlgorithm READ get_ZingerAlgorithm WRITE set_ZingerAlgorithm)
  QCEP_INTEGER_PROPERTY(ZingerAlgorithm)

  Q_PROPERTY(int zingerMask READ get_ZingerMask WRITE set_ZingerMask)
  QCEP_INTEGER_PROPERTY(ZingerMask)

  Q_PROPERTY(int zingerDestination READ get_ZingerDestination WRITE set_ZingerDestination)
  QCEP_INTEGER_PROPERTY(ZingerDestination)

  Q_PROPERTY(int zingerSize1 READ get_ZingerSize1 WRITE set_ZingerSize1)
  QCEP_INTEGER_PROPERTY(ZingerSize1)

  Q_PROPERTY(int zingerSize2 READ get_ZingerSize2 WRITE set_ZingerSize2)
  QCEP_INTEGER_PROPERTY(ZingerSize2)

  Q_PROPERTY(double zingerThreshold READ get_ZingerThreshold WRITE set_ZingerThreshold)
  QCEP_DOUBLE_PROPERTY(ZingerThreshold)
};

#endif // QXRDZINGERFINDER_H
